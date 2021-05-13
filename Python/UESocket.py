import socket

HOST = 'localhost'      # Standard loopback interface address (localhost)
PORT = 8080            # Port to listen on (non-privileged ports are > 1023)

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
            while True:
                data = conn.recv(1024)
                print(f"received data: {data}")
                if not data:
                    break

                if data == b'SAY HELLO MY FRIEND\n':
                    
                    r = 'Hello friend.'
                    conn.sendall(r.encode())
                    continue
                
                #todo read observation/rewards data.
