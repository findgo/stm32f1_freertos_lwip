/*
 * This file is part of the lwIP TCP/IP stack.
 *
 * 
 *
 */
#ifndef __SYS_RTXC_H__
#define __SYS_RTXC_H__

#if !NO_SYS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define SYS_MBOX_NULL (xQueueHandle)0
#define SYS_SEM_NULL  (xSemaphoreHandle)0
#define SYS_DEFAULT_THREAD_STACK_DEPTH	configMINIMAL_STACK_SIZE

typedef xSemaphoreHandle sys_sem_t;
typedef xQueueHandle sys_mbox_t;
typedef xTaskHandle sys_thread_t;
//typedef int sys_prot_t;  //这个在cc.h已经定义过了

/*
typedef struct _sys_arch_state_t
{
	// Task creation data.
	char cTaskName[configMAX_TASK_NAME_LEN];
	unsigned short nStackDepth;
	unsigned short nTaskCount;
} sys_arch_state_t;
*/


//extern sys_arch_state_t s_sys_arch_state;

//void sys_set_default_state();
//void sys_set_state(signed char *pTaskName, unsigned short nStackSize);


/* Message queue constants. */
//#define archMESG_QUEUE_LENGTH	( 6 )

#else

u32_t sys_now(void);

#endif
#endif /* __SYS_RTXC_H__ */


