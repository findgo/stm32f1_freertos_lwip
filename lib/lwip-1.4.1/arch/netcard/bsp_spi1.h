/*
********************************************************************************
*                                   
* 模块名称 : SPI1驱动模块    
* 文件名称 : bsp_spi1.h
* 版    本 : V0.1
* 说    明 : 配置SPI1相关参数
* 修改记录 :
*   版本号  日期        作者      说明
*   V0.1    2012-11-26  徐凯      创建该文件
*
*
********************************************************************************
*/
#ifndef __BSP_SPI1_H
#define __BSP_SPI1_H

/* 引用头文件 *****************************************************************/
#include "stm32f10x.h"
/* 外部数据类型 ***************************************************************/
/*
 * RST ---- PA3
 * INT ---- PA1
 * NSS ---- PA4
 * SCK ---- PA5
 * SI  ---- PA7
 * SO  ---- PA6
 */
/*定义硬件IO*/
#define HAL_SPI_SCK_PORT 		GPIOA
#define HAL_SPI_SCK_PIN	 		GPIO_Pin_5
#define HAL_SPI_MISO_PORT 		GPIOA
#define HAL_SPI_MISO_PIN	 	GPIO_Pin_6
#define HAL_SPI_MOSI_PORT 		GPIOA
#define HAL_SPI_MOSI_PIN	 	GPIO_Pin_7
#define HAL_SPI_CS_PORT 		GPIOA
#define HAL_SPI_CS_PIN			GPIO_Pin_4
#define HAL_SPI_GPIO_CLOCK_EN()	do{RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE );}while(0)


#define HAL_SPI_ACTIVE SPI1
#define HAL_SPI_ACTIVE_CLOCK_EN()	do{RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 ,ENABLE );}while(0)

/* 外部常数宏 *****************************************************************/
/* 外部动作宏 *****************************************************************/
/* 外部变量 *******************************************************************/
/* 外部函数声明 ***************************************************************/
void BSP_ConfigSPI1(void);
uint8_t BSP_SPI1SendByte(uint8_t byte);
uint8_t BSP_SPI1ReceiveByte(void);

#endif
/***************************************************************END OF FILE****/
