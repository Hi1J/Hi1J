#include "stm32f10x.h"
#include "delay.h"
#include "tim6.h"
#include "usart.h"
#include "string.h"

//TIM6Э�����ڽ��ղ��ȳ����� 

int main()
{
	//���ݲ�����115200 ��֪1s�ɴ���11520���ֽ� ��10ms�ɴ���115���ֽ� ��Ա����� ��ȫ����
	TIM6_Inter_Init(7200,200);//����20ms�ж�
	USART1_Config();//���ڳ�ʼ��
	printf("����1��ʼ���ɹ���\r\n");
	USART1_RX_STA=0;	//����1���ݽ��ձ�־λ����
	

	while(1)
	{
		if(USART1_RX_STA & 0x8000)//���յ�����
		{
			printf("���յ�������Ϊ(��ȥ���һ��)��\r\n%s\r\n",USART1_RX_BUF);
			memset(USART1_RX_BUF,0,USART1_REC_LEN);//��ս�����������
			USART1_RX_STA=0;	//����1���ݽ��ձ�־λ����
		
		}
		
	
	}
	
	
}