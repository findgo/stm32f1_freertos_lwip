#ifndef __NETCONF_H_
#define __NETCONF_H_

/*see api doc: http://www.nongnu.org/lwip/2_0_x/index.html*/

// 定义netcard 数据中断线
#define NET_PKT_INT_PORT    GPIOA
#define NET_PKT_INT_PIN     GPIO_Pin_0
#define NET_PKT_POART_CLOCK_EN() RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);

#define NET_PKT_INT_PORTSource  GPIO_PortSourceGPIOA
#define NET_PKT_INT_PINSource   GPIO_PinSource0
#define NET_PKT_INT_EXTI_LINE   EXTI_Line0


void netconf_init(void);
void net_Periodic_Handle(void); 
void Display_IPAddress(void);


#endif
