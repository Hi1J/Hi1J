#include "stm32f4xx.h"
#include "led.h"

#include "systick.h"

int main(void)
{
	

	LED_Init();
	
	delay_Inter_ms(500);//���ܳ���ʵ��ʲô���� �����ȫ��ÿ��500ms��һ���жϷ���������  ��Ȼ ǰ��������ʹ��SysTick


	
	while(1)
	{
//		GPIOC->ODR &= ~(1 << 4);
//		delay_ms(500);
//		GPIOC->ODR |= 1<< 4;
//		delay_ms(500);


		
	
	}
}

