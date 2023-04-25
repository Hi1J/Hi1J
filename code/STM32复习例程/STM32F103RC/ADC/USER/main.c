#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"


int main()
{
	
	u16 temp = 0;
	float Vx = 0;

	USART1_Config();
	ADC1_CH4_Init();

	
	while(1)
	{
		temp = ADC1_CH4_GetValue(); // ��ȡ������
		Vx =(temp * 3.3) / 4096;  //��ȡģ����
		printf("������=%d\r\n",temp);
		printf("ģ����=%.1f\r\n\r\n",Vx);
		delay_s(2);
		
	
	}
	
	
}