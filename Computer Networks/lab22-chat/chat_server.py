#!/usr/bin/python3
# -*- coding: utf-8 -*-

import socket, sys, select

PACKAGE_LEN = 256
BITS = 8
MASK = PACKAGE_LEN - 1
HEADER_LEN = 1

HOST = '' #По умолчанию слушаем все хосты
PORT = 8081 #Случайное незарезервированное число

SOCKET_LIST = []

def name_of(client):
    return "%s:%d" % client.getsockname()

def get_message(src):
    try:
        head = src.recv(PACKAGE_LEN);
        num_of_packages = int(head[0]) - 1 #Одну часть сообщения уже приняли
        message = head[1:]
        while num_of_packages:
            message += src.recv(PACKAGE_LEN)
            num_of_packages -= 1
        #print(message.decode())
        return message
    except:
        print("Unable to get the message from", name_of(src))

def send_message(dest, bin_data):
    mess_len = len(bin_data)
    num_of_packages = (mess_len >> BITS) + (bool(mess_len & MASK))
    try:
        x = dest.send(bytes([num_of_packages]) + bin_data)
        #print('{%d}'% x)
        return x
    except:
        print("Can't send to %s the message '" % name_of(dest), bin_data.decode(), "'")
        return 0

def broadcast(server, src, bin_data):
    for s in SOCKET_LIST:
        if s != server and s != src:
            if not send_message(s, bin_data):
                print("Unable to send the message to", name_of(src))
                disconnect(s)

def disconnect(sock):
    print("Disconnection of", sock.getsockname())
    broadcast(server_socket, sock, ("Client [%s:%d] is offline\n" % sock.getpeername()).encode())
    SOCKET_LIST.remove(sock)
    sock.close()

def register_new_client():
    #new_connectionconn - сокет для обмена с клиентом
    #addr - массив, [0]-ip (строка), [1]-хост (число)
    new_connection, addr = server_socket.accept()
    SOCKET_LIST.append(new_connection)
    print("New connection from %s:%d" % new_connection.getsockname())
##    new_connection.send('Hello'.encode())
##    new_connection.send(b'')
    broadcast(server_socket, new_connection, ("[%s:%s] joined us\n" % addr).encode())
    
                
def run_server():
    global server_socket
    server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind( (HOST, PORT) )
    server_socket.listen(5)              #аргумент- макс кол-во подключений
    SOCKET_LIST.append(server_socket) #Наш сокет тоже нужно слушать на предмет подключений

    while True: # работаем постоянно
        # 4-й аргумент, time_out  = 0 (poll and never block)
        ready_to_read,ready_to_write,in_error = select.select(SOCKET_LIST,[],[],0)

        for s in ready_to_read:
            #Событие случилось на серверном сокете - новое подключение
            if s == server_socket:
                register_new_client()
                
            #Событие на сокете клиента - принимаем сообщение и рассылаем
            else:
                message = get_message(s)
                if message: #Получили данные
                    print("Recieved \'", message.decode(), "\' from [%s:%s]" % s.getsockname())
                    broadcast(server_socket, s, message)
                else: #Получили сообщение о shutdown либо ошибку
                    disconnect(s)
##    try:
##        while True: # работаем постоянно
##            # 4-й аргумент, time_out  = 0 (poll and never block)
##            ready_to_read,ready_to_write,in_error = select.select(SOCKET_LIST,[],[],0)
##                    #conn - сокет для обмена с клиентом
##                    #, addr - массив, [0]-ip, [1]-хост
##            for s in ready_to_read:
##                #Событие случилось на серверном сокете - новое подключение
##                if s == server_socket:
##                    register_new_client()
##                    
##                #Событие на сокете клиента - принимаем сообщение и рассылаем
##                else:
##                    message = get_message(s)
##                    if message: #Получили данные
##                        print("Recieved \'", message.decode(), "\' from [%s:%s]" % s.getsockname())
##                        broadcast(server_socket, s, message)
##                    else: #Получили сообщение о shutdown либо ошибку
##                        disconnect(s)
##    except:
##        print("!!Everything is broken!!")
##    finally:
##        for s in SOCKET_LIST:
##            s.shutdown(socket.SHUT_RDWR)
##            s.close()
##        print("Everyting is closed")

if __name__ == "__main__":
    run_server()
    for s in SOCKET_LIST:
        s.shutdown(socket.SHUT_RDWR)
        s.close()
    print("Everyting is closed")
    sys.exit(0)
