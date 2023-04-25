#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"

//����1 - 8 ���� LED 1 - 8
int main(void)
{
	u8 KeyValue;//����һ��ֵ��¼�ĸ���������
	LED_Init(); //LED��ʼ��
	
	KEY_Indenpendent_Init();//����������ʼ��
	
	while(1)
	{
		KeyValue = Key_GetValue();//ɨ��
		switch(KeyValue)
		{
			case 1:LED1_TOGGLE;break;
			case 2:LED2_TOGGLE;break;
			case 3:LED3_TOGGLE;break;
			case 4:LED4_TOGGLE;break;
			case 5:LED5_TOGGLE;break;
			case 6:LED6_TOGGLE;break;
			case 7:LED7_TOGGLE;break;
			case 8:LED8_TOGGLE;break;
			default:break;
		}
		
		
	}
	
	

}
