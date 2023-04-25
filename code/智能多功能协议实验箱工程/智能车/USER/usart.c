#include "usart.h"

u8 RX_BUF[RX_MAX];
u8 RX_OK = 0;

void Usart1_Init(u32 b)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);//��ʱ��
	
	//����GPIOA
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	
	
	
	//���ô���
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ͺͽ���
	USART_InitStructure.USART_Parity = USART_Parity_No;//��ʹ����ż����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8������λ
	
	USART_Init(USART1,&USART_InitStructure);//д��Ĵ���
	
	
	//����NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ��IRQͨ��
	NVIC_Init(&NVIC_InitStructure);//д��Ĵ���
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���������ж�
	
	USART_Cmd(USART1,ENABLE);//ʹ�ܴ���1
	

}
//��дprintf
int fputc(int ch,FILE *fp)
{
	USART1->DR = ch;
	while(!(USART1-> SR & (1 << 7)));
	return ch;
}


/*
	
��ʽ�淶��

һ֡���ݣ�

  λ��        1            2          3           4           5         6
���ܣ�    ��ʼλ��λ   ��ʼλ��λ   ���״̬    ���״̬    �ٶȸ�λ   �ٶȵ�λ


������⣺

��ʼλ��0xFAFC

���״̬��  00����ǰ   01����ת    10����ת    11��ֹͣ

���״̬��  00������   01������

�ٶȣ��������ֽ� 16λ�������洢
					

*/



//USART1�����жϺ���
void USART1_IRQHandler(void)
{
	u8 res;//��ʱ�������ֵ
//	static u8 RX_Count = 2;//BUF������ʼֵ
//	static u8 RX_Flag = OFF;//��ʼ���ձ�־λ
//	static u8 SartBit_H = NULL;//��ʼλ��λ������
	
	if(USART1->SR & (1 << 5))
	{
		USART1->SR &= ~(1 << 5);//����жϱ�־λ
		
		res = USART1->DR;//��ȡ����
		key = res;
//		if(RX_OK == OFF)//һ�ν���һ֡���� 
//		{
//			if((RX_Flag == ON) && (RX_Count < RX_MAX))//�������֡��ʽ��ȷ ��ʼ���� (�Ѵ˴����ǰ��Ŀ���Ƿ�ִֹ�в���Ҫ���ж�)
//				RX_BUF[RX_Count++] = res;//����ֵ��������
//			else if((res == (SartBit >> 8)) && (RX_Flag == OFF) && (SartBit_H == NULL))//�����һ�ֽ�����ʼλ��8λ 
//				SartBit_H = res;//�����һλ����ʼλ��λ ������뻺����
//			else if((res == (SartBit & 0x00FF)) && (RX_Flag == OFF) && (SartBit_H == (SartBit >> 8)))//����ڶ��ֽ�����ʼλ��8λ
//				{
//					RX_BUF[0] = SartBit_H;//��ʼλ��λ��λ����buf[0]
//					RX_BUF[1] = res;//��ʼλ��λ����buf[1]
//					SartBit_H = NULL;//��ʼλ��λ����������
//					RX_Flag = ON;//���ձ�־λ����
//				}
//				else //�������ʼλ����
//					SartBit_H = NULL;//��ʼ��λ���������� ��ֹ����
//			
//			if(RX_Count == RX_MAX)//��һ֡���ݽ������ ���б�־λ��λ
//				{
//					RX_Flag = OFF;
//					RX_OK = ON;//һ֡���ݽ������
//					RX_Count = 2;
//				}
		
		
//		}



		
	
	}

}	

