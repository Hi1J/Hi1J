#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "illu.h"



int main(void)
{
	u16 value = 0;
	Usart1_Init(115200);
	printf("����1��ʼ����ɣ�\r\n");
	ILLU_Gpio_Init();
	while(1)
	{
		ILLU_Show_Usart();
		delay_s(1);
	}

}