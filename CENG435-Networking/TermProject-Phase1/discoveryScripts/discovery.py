import socket
import sys
import time
import threading


#link s-r1
s_r1 = ('10.10.1.1',4000)

#link s-r2
s_r2 = ('10.10.2.2',4000)

#link s-r3
s_r3 = ('10.10.3.1',4000)

#link r1-d
r1_d = ('10.10.4.2',4000)

#link r2-d
r2_d = ('10.10.5.2',4000)

#link r2-r3
r2_r3 = ('10.10.6.1',4000)

#link r3-d
r3_d = ('10.10.7.1',4000)

#link r1-r2
r1_r2 = ('10.10.8.2',4000)

# used for stored link cost
linkCost = []


#Creates  UDP  socket  and binds its interface address according to intended link. 
#Then, it starts to wait a message from a client. 
#When a message came to  the  server,  server  sends  response  message  to  client
#Not to kill process in terminal every time, after all the messages transmitted, client sent 'Finished' message to stop server
def server(server_address):

    print 'Opening Socket'
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(server_address)

    while True:
        data, address = sock.recvfrom(4096)

        if data == 'Finished':
            break

        if data:
            sent = sock.sendto(data, address)
    
    print 'Closing Socket'
    sock.close()           

        

#Creates  UDP  socket  and  sends  multiple  number  of  messages to  intended  address
#Not to kill process in terminal every time, after all the messages are sent, client sent 'Finished' message to stop server
#It calculates avg RTT time in ms and store value to linkCost array
def client(target_address,fromTo):
    global linkCost

    print 'Opening Socket'
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    message = 'Sending'
    avgTime = 0;

    try:
        for i in range(1000):

            start = time.time()
            sent = sock.sendto(message, target_address)

            data, server = sock.recvfrom(4096)
            end = time.time()

            avgTime += (end-start)*1000

    finally:
        sock.sendto('Finished',target_address)
        avgTime/= 1000
        linkCost.append(fromTo + ' : ' + str(avgTime) + ' ms')
        print fromTo , avgTime 
        print 'Closing Socket'
        sock.close()


#getting command line argumant to determine which node it is
thisNode = str(sys.argv[1])


#to able to handle multiple messages at the same time
threads = list()


#according to command line argumant, it opens clients and servers on that node by threading
if(thisNode == 's'):
    
    x = threading.Thread(target=server, args=(s_r1,))
    threads.append(x)
    x.start()

    x = threading.Thread(target=server, args=(s_r2,))
    threads.append(x)
    x.start()

    x = threading.Thread(target=server, args=(s_r3,))
    threads.append(x)
    x.start()

elif(thisNode == 'r1'):
    x = threading.Thread(target=client, args=(s_r1,'r1 -> s',))
    threads.append(x)
    x.start()

    x = threading.Thread(target=client, args=(r1_r2,'r1 -> r2',))
    threads.append(x)
    x.start()

    x = threading.Thread(target=client, args=(r1_d,'r1 -> d',))
    threads.append(x)
    x.start()

elif(thisNode == 'r2'):
    x = threading.Thread(target=server, args=(r2_r3,))
    threads.append(x)
    x.start()

    x = threading.Thread(target=server, args=(r1_r2,))
    threads.append(x)
    x.start()

    x = threading.Thread(target=client, args=(r2_d,'r2 -> s',))
    threads.append(x)
    x.start()

    x = threading.Thread(target=client, args=(s_r2,'r2 -> d',))
    threads.append(x)
    x.start()


elif(thisNode == 'r3'):
    x = threading.Thread(target=client, args=(s_r3,'r3 -> s',))
    threads.append(x)
    x.start()

    x = threading.Thread(target=client, args=(r2_r3,'r3 -> r2',))
    threads.append(x)
    x.start()

    x = threading.Thread(target=client, args=(r3_d,'r3 -> d',))
    threads.append(x)
    x.start()

elif(thisNode == 'd'):
    x = threading.Thread(target=server, args=(r1_d,))
    threads.append(x)
    x.start()

    x = threading.Thread(target=server, args=(r2_d,))
    threads.append(x)
    x.start()

    x = threading.Thread(target=server, args=(r3_d,))
    threads.append(x)
    x.start()


#waits until all the threads are done
for t in threads:
    t.join()


#if exists, writes link cost to the file
if(len(linkCost) != 0):
    f = open("link_costs.txt",'w+')
    for string in linkCost:
        f.write(string)
        f.write('\n')

    f.close()




