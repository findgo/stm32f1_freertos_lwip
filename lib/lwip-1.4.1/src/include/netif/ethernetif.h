#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"
#include "enc28j60.h"


// 以下宏是netcard接口，避免换网卡时产生大量替换工作，抽像网卡层
#define ETH_init(pmac_addr)     enc28j60_init(pmac_addr)
#define ETH_readbuf(pbuf,len)   enc28j60_readbuf(pbuf,len)
#define ETH_writebuf(pbuf,len) 	enc28j60_writebuf(pbuf,len)
#define ETH_init_send(len)       enc28j60_init_send(len)
#define ETH_start_send()         enc28j60_start_send()
#define ETH_GetRxPktSize()   enc28j60_packet_getcount()
#define ETH_packet_getlen()     enc28j60_packet_getlen()
#define ETH_finish_receive()    enc28j60_finish_receive()


err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void Set_MAC_Address(unsigned char* macadd);

#endif 

