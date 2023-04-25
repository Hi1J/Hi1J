#ifndef _HAL_WIRELESS_H_
#define _HAL_WIRELESS_H_


#include "stm32f10x.h"
#include "stdio.h"
//#include "crc16.h"
#include "delay.h"

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


void UART4_Config(u32 boad);
void UART4_NVICConfig(void);
void UART_SendData(USART_TypeDef *puart,u8 *data,u16 len);
void Time4_Config(u16 psc,u16 arr);
#endif
