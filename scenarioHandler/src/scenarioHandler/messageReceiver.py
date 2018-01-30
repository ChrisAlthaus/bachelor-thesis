import socket
from subprocess import check_output
from scenarioHandler import parseMessage


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
conn= None
address=None
    
def setupServer():
    print ("Socket created")
    
    host= check_output(['hostname','-I'])
    port=5560

    try:
        s.bind((host, port))
    except socket.error as msg:
        print(msg)
    print("Socket bind complete.")
    print("Host ip:", host, ",Port:", port)
    return s

def setupConnection():
    global conn
    global address

    if conn is not None:
      conn.close()

    print("Listen on a client...")
    s.listen(1) # Allows one connection at a time.
    conn, address = s.accept()
    print("Connected to: " + address[0] + ":" + str(address[1]))

def receiveMessageAndParse():
    while(True):
       try:
          data = conn.recv(1024)
          data = data.encode("utf-8")
          print("Received data=",data)
       except:
          # happens when connection is reset from the peer
          print("Connection reset")
       	  break;
       
       if data == "quit":
          #quit command to regulary disconnect
          print("Quit current connection")
          break
       if data == "STATUS:":
	  sendStatusFiles()	
       else:
          parseMessage(data)

def sendFile(filePath):
    with open(filePath) as f:
       print "file position=",f.tell()
       f.seek(0)
       lines = f.readlines()
       
       for line in lines:
          conn.send(line)


def sendStatusFiles():
    conn.send("OUTPUT FILE:\n")
    sendFile('output.log')
    conn.send("ERROR FILE:\n")
    sendFile('error.log')

    conn.send("EOF\n")

def closeTcpConnection():
    if s is not None and conn is not None:
       s.close()
       conn.close()
	            


def getWifiIPAddress():    
    wifi_ip = check_output(['ifconfig', '$wlan | grep -q "inet addr:"'])
    return wifi_ip
