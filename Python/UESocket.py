import socket
import numpy as np
import struct
import time

from NN import Agent

HOST = 'localhost'      # Standard loopback interface address (localhost)
PORT = 8080            # Port to listen on (non-privileged ports are > 1023)

global mode
mode = None 

def receive_byte_and_format(conn, pack='f', byte_size=4):
    data = struct.unpack(pack, conn.recv(byte_size))
    return data[0]

def receive_int(conn):
    return receive_byte_and_format(conn, 'i', 4)

def receive_float(conn):
    return receive_byte_and_format(conn, 'f', 4)

def receive_array(conn, size):
    data = np.zeros(size)

    for i in range(size):
        data[i] = struct.unpack('f', conn.recv(4))[0]
    
    return np.array(data)

def wait_for_mode(conn):
    print("Waiting for mode.")
    data = str(conn.recv(32), 'utf-8')
    if data == "inference":
        global mode
        mode = 0
        conn.send('ok'.encode())
    else:
        raise NotImplementedError()

def build_NN(conn):
    print("Waiting params to build a NN.")
    input_space = receive_int(conn)
    action_space = receive_int(conn)
    learning_rate = receive_float(conn)

    print(f'Build a NN with input_space: {input_space} and action_space: {action_space} and learning_rate: {learning_rate}')

    agent = Agent(input_space, action_space, learning_rate)

    return agent


def handle_inference_mode(conn, agent : Agent):
    #get obs
    print("waiting for observations")
    obs_num = agent.actor.inputs[0].shape[1]
    obs = receive_array(conn, obs_num)

    #run inference graph

    print("Running Inference Graph")
    start = time.time()
    up, right = agent.get_action(obs)
    end = time.time()

    print("Run infernece graph took %f ms", (end - start) * 1000)
    
    #send actions back

    size = struct.pack("i", 2) # 2 actions
    conn.send(size)

    conn.send(struct.pack('i', up))
    conn.send(struct.pack('i', right))


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
            agent = build_NN(conn)

            while True:
                wait_for_mode(conn)
                
                print("Mode Selected: ", mode)

                if mode == 0:
                    handle_inference_mode(conn, agent)
                elif mode == 1:
                    raise NotImplementedError()

