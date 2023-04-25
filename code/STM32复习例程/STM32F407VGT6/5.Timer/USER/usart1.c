#include "usart1.h"


void Usart1_Init(uint32_t Baud,uint32_t fclk,uint8_t OVER8)
{
	float USARTDIV;//����д��BRR�Ĵ�������
	uint32_t DIV_Mantissa;//����д��BRR�Ĵ������ݵ���������
	uint32_t DIV_Fraction;//����д��BRR�Ĵ������ݵ�С������
	uint32_t Priority;//�ж����ȼ�
//��GPIOAʱ��
	RCC->AHB1ENR |= 1 << 0;
	
//PA9����(RX)
	GPIOA->MODER &= ~(3 << (2 * 9));
	GPIOA->MODER |= 2 << (2 * 9);
	
	//����ΪUSART1����
	GPIOA->AFR[1] &= ~(15 << ( 4 * (9 - 8)));//����
	GPIOA->AFR[1] |= 7 << ( 4 * (9 - 8));//д��0111
	
//PA10����(TX)
	GPIOA->MODER &= ~(3 << (2 * 10));
	GPIOA->MODER |= 2 << (2 * 10);
	
	//����ΪUSART1����
	GPIOA->AFR[1] &= ~(15 << ( 4 * (10 - 8)));//����
	GPIOA->AFR[1] |= 7 << ( 4 * (10 - 8));//д��0111	
	
	
//��USART1ʱ��
	RCC->APB2ENR |= 1 << 4;
	
//���ô��ڹ���ģʽCR1
	//�ֳ� 0��1λ��ʼλ 8λ����λ nλֹͣλ  �ֳ� 0��1λ��ʼλ 9λ����λ nλֹͣλ
	USART1->CR1 &= ~(1 << 12);
	
	//д��ֹͣλ���� 00��1λֹͣλ
	USART1->CR2 &= ~(3 << 12);
	
	//ʹ�ܷ���
	USART1->CR1 |= 1 << 3;
	
	//ʹ�ܽ���
	USART1->CR1 |= 1 << 2;
	
//���ò�����
	//������ģʽ������ 0��16�������� 1��8��������
	if(OVER8 == 0)
	{
		USART1->CR1 &= ~(1 << 15);//16��������ģʽ
	}
	else
	{
		USART1->CR1 |= 1 << 15;  //8�������ģʽ
		
	}
	
	
	USARTDIV = fclk * 1000000 /(Baud * 8 * (2 - OVER8));//д��BRR�Ĵ������ݵļ���
	
	DIV_Mantissa = USARTDIV;//��������
	DIV_Fraction = (USARTDIV - DIV_Mantissa) * 16; // С����     * 16 ��Ҳ��������4λ
	
	USART1->BRR = DIV_Mantissa << 4 | DIV_Fraction;//д��BRR�Ĵ��� �൱����������4λ��+С��
	
//�жϳ�ʼ��
	//ʹ��usart1�Ľ����ж�
	USART1->CR1 |= 1 << 5;
	
	NVIC_SetPriorityGrouping(7-2);
	Priority = NVIC_EncodePriority(7-2,1,2);
	NVIC_SetPriority(USART1_IRQn, Priority);
	NVIC_EnableIRQ(USART1_IRQn);
	
	
//ʹ��USART1
	USART1->CR1 |= 1 << 13;
	

}

//�����ַ�������

void Usart1_Send_String(uint8_t * str)
{
	while( *str != '\0')
	{
		USART1->DR = *str;
		while(!(USART1->SR & (1 << 7)));//�ȴ��������
		str++;
	}

}

//��дprintf����

int fputc(int ch,FILE *p)
{
	USART1->DR = ch;
	while(!(USART1->SR & (1 << 7 )));
	return ch;


}

//USART1�жϷ�����
u8 Rec;
void USART1_IRQHandler(void)
{
	if(USART1->SR & (1 << 5))
	{
		Rec = USART1->DR;
		USART1->SR &= ~(1 << 5); //��ձ�־λ
	
	}


}
