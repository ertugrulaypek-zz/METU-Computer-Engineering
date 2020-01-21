from socket import timeout
import socket
import sys
import time
import threading
import hashlib
import cPickle
import os
import math


#to calculate standard error in the experiment
def StandartError(timeResult,avg):
	error = 0
	for i in range(len(timeResult)):
		error += (avg-timeResult[i])**2
	error /= len(timeResult)
	error = math.sqrt(error)*1.96/math.sqrt(len(timeResult))
	return error


#all used links 
#first two in the name represents node
#p represent port numbers
#p1 is for main usage
#p2 is for backup usage in case of link failure
s_r1_p1 = ('10.10.1.2',4000)
s_r1_p2 = ('10.10.1.2',5000)
s_r2_p1 = ('10.10.2.1',4000)
s_r2_p2 = ('10.10.2.1',5000)
s_r3_p1 = ('10.10.3.2',4000)

r1_s_p1 = ('10.10.1.1',4000)
r1_s_p2 = ('10.10.1.1',5000)
r2_s_p1 = ('10.10.2.2',4000)
r2_s_p2 = ('10.10.2.2',5000)
r3_s_p1 = ('10.10.3.1',4000)

r1_d_p1 = ('10.10.4.2',4000)
r1_d_p2 = ('10.10.4.2',5000)
r2_d_p1 = ('10.10.5.2',4000)
r2_d_p2 = ('10.10.5.2',5000)
r3_d_p1 = ('10.10.7.1',4000)

#forwarding table 
#router method uses this table, forwards messages between client and server
forwardingTable = {}
forwardingTable[r3_s_p1] = r3_d_p1
forwardingTable[r3_d_p1] = r3_s_p1

forwardingTable[r2_s_p2] = r2_d_p2
forwardingTable[r2_d_p2] = r2_s_p2

forwardingTable[r2_s_p1] = r2_d_p1
forwardingTable[r2_d_p1] = r2_s_p1

forwardingTable[r1_s_p2] = r1_d_p2
forwardingTable[r1_d_p2] = r1_s_p2

forwardingTable[r1_s_p1] = r1_d_p1
forwardingTable[r1_d_p1] = r1_s_p1

#to bind the socket of the clients
#if it is not bound manually, computer binds its port automatically
#its port number should be determined because forwarding table needs to know that port number 
bindTable = {}
bindTable[s_r1_p1] = r1_s_p1
bindTable[s_r1_p2] = r1_s_p2
bindTable[s_r2_p1] = r2_s_p1
bindTable[s_r2_p2] = r2_s_p2
bindTable[s_r3_p1] = r3_s_p1

#makes packet that will be sent
#header is actual data 
#for client, it is tuple such that (sequence Number,message)
#for server, it is tuple such that (sequence Number,ACK/NACK)
#It uses md5 in order to create checksum
#It uses cPickle.dumps in order to convert tuple to string
#both md5 and sendto method need string as parameter 
def makePacket(header):
	dataString = cPickle.dumps(header)
	checksum = hashlib.md5(dataString.encode()).hexdigest()

	packetData = (checksum,header)
	packet = cPickle.dumps(packetData)
	return packet

#compares checksum of the received data with current checksum that is generated from received data
#if it data is corrupted, return false
#otherwise, return true
def checkChecksum(receivedData):
	checksum = hashlib.md5((cPickle.dumps(receivedData[1])).encode()).hexdigest()
	return checksum == receivedData[0]


#Creates UDP socket and binds its interface IP address according to intended link
#Forwards messages between client and server according to forwarding table
def router(address):
	
	print 'Opening Socket'
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.bind(address)
	#sock.settimeout(15)
	while True:
		data, address3 = sock.recvfrom(4096)
		print address3
		if data:
			if address3 in forwardingTable:
				sock.sendto(data, forwardingTable[address3])

	print 'Closing Socket'
	sock.close()


#Creates  UDP  socket and binds its interface according to bind table
#It is responsible for sending packets by using pipelining in reliable fashion
#Pipelining and reliability are explained in detail in the report
def client(addNumber,address,indices):

	PayloadSize = 900
	WindowSize = 10

	readedData = indices[0]
	fileSize = indices[1]

	SentBuffer = {}

	seqNo = 0
	waitingSeqNo = 0

	for i in range(addNumber):
		print 'Opening Socket'
		sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		sock.settimeout(0.25)
		#sock.bind(r2_s) 
		sock.bind(bindTable[address[i]])

		timer = time.time()
		lastResponse = time.time()

		while True:			

			while( (len(SentBuffer) < WindowSize) and (readedData < fileSize)):
				header = (seqNo,content[readedData:readedData+PayloadSize])

				if((readedData+PayloadSize)> fileSize):
					header = (seqNo,content[readedData:fileSize])

				packet = makePacket(header)

				##sent packet
				sock.sendto(packet, address[i])

				SentBuffer[seqNo] = packet

				seqNo+=1
				readedData+= PayloadSize



			end = time.time()

			if((end-lastResponse) > 3):
				break

			if((end-timer) > 0.5):
				timer = time.time()
				for j in SentBuffer:
					sock.sendto(SentBuffer[j], address[i])

			if((readedData >= fileSize) and (len(SentBuffer) == 0)):
				break

			try:
				for k in range(len(SentBuffer)):

					data, server = sock.recvfrom(4096)

					lastResponse = time.time()

					receivedData = cPickle.loads(data)

					if(checkChecksum(receivedData)):

						currSeqNo = receivedData[1][0]
						currAck = receivedData[1][1]

						if(currAck == 0):
							if(currSeqNo in SentBuffer):
								SentBuffer.pop(currSeqNo)
								if(currSeqNo == waitingSeqNo):
									timer = time.time()
									if(len(SentBuffer) == 0):
										waitingSeqNo += 1
									else:
										waitingSeqNo = SentBuffer.keys()[0]
			except timeout:
				for j in SentBuffer:
					sock.sendto(SentBuffer[j], address[i]) 
				continue

		print 'Closing Socket'
		sock.close()

		if((readedData >= fileSize) and (len(SentBuffer) == 0)):
			break


