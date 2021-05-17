import socket
import numpy as np
import struct

HOST = 'localhost'      # Standard loopback interface address (localhost)
PORT = 8080            # Port to listen on (non-privileged ports are > 1023)

global mode
mode = None 

def receive_read_size(conn):
    i = struct.unpack('i', conn.recv(4))
    return i[0]

def receive_array(conn, size):
    data = np.zeros(size)

    for i in range(size):
        data[i] = struct.unpack('f', conn.recv(4))[0]
    
    return data

def wait_for_mode(conn):
    data = str(conn.recv(32), 'utf-8')
    if data == "inference":
        global mode
        mode = 0
        conn.send('ok'.encode())
    else:
        raise NotImplementedError()

def handle_inference_mode(conn):
    # get state
    
    #obs
    obs_num = receive_read_size(conn)
    obs = receive_array(conn, obs_num)

    #run inference graph

    print("Running Inference Graph")

    #send actions back

    dummy_actions = [1.0, 1.0]

    size = struct.pack("i", 2) # 2 actions
    conn.send(size)

    conn.send(struct.pack('f', dummy_actions[0]))
    conn.send(struct.pack('f', dummy_actions[1]))


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))

    print(f"connected to: {HOST}, at port {PORT}")

    s.listen()

    while True:
        print("Listening for connection")
        conn, addr = s.accept()

        print("Accepting Connection")
        with conn:
            print('Connected by', addr)

            wait_for_mode(conn)
            
            print("Mode Selected: ", mode)

            if mode == 0:
                handle_inference_mode(conn)
            elif mode == 1:
                raise NotImplementedError()

''' while True:
    size = receive_read_size(conn)

    if(size <= 0):
        print("Read End. bye bye.")
        break

    data = receive_array(conn, size)

    print(f"received data: {data}")
    
    #todo read observation/rewards data.  '''
