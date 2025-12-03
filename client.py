import socket
import sys
import threading

try:
    pid = sys.argv[1]
except:
    print("No pid given")
    exit(1)
    
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.connect(f'/tmp/cheat_io_{pid}.sock')

def recv_thread():
    while True:
        data = sock.recv(1024)
        sys.stdout.buffer.write(data)
        sys.stdout.buffer.flush()

def send_thread():
    while True:
        data = input().encode()
        sock.send(data)

t1 = threading.Thread(target=recv_thread)
t2 = threading.Thread(target=send_thread)

t1.start()
t2.start()

t1.join()
t2.join()
