#include "stm32f4xx.h"
#include "led.h"
#include "usart1.h"

void delay(uint32_t t)
{
	while(t--);
	
}


int main(void)
{
	
	uint8_t rec;//���ڴ�Ž��յ�����
//	LED_Init();
	Usart1_Init(115200,84,0);//������ 115200 AHB2Ƶ�� 84MHz ������ģʽ 16��������

	
	while(1)
	{
		while(!(USART1->SR & (1 << 5)));//�ȴ��������
		rec = USART1->DR;
		USART1->DR = rec;
		while(!(USART1->SR & (1 << 7)));//�ȴ��������
		

//�ж�		
//		if(Rec)
//		{
//			
//			USART1->DR = Rec;
//			while(!(USART1->SR & (1 << 7)));//�ȴ��������
//			Rec = 0;
//		}
//		
		
			
	
	}
}

