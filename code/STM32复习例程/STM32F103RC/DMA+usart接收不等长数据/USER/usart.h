#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"
#include "stdio.h"//需要重定向printf函数
#include "dma.h"

#define USE_USART_DMA_RX 1   //DMA接收数据  0：关闭 1：开启

#define Simulation_BaudRate 57600 //Proteus 8.9仿真软件里仿真的最大波特率为57600，因此该值为仿真时使用 
#define General_BaudRate 115200 //一般在开发板上使用的波特率

#define USART1_REC_LEN  			400  	//定义USART1最大接收字节数

extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	


//函数声明
int fputc(int ch,FILE *fp);

void USART_Config(void);


#endif