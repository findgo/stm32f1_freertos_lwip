

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "netconf.h"

static void prvSetupHardware( void );
void vLEDTask( void *pvParameters ) ;


int main(void)
{
	// hardware init
	 prvSetupHardware(); 
	 netconf_init();

	//create tast
	 xTaskCreate( vLEDTask, ( signed portCHAR * ) "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );
	/* Æô¶¯OS */  
	vTaskStartScheduler();

	// never go here
	return 0;
}

/*-----------------------------------------------------------*/  
static void prvSetupHardware( void )  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );  
	/*LED0 @ GPIOB.0*/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init( GPIOB, &GPIO_InitStructure );  
}  


void vLEDTask( void *pvParameters )  
{  
	while(1) 
	{  
		GPIO_SetBits(GPIOB, GPIO_Pin_8);  
		vTaskDelay( 500/portTICK_RATE_MS );  
		GPIO_ResetBits(GPIOB, GPIO_Pin_8); 
		vTaskDelay( 500/portTICK_RATE_MS );  
	}  
}  

