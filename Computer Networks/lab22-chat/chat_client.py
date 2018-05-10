#!/usr/bin/python3
# -*- coding: utf-8 -*-

import socket, sys, select
from chat_server import get_message, send_message

IP = "localhost"
PORT = 8081

def start_client_app():

    out_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    out_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    out_socket.settimeout(1)#было 2
    try:
        out_socket.connect((IP, PORT))
    except:
        print("Unable to connect to %s:%d " % (IP, PORT))
        return
    print('Connection is OK\n[Me]: ', end='')
    try:
        while True:
            message = ''
            read_sockets, write_sockets, error_sockets = select.select([sys.stdin, out_socket], [],[], 0)
            for s in read_sockets:
                if s == out_socket:
                    message = get_message(s)
                    if message:
                        print("[recieved]", message.decode(), "\n[Me]:")
                    else:
                        print('Connection failed')
                        return
                else:
                    message = input().encode()
                    x = send_message(out_socket, message)
                    if x:
                        print("[Me]:")
                    else:
                        print('Connection failed. End of chat')
                        return
                    
    finally:                            
        out_socket.shutdown(socket.SHUT_RDWR)
        out_socket.close()
        print('Everything is closed')

if __name__ == "__main__":
    start_client_app()
    sys.exit(0)
