#!/usr/bin/python3
# -*- coding: utf-8 -*-

import socket, sys, select

PACKAGE_LEN = 256
BITS = 8
MASK = PACKAGE_LEN - 1
HEADER_LEN = 2

HOST = '' #По умолчанию слушаем все хосты
PORT = 8081 #Случайное незарезервированное число

SOCKET_LIST = []

#op == 0 - флаг ошибки
SEND_MESSAGE = 1
CHANGE_NICK = 2
ALL_USERS_LIST_REQUEST = 3

#Удобно формировать список тех, кто онлайн или искать, зарегистрирован ли ник
#если в качестве ключа - имя, а в качестве значения - сокет
#2 словаря в первом пары сокет:имя, во втором наоборот
SOCKET_NAME = {}
NAME_SOCKET = {}

def name_of(client):
    #return USERS[client].decode() #Последнее имя из списка
    return "%s:%d" % client.getpeername()

from functools import reduce
def form_list_online():
    res = b'>> Users online:\n>>   '
    res = reduce(lambda string, name: string + name + b", ", NAME_SOCKET.keys(), res)
    return res[:-2]

def get_message(src):
    try:
        head = src.recv(PACKAGE_LEN); #Первое сообщение, содержащее заголовок, в котором вся информация
        num_of_packages = int(head[0]) - 1 #Одну часть сообщения уже приняли
        op = int(head[1])
        message = head[2:]
        while num_of_packages: #Дочитываем сообщение
            message += src.recv(PACKAGE_LEN)
            num_of_packages -= 1
        #print(message.decode())
        return op, message
    except:
        print("Unable to get the message")
        return 0, b''

#[0] - количество пакетов, на которое будет разбито сообщение
#[1] - код запроса (по умолчанию - отправка сообщения в чат)
#[2:] - остальное - отправляемое сообщение
def send_message(dest, bin_data = b'', op = SEND_MESSAGE):
    mess_len = len(bin_data) + HEADER_LEN
    num_of_packages = (mess_len >> BITS) + (bool(mess_len & MASK))
    try:
        x = dest.send(bytes([num_of_packages]) + bytes([op]) + bin_data)
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


def register_new_client():
    #new_connectionconn - сокет для обмена с клиентом
    #addr - массив, [0]-ip (строка), [1]-хост (число)
    new_connection, addr = server_socket.accept()
    SOCKET_LIST.append(new_connection)
    print("New connection from %s:%s" % addr)
    _, new_name = get_message(new_connection)
    if new_name in NAME_SOCKET: #Имя уже зарегистрировано - отказать
        send_message(s, form_list_online(), CHANGE_NICK)
    SOCKET_NAME[new_connection]= new_name #Добавить новое имя клиента
    NAME_SOCKET[new_name] = new_connection
    broadcast(server_socket, new_connection, (">> %s joined us\n" % name_of(new_connection)).encode())
    
def change_nick(sock, nick):
    if message in NAME_SOCKET: #Имя уже зарегистрировано - отказать
        send_message(s, form_list_online(), CHANGE_NICK)
        return 0
    old_name = SOCKET_NAME[s]
    del NAME_SOCKET[old_name]
    SOCKET_NAME[s] = message
    NAME_SOCKET[message] = s
    print('User ' + old_name.decode() + ' changed name to ' + SOCKET_NAME[s].encode() + '.')
    return 1

def disconnect(sock):
    print("Disconnection of", name_of(sock))
    broadcast(server_socket, sock, (">> Client %s is offline\n" % name_of(sock)).encode())
    SOCKET_LIST.remove(sock)
    name = SOCKET_NAME.pop(sock)
    NAME_SOCKET.pop(name)
    sock.close()
    
                
def run_server():
    global server_socket
    server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind( (HOST, PORT) )
    server_socket.listen(5)              #аргумент- макс кол-во подключений
    SOCKET_LIST.append(server_socket) #Наш сокет тоже нужно слушать на предмет подключений
##    USERS[server_socket] = list()
##    USERS[server_socket].append(b"server")
    try:
        while True: # работаем постоянно
            # 4-й аргумент, time_out  = 0 (poll and never block)
            ready_to_read,ready_to_write,in_error = select.select(SOCKET_LIST,[],[],0)
                    
            for s in ready_to_read:
                #Событие случилось на серверном сокете - новое подключение
                if s == server_socket:
                    register_new_client()
                    
                #Событие на сокете клиента - принимаем сообщение и рассылаем
                else:
                    op, message = get_message(s)
                    if op == SEND_MESSAGE: #Получили данные
                        print("Recieved \'", message.decode(), "\' from", name_of(s))
                        broadcast(server_socket, s, message)
                    elif op == CHANGE_NICK:
                        if change_nick(s, message):
                            broadcast(server_socket, s, b'>> User ' + old_name + b' changed name to ' + SOCKET_NAME[s] + b'.')
                            send_message(s, b'1', CHANGE_NICK)
                    elif op == ALL_USERS_LIST_REQUEST:
                        print('User', name_of(s), 'wants to know about all online')
                        send_message(s, form_list_online())
                   # elif op == REQUEST_NICK_HISTORY:
                        
                    else: #Получили сообщение о shutdown либо ошибку
                        disconnect(s)
    finally:
        for s in SOCKET_LIST:
            s.shutdown(socket.SHUT_RDWR)
            s.close()
        print("Everyting is closed")


if __name__ == "__main__":
    run_server()
    sys.exit(0)
