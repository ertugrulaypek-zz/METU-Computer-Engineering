import socket
import sys
import time
import threading

# used for stored edge to edge cost
e2eCost = []

#link s-r3
s_r3 = ('10.10.3.2',4000)

#link r3-d
r3_d = ('10.10.7.1',4000)


#Creates  UDP  socket  and binds its interface address according to intended link. 
#Then, it starts to wait a message from a client. 
#When a message came to  the  server,  server  sends  response  message  to  client
#Not to kill process in terminal every time, after all the messages transmitted, client sent 'Finished' message to stop server
def server(address):

    print 'Opening Socket'
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(address)

    
    while True:
        data, address = sock.recvfrom(4096)
        
        if data == 'Finished':
            break

        if data:
            sent = sock.sendto(data, address)

    print 'Closing Socket'
    sock.close()


#Creates UDP socket and binds its interface IP address according to link with the client. 
#Forwards messages between client and server.
def router(address,address2):
    
    print 'Opening Socket'
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(address)

    while True:
        data, address = sock.recvfrom(4096)
        
        if data:
            sent = sock.sendto(data, address2)
            if data == 'Finished':
                break
            data2,address3 = sock.recvfrom(4096)
            sent = sock.sendto(data, address)  

    print 'Closing Socket'
    sock.close()


#Creates  UDP  socket  and  sends  multiple  number  of  messages to  intended  address
#Not to kill process in terminal every time, after all the messages are sent, client sent 'Finished' message to stop server
#It calculates edge to edge time*2 in ms and store value to e2eCost array
def client(address):
    global e2eCost

    print 'Opening Socket'
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    message = 'Sending'
    avgTime = 0;

    try:
        for i in range(100):

    
            start = time.time()
            sent = sock.sendto(message, address)
            
            data, server = sock.recvfrom(4096)
            end = time.time()
            
            avgTime += (end-start)*1000

    finally:
        sock.sendto('Finished',address)
        avgTime/= 100
        avgTime/= 2
        e2eCost.append(str(avgTime))
        print 'Closing Socket'
        sock.close()


#getting command line argumant to determine which node it is
thisNode = str(sys.argv[1])

#according to command line argumant, it opens client, server or router.
if(thisNode == 's'):
    client(s_r3)

elif(thisNode == 'r3'):
    router(s_r3,r3_d)

elif(thisNode == 'd'): 
    server(r3_d)


#if exists, writes edge to edge delay to the file
if(len(e2eCost) != 0):
    f = open("e2eDelay.txt",'w+')
    for string in e2eCost:
        f.write(string)
        f.write('\n')

    f.close()


