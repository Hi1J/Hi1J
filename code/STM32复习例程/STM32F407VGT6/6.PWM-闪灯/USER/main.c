#include "stm32f4xx.h"
#include "tim9.h"


//���Ƴ���
int main(void)
{
	
	TIM9_CH1_Init(16800,10000,5000);//pwm:1/2  500ms�ߵ�ƽ 500ms�͵�ƽ
	

	while(1)
	{
		
	}
}

