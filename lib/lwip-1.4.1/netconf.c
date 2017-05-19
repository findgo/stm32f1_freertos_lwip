/* Includes ------------------------------------------------------------------*/
#include "lwip/memp.h"
#include "lwip/init.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/inet.h"
#include "lwip/timers.h"
#include "ethernetif.h"

#include "stm32f10x.h"
#include "arch/sys_arch.h"
#include <stdio.h>
#include "netconf.h"
#include "bsp_spi1.h"

#define MAX_DHCP_TRIES	4


/* Private macro -------------------------------------------------------------*/
static struct netif netif;
static uint32_t IPaddress = 0;



static void TcpipInitDone(void *arg);
static void net_pkt_intConfig(void);


/** * @brief Init_LwIP initialize the LwIP */
void netconf_init(void) 
{  
    struct ip_addr ipaddr;  
    struct ip_addr netmask;  
    struct ip_addr gw; 
    //sys_sem_t sem;
    
    BSP_ConfigSPI1();
    // 调用LWIP初始化函数  
#if NO_SYS
    lwip_init();  
#else


    //sys_sem_new(&sem, 0);
    tcpip_init(NULL, NULL);
    //sys_sem_wait(&sem);
    //sys_sem_free(&sem);
#endif

#if LWIP_DHCP /* 启用DHCP服务器 */
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
#else
    /* 启用静态IP */
    IP4_ADDR(&ipaddr, 192, 168, 1, 116); // 设置网络接口的ip地址  
    IP4_ADDR(&netmask, 255, 255, 255, 0);    // 子网掩码  
    IP4_ADDR(&gw, 192, 168, 1, 1);   // 网关  
//    inet_aton("192.168.1.116",&ipaddr);
//    inet_aton("255.255.255.0",&netmask);
//    inet_aton("192.168.1.1",&netmask);

#endif
    //  Set_MAC_Address(macaddress); //设置MAC地址
 
    // 初始化netcard 与LWIP的接口，参数为网络接口结构体、ip地址、  
    // 子网掩码、网关、网卡信息指针、初始化函数、输入函数  
#if !NO_SYS
    netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input); 
#else
    netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);  
#endif 
    // 设置默认网卡  
    netif_set_default(&netif);  

#if LWIP_DHCP
    /*  Creates a new DHCP client for this interface on the first call.
    Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
    the predefined regular intervals after starting the client.
    You can peek in the netif->dhcp struct for the actual DHCP status.*/
    dhcp_start(&netif);
#endif
    netif_set_up(&netif);

    net_pkt_intConfig();
}
/**   
* @brief TcpipInitDone wait for tcpip init being done  
*   
* @param arg the semaphore to be signaled  
*/
static void TcpipInitDone(void *arg)
{
#if !NO_SYS
	sys_sem_t *sem;
	sem = arg;
	sys_sem_signal(sem); 
#endif
}  


#if NO_SYS
void net_Periodic_Handle(void)
{
    // 轮询接收数据，要采用中断
    //net_pkt_handle();
    /** Handle timeouts for NO_SYS==1 (i.e. without using
     * tcpip_thread/sys_timeouts_mbox_fetch(). Uses sys_now() to call timeout
     * handler functions when timeouts expire.
     *
     * Must be called periodically from your main loop.
     */
    sys_check_timeouts();
}
#endif 

// 中断接口调用，处理数据包
/** * @brief  Ethernet ISR */
void net_pkt_handle(void)
{
  /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
    while(ETH_packet_getcount() != 0) 
    {     
        ethernetif_input(&netif);
    }
}

/* @brief Display_IPAddress Display IP Address  */
void Display_IPAddress(void) 
{     
	if(IPaddress != netif.ip_addr.addr){
		/* IP 地址发生改变*/
		__IO uint8_t iptab[4];
//		uint8_t iptxt[20]; 
		 /* read the new IP address */
		 IPaddress = netif.ip_addr.addr;
		 iptab[0] = (uint8_t)(IPaddress >> 24);
		 iptab[1] = (uint8_t)(IPaddress >> 16);
		 iptab[2] = (uint8_t)(IPaddress >> 8);
		 iptab[3] = (uint8_t)(IPaddress);
	} 
#if LWIP_DHCP
	else if(IPaddress == 0){  
		// 等待DHCP分配IP                  
		/* If no response from a DHCP server for MAX_DHCP_TRIES times */         
		/* stop the dhcp client and set a static IP address */         
		if(netif.dhcp->tries > MAX_DHCP_TRIES){
			/* 超出DHCP重试次数，改用静态IP设置 */
			struct ip_addr ipaddr;
			struct ip_addr netmask;
			struct ip_addr gw;
			dhcp_stop(&netif);
			IP4_ADDR(&ipaddr, 10, 21, 11, 245);
			IP4_ADDR(&netmask, 255, 255, 255, 0);
			IP4_ADDR(&gw, 10, 21, 11, 254);
			netif_set_addr(&netif, &ipaddr , &netmask, &gw); 
		}
	} 
#endif 
}  


static void net_pkt_intConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    NET_PKT_POART_CLOCK_EN();
    GPIO_InitStructure.GPIO_Pin   = NET_PKT_INT_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(NET_PKT_INT_PORT, &GPIO_InitStructure);

    // 配置net数据来时的中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =  2; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                
    NVIC_Init(&NVIC_InitStructure);
    
    // 配置外部中断线源
    GPIO_EXTILineConfig(NET_PKT_INT_PORTSource, NET_PKT_INT_PINSource);
    EXTI_InitStructure.EXTI_Line    = NET_PKT_INT_EXTI_LINE;             
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;       // 中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure); 
}


void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0)){
        net_pkt_handle();

        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    
}


