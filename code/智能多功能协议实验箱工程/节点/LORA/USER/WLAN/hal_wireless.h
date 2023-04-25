#ifndef _HAL_WIRELESS_H_
#define _HAL_WIRELESS_H_


#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"

#define USART_DMA_COM  USART3

/**********************�ײ��жϽ�������**********************/
//��������һ֡���������պͷ������ݸ���
#define WIRELESS_RX_MAX 64
#define WIRELESS_TX_MAX 64
typedef struct
{
	uint8_t  rx_buff[WIRELESS_RX_MAX];//�������ݽ��ջ�����
	uint8_t  tx_buff[WIRELESS_TX_MAX];//�������ݷ��ͻ�����
	uint8_t  rx_over;    //������ɱ�־0 -- û�н������ 1 -- �������
	uint32_t rx_count;                 //�������ݵĸ���
	uint32_t tx_count;                 //�������ݵĸ���
}WIRELESS_RX_TX;
extern WIRELESS_RX_TX wireless_data;

//��ز��������루��4λʹ�� -- ���16λ������ʹ�ã�
//WDC--Wireless Data Communication ��������ͨ��
#define WDC_CMD_    0x01//������Ĵ��� -- ���д���������ֵ�Ϳ���ģ���״̬
#define WDC_CMD_WriteReg     0x02//д����Ĵ��� -- �ɹ�����ģ��Ķ���

#define BROADCAST_ADDR       0x00//�㲥��ַ
extern  uint8_t Slave_ID;        //�ӻ������ַ


void USART3_Config(u32 boad);
void USART3_NVICConfig(void);
void UART_SendData(USART_TypeDef *puart,u8 *data,u16 len);
void Time4_Config(u16 psc,u16 arr);
void USART3_TxDMA_Config(void);
u8 USART3_DMA_SentData(char *p,uint32_t lenth);
uint8_t Slave_GetID(void);
#endif
