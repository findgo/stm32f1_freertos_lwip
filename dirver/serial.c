


#include "serial.h"
#include "freeRTOS.h"
#include "queue.h"

typedef struct {
	QueueHandle_t RX;
    QueueHandle_t TX;
}comcfg_t;

//private 
#if COM_USE_NUM >= 1
static comcfg_t comcfg0; // serial����ʵ��
#endif
#if COM_USE_NUM >= 2
static comcfg_t comcfg1; // serial����ʵ��
#endif
#if COM_USE_NUM >= 3
static comcfg_t comcfg2; // serial����ʵ��
#endif
#if COM_USE_NUM >= 4
static comcfg_t comcfg3; // serial����ʵ��
#endif

void ComInit(void)
{
#if COM_USE_NUM >= 1
	comcfg0.RX = xQueueCreate(COM0_RX_MAX_SIZE, sizeof(uint8_t));
	comcfg0.TX = xQueueCreate(COM0_TX_MAX_SIZE, sizeof(uint8_t));
#endif
#if COM_USE_NUM >= 2
	comcfg1.RX = xQueueCreate(COM1_RX_MAX_SIZE, sizeof(uint8_t));
	comcfg1.TX = xQueueCreate(COM1_TX_MAX_SIZE, sizeof(uint8_t));
#endif
#if COM_USE_NUM >= 3
	comcfg2.RX = xQueueCreate(COM2_RX_MAX_SIZE, sizeof(uint8_t));
	comcfg2.TX = xQueueCreate(COM2_TX_MAX_SIZE, sizeof(uint8_t));
#endif
#if COM_USE_NUM >= 4
	comcfg3.RX = xQueueCreate(COM3_RX_MAX_SIZE, sizeof(uint8_t));
	comcfg3.TX = xQueueCreate(COM3_TX_MAX_SIZE, sizeof(uint8_t));
#endif	
	
}

static comcfg_t *GetUseCom(uint8_t COM)
{
    switch(COM){
#if COM_USE_NUM >= 1
    case COM0:
        return &comcfg0;
#endif
#if COM_USE_NUM >= 2
    case COM1:
        return &comcfg1;
#endif
#if COM_USE_NUM >= 3
    case COM2:
        return &comcfg2;
#endif
#if COM_USE_NUM >= 4
    case COM3:
        return &comcfg3;
#endif
    default:
        return &comcfg0;
    }
}

//����һ�����Ϳ��ж�
static void Start_TXEtransmit(uint8_t COM,comcfg_t *cfg)
{
    //���÷�����ɿ��ж�
    if(uxQueueMessagesWaiting(cfg->TX)){//������
        switch(COM){
#if COM_USE_NUM >= 1
            case COM0:
                COM0TxIEEnable();
                break;
#endif
#if COM_USE_NUM >= 2
            case COM1:
                COM1TxIEEnable();
                break;
#endif
#if COM_USE_NUM >= 3
            case COM2:
                COM2TxIEEnable();
                break;
#endif
#if COM_USE_NUM >= 4
            case COM3:
                COM3TxIEEnable();
                break;
#endif
        }
    }
}
//����һ����������ж�
static void Start_TXCtransmit(uint8_t COM,comcfg_t *cfg)
{
    uint8_t temp;
        
    if(xQueueReceive(cfg->TX,&temp,0)){
         switch(COM){
#if COM_USE_NUM >= 1
            case COM0:
                COM0PutByte(temp);
                break;
#endif
#if COM_USE_NUM >= 2
            case COM1:
                COM1PutByte(temp);
                break;
#endif
#if COM_USE_NUM >= 3
            case COM2:
                COM2PutByte(temp);
                break;
#endif
#if COM_USE_NUM >= 4
            case COM3:
                COM3PutByte(temp);
                break;
#endif
        }
    }
   
}
/**
  * @brief  TX�������Ϊlen������
  * @param  buf:������ݻ�����,len:������ݳ���
  * @note   
  * @retval  number write
  */
uint16_t Serial_WriteBuf(uint8_t COM,uint8_t *buf,uint16_t len)
{
    uint16_t count = 0;
    comcfg_t *cfg;

    cfg = GetUseCom(COM);
	while(len--){
		count += xQueueSend(cfg->TX, buf, portMAX_DELAY);
		buf++;
	}
    //���÷�������ж� has some bug
//    Start_TXCtransmit(COM,cfg);

    //���÷�����ɿ��ж�
    Start_TXEtransmit(COM,cfg);
    
    return count;
}



uint16_t Serial_WriteByte(uint8_t COM,uint8_t dat)
{
	return Serial_WriteBuf(COM,&dat,1);
}

/**
  * @brief  �ӽ���ȡ��len������
  * @param  buf:�������ݻ�����,len:�������ݳ���
  * @note   
  * @retval  ����ȡ�����ݵ�ʵ�ʸ���
  */
uint16_t Serial_Read(uint8_t COM,uint8_t *buf, uint16_t len)
{
    uint16_t length = 0;
    comcfg_t *cfg;

    cfg = GetUseCom(COM);
	while(len--)
	{
    	length += xQueueReceive(cfg->RX,buf, len);
		buf++;
	}
    return length;
}

/***********only for Isr callback***********/
/**
  * @brief  ���Ϳ��жϵ��ú���
  * @param  None
  * @note   
  * @note    
  * @note   
  * @retval  None
  */
void COM0_TXE_Isr_callback(void)
{
    uint8_t temp;

    if(xQueueReceiveFromISR(comcfg0.TX,&temp,NULL)){
        COM0PutByte(temp);
    }else{
        COM0TxIEDisable();
    }
}
/**
  * @brief  ��������жϻص�����
  * @param  None
  * @note   
  * @retval  None
  */
void COM0_TXC_Isr_callback(void)
{


}
/**
  * @brief  �����жϻص�����
  * @param  None
  * @note   
  * @retval  None
  */
void COM0_RX_Isr_callback(void)
{
    uint8_t temp = COM0GetByte();

	xQueueSendFromISR(comcfg0.RX, &temp, NULL);
}