#Creates  UDP  socket  and binds its interface address according to intended link.
#It is responsible for receiving the message and write them to a file by using  pipelining in reliable fashion.
#Pipelining and reliability are explained in detail in the report
def server(addNumber,address,fileName):

	f = open(fileName, "w")

	readBuffer = {}
	waitingSeqNo = 0

	for i in range(addNumber):

		print 'Opening Socket'
		sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		sock.bind(address[i])
		sock.settimeout(15)


		timer = time.time()

		while True:
			
			currTime = time.time()
			
			try:
				data, address1 = sock.recvfrom(4096)
			except timeout:
				break   

			if data:
				timer = time.time()
				receivedData = cPickle.loads(data)

				if(checkChecksum(receivedData)):
					
					currSeqNo = receivedData[1][0]
					currMsg = receivedData[1][1]

					packet = makePacket((currSeqNo,0))

					sock.sendto(packet, address1)

					if(currSeqNo == waitingSeqNo):
						f.write(currMsg)
						waitingSeqNo += 1

						while(waitingSeqNo in readBuffer):
							currMsg = readBuffer.pop(waitingSeqNo)
							f.write(currMsg)
							waitingSeqNo+=1

					elif(currSeqNo > waitingSeqNo):
						readBuffer[currSeqNo] = currMsg

				else:

					packet = makePacket((-1,1))

					sock.sendto(packet, address1)

		print 'Closing Socket'
		sock.close()

	f.close()


#getting command line argumant to determine which node it is
thisNode = str(sys.argv[1])

#to handle multi-homing
#also, to handle both experiment transmission at the same time
threads = list()

#according to command line argumant, it opens client,server or routers on that node by threading
if(thisNode == 's'):
	f = open("input.txt","r")
	content = f.read() 

	totalSize = len(content)

	x = threading.Thread(target=client, args=(1,[s_r3_p1],[0,totalSize],))
	threads.append(x)
	x.start()

	x = threading.Thread(target=client, args=(2,[s_r2_p1,s_r1_p2],[0,totalSize/2],))
	threads.append(x)
	x.start()

	x = threading.Thread(target=client, args=(2,[s_r1_p1,s_r2_p2],[totalSize/2,totalSize],))
	threads.append(x)
	x.start()

	for t in threads:
		t.join()

	f.close()

elif(thisNode == 'r1'):
	x = threading.Thread(target=router, args=(s_r1_p1,))
	threads.append(x)
	x.start()

	x = threading.Thread(target=router, args=(s_r1_p2,))
	threads.append(x)
	x.start()

elif(thisNode == 'r2'):
	x = threading.Thread(target=router, args=(s_r2_p1,))
	threads.append(x)
	x.start()

	x = threading.Thread(target=router, args=(s_r2_p2,))
	threads.append(x)
	x.start()

elif(thisNode == 'r3'):
	router(s_r3_p1)

elif(thisNode == 'd'):
	x = threading.Thread(target=server, args=(1,[r3_d_p1],"input1.txt",))
	threads.append(x)
	x.start()

	x = threading.Thread(target=server, args=(2,[r2_d_p1,r1_d_p2],"input2_0.txt",))
	threads.append(x)
	x.start()

	x = threading.Thread(target=server, args=(2,[r1_d_p1,r2_d_p2],"input2_1.txt",))
	threads.append(x)
	x.start()

	for t in threads:
		t.join()

	f = open("input2.txt", "w")
	for i in range(2):
		f1 = open("input2_" + str(i) + ".txt","r")
		content = f1.read() 
		f.write(content)
		f1.close()
		os.system("rm -rf "+"input2_" + str(i) + ".txt")

#experiment 2 with %5 packet loss
elif(thisNode == 'sExp'):
	n = 5
	timeResults = list()
	totalSum = 0
	for i in range(n):
		expStart = time.time()

		f = open("input.txt","r")
		content = f.read() 

		totalSize = len(content)

		x = threading.Thread(target=client, args=(2,[s_r2_p1,s_r1_p2],[0,totalSize/2],))
		threads.append(x)
		x.start()

		x = threading.Thread(target=client, args=(2,[s_r1_p1,s_r2_p2],[totalSize/2,totalSize],))
		threads.append(x)
		x.start()

		for t in threads:
			t.join()

		expEnd = time.time()

		timeResults.append((expEnd - expStart))
		totalSum += ((expEnd - expStart))

	avg1 = totalSum/n
	print avg1
	print StandartError(timeResults,avg1)

#waits until all the threads are done
for t in threads:
	t.join()

