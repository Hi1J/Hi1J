#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "usart.h"

//USART�շ�
int main()
{
	systick_config();//ʱ�ӳ�ʼ��
	usart0_init(115200U);
	printf("����0��ʼ����ɣ�\r\n");
	usart1_init(115200U);
	USART1_Send_Data_buf("����1��ʼ����ɣ�\r\n",sizeof("����2��ʼ����ɣ�\r\n"));
	while(1)
	{
		
	}
}

