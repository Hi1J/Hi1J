#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"


//DMA+���ڽ��ղ���������
int main()
{
	USART_Config();//���ڳ�ʼ��
	printf("����1��ʼ����ɣ�\r\n");
	
	while(1)
	{
		if(USART1_RX_STA & 0xC000)//�жϽ����Ƿ����
		{
			printf("���յ�������Ϊ��\r\n%s\r\n",USART1_RX_BUF);
			memset(USART1_RX_BUF,0,USART1_REC_LEN);//��ս�����������
			USART1_RX_STA = 0;//���ձ�־λ����
		}
		
	
	}
	
	
}