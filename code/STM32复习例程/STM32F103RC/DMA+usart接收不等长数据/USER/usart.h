#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"
#include "stdio.h"//��Ҫ�ض���printf����
#include "dma.h"

#define USE_USART_DMA_RX 1   //DMA��������  0���ر� 1������

#define Simulation_BaudRate 57600 //Proteus 8.9��������������������Ϊ57600����˸�ֵΪ����ʱʹ�� 
#define General_BaudRate 115200 //һ���ڿ�������ʹ�õĲ�����

#define USART1_REC_LEN  			400  	//����USART1�������ֽ���

extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	


//��������
int fputc(int ch,FILE *fp);

void USART_Config(void);


#endif