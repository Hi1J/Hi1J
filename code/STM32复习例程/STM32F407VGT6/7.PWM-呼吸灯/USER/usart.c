#include "usart.h"

/*
�������ܣ�usart��ʼ��
����ֵ����
�βΣ�flck , bood, over8
��ע��PA9 -- RX   PA10 -- TX
*/
void USART_Init(uint32_t fclk, uint32_t bood, uint8_t over8)
{
	float USARTDIV;    
	uint32_t  DIV_Mantissa;   //����
  uint32_t  DIV_Fraction;   //С��
	  /* GPIOʱ��ʹ�� */
	 RCC->AHB1ENR |= 1 << 0;    //GPIOAsʱ��ʹ��
	
	  /* PA9 -- RX*/
	 GPIOA->MODER &= ~(3 << (2 * 9));
	 GPIOA->MODER |= 2 << (2 * 9);  //����ģʽ
	 
	 GPIOA->AFR[1] &= ~(15 << (4 * (9 - 8)));
	 GPIOA->AFR[1] |= 7 << (4 * (9-8));  //����Ϊusart1
	 
	 /* PA10 -- TX*/
	 GPIOA->MODER &= ~(3 << (2 * 10));
	 GPIOA->MODER |= 2 << (2 * 10);  //����ģʽ
	 
	 GPIOA->AFR[1] &= ~(15 << (4 * (10-8)));
	 GPIOA->AFR[1] |= 7 << (4 * (10-8));  //����Ϊusart1
	 
	 /* ʹ�ܴ���ʱ�� */
	RCC->APB2ENR |= 1 << 4;   
	
	/* ���ô��ڹ�����ʽ */
	USART1->CR1 &= ~(1 << 12);   //����λ8λ
	USART1->CR2 &= ~(3 << 12);   //1λֹͣλ
	USART1->CR1 |= 1 << 3;       //����ʹ��
	USART1->CR1 |= 1 << 2;       //����ʹ��
	
	/* ���ò����� */
	if(over8 == 0)
	{
		USART1->CR1 &= ~(1 << 15);   //16��������
	}
	else
	{
		USART1->CR1 |= 1 << 15;     //8��������
	}
	
	 USARTDIV = fclk * 1000000 / (8 * (2 - over8) * bood);
	 DIV_Mantissa = USARTDIV;   //����
	 DIV_Fraction = (USARTDIV - DIV_Mantissa) * 16; 
	 
	 USART1->BRR = DIV_Mantissa << 4 | DIV_Fraction;
	
	 /* ʹ��USART*/
	USART1->CR1 |= 1 << 13;  
}



