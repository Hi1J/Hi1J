#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"


int main(void)
{
	u8 i;
	u16 value = 0;
	Usart1_Init(115200);
	printf("����1��ʼ����ɣ�\r\n");
	Motor_Gpio_Init();
	delay_ms(200);
	while(1)
	{
		if(i<100)
		{
			Motorcw(1);//��ת
			delay_ms(10);//תһȦ��Ҫ��ʱ��
			i++;
		}
		else
		{
			Motorcw(0);//��ת
			delay_ms(10);
			i++;
			if(i>=200)i=0;
		
		}
		
	}

}