#include "usart.h"

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

//USART1�����жϺ���
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART1->SR & (1 << 5))
	{
		res = USART1->DR;//��ȡ����
		printf("%c",res);//��������
		
		USART1->SR &= ~(1 << 5);//����жϱ�־λ
	
	}

}	

