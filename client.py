import socket
import sys
import threading
import os

def recv_thread():
    while True:
        data = sock.recv(1024)
        sys.stdout.buffer.write(data)
        sys.stdout.buffer.flush()

def send_thread():
    while True:
        data = input().encode()
        sock.send(data)

def find_amongus_pid(cmd: str):
    for filename in os.listdir("/proc"):
        if not filename.isnumeric():
            continue
            
        with open(f"/proc/{filename}/cmdline") as f:
            if cmd in f.read():
                return int(filename)
    
    return None

path = "Z:\\home\\ott\\.local\\share\\Steam\\steamapps\\common\\Among Us\\Among Us.exe"
pid = find_amongus_pid(path)

if not pid:
    print("Among Us not running...")
    exit(1)
    
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.connect(f'/tmp/cheat_io_{pid}.sock')

t1 = threading.Thread(target=recv_thread)
t2 = threading.Thread(target=send_thread)

t1.start()
t2.start()

t1.join()
t2.join()
