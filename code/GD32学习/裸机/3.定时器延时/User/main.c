#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "usart.h"
#include "timer.h"

//USART�շ�
int main()
{
	systick_config();//ʱ�ӳ�ʼ��
	usart0_init(115200U);
	printf("����0��ʼ����ɣ�\r\n");
	TIM6_Delay_Init();
	
	printf("���1\r\n");
	TIM6_Delay_ms(500);
	printf("���2\r\n");
	
	
	while(1)
	{
//		TIM6_Delay_ms(500);
//		printf("yes\r\n");
		
	}
}

