//см /usr/include/netinet

#ifndef __NETINET_IP_H
#define __NETINET_IP_H 1

#include <features.h>
#include <sys/types.h>

#include <netinet/in.h>

__BEGIN_DECLS

struct timestamp
  {
    u_int8_t len;
    u_int8_t ptr;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int flags:4;
    unsigned int overflow:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int overflow:4;
    unsigned int flags:4;
#else
# error "Please fix <bits/endian.h>"
#endif
    u_int32_t data[9];
  };

struct iphdr
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    u_char ip_header_len:4; //unsigned int ihl:4;
    u_char version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    u_char version:4;
    u_char ip_header_len:4;
#else
# error "Please fix <bits/endian.h>"
#endif
    u_int8_t tos; //type_of_service
    u_int16_t tot_len; //total length
    u_int16_t id; //identificator
    u_int16_t frag_off; //3 бита - flags, остальное - fragment offset
    u_int8_t ttl; //time to live
    u_int8_t protocol;
    u_int16_t check; //контрольная сумма
    
    char source_ip[4];// u_int32_t saddr; 
    char dest_ip[4];
    /*The options start here. */
  };
