import socket
import Gait_Recognition

soc = socket.socket()
soc.bind(('10.10.20.29',8081))
soc.listen(1)


print ('server started and listening')
c =0
recieved = False
while 1:
    try:
        savefilename = "input_videos/input.mp4"
        (clientsocket, address) = soc.accept()
        print ("connection found!")
        with clientsocket:
            with open(savefilename,'wb') as file:
                size = int(clientsocket.recv(1024).decode())
                while size:
                    recvfile = clientsocket.recv(4096)
                    file.write(recvfile)
                    size-=len(recvfile)
                print("File has been received.")

            #todo: authenticate
            reply = Gait_Recognition.recognize(savefilename)
            #reply='OK'
            if (reply[-4:]==".png"):
                reply = reply[:-4]
            clientsocket.send(reply.encode())
        
    except KeyboardInterrupt:
        print('interrupted!')
        break
