#include "stm32f4xx.h"
#include "led.h"
#include "key.h"

//GPIO->ODR ȡ�������� GPIOx->ODR = ~ (GPIOx->ODR & (1 << Pin));     GPIOx->ODR ^= 1 << Pin

//��һ����������static ����һ��������ʱ��  ������ִ����� ������ֵ�ᱣ��
void delay(uint32_t t)
{
	while(t--);
	
}


int main(void)
{
	
//	uint8_t buf[4]={4,5,6,7};
	uint8_t flag = 0;   //�������±�־λ 0��û�а��� 1������
	uint8_t i;
	LED_Init();
	KEY_Init();
	

	
	while(1)
		{

	//		for(i=0;i<4;i++){
	//			GPIOC->ODR &= ~(1<<buf[i]);
	//			delay(500000);
	//			GPIOC->ODR |= 1<<buf[i];
	//			delay(500000);
			
			if(!(GPIOE->IDR & (1<<2)) && flag == 1)
			{
				delay(150000);
				if(~(GPIOE->IDR) & (1<<2))
					{
						GPIOC->ODR ^= 1<<4;
						flag=0;
					}
			
			}
			else if(GPIOE->IDR & (1<<2))flag=1;//Ϊ��һ�ΰ�����׼��
			
			
			
			
			
			if(~(GPIOE->IDR) & (1<<3))
			{
				delay(200000);
				if(~(GPIOE->IDR) & (1<<3))
					{
						GPIOC->ODR &=~(1<<5);
						while(~(GPIOE->IDR) & (1<<3));
						GPIOC->ODR |=1<<5;
					
					
					}
			
			}
		
		}
		
		
}

