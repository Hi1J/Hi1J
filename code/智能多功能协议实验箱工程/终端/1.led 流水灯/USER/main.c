#include "stm32f10x.h"
#include "delay.h"
#include "led.h"

//gpio��ˮ��
int main(void)
{
	u8 k = 0;
	LED_Init();
	
	
	while(1)
	{
//		if(k == 7)
//		{
//			LED1_7_Port->ODR = ~(0 << (k - 1));//��7�ƹر�
//			k = 0;
//			LED8_Port->ODR = ~(1 << 13);//��8�ƿ���
//			delay_ms(300);
//			LED8_Port->ODR = ~(0 << 13);//��8�ƹر�
//		}
//		
//		if(k == 7)k = 0;
//		LED1_7_Port->ODR = ~(1 << (k++));//1-7����ˮ
//		LED8_Port->ODR = ~(1 << 13);
//		delay_ms(300);
		
		if(k == 7)
		{
			k = 0;
			LED1_7_Port->ODR = ~(0 << 6);//��7�ƹر�
			LED8_Port->ODR = ~(1 << 13);//��8�ƿ���
		}
		else
		{
			LED1_7_Port->ODR = ~(1 << (k++));//1-7����ˮ
			LED8_Port->ODR = ~(0 << 13);//��8�ƹر�
		
		}
		delay_ms(300);//300ms�ӳ�
	
	}
	
	

}
