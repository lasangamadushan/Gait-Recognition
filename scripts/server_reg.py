import socket
import registration

soc = socket.socket()
soc.bind(('10.10.20.29',8080))
soc.listen(1)

##serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
##host = "192.168.1.3"
##port = 8000
##print (host)
##print (port)
##serversocket.bind((host, port))

#serversocket.listen(5)

print ('server started and listening')
video_file_name = "input_videos/input.mp4"

while 1:
    try:
        (clientsocket, address) = soc.accept()
        print ("connection found!")
        name = clientsocket.recv(1024).decode()
        print (name)
        reply='OK'
        clientsocket.send(reply.encode())
##        if(savefilename[-4:]!=".mp4"):
##            savefilename+=".mp4"
##
##        savefilename = "input_videos/"+savefilename
        #savefilename = input("enter file name to receive: ")
        with clientsocket,open(video_file_name,'wb') as file:
            while True:
                recvfile = clientsocket.recv(4096)
                if not recvfile: break
                file.write(recvfile)
        print("File has been received.")
        registration.register(video_file_name,name)
        
    except KeyboardInterrupt:
        print('interrupted!')
        break
