#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>

//GD32����ģ��
int main()
{
	systick_config();//ʱ�ӳ�ʼ��
	
	rcu_periph_clock_enable(RCU_GPIOA);
	
	gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);
	
	while(1)
	{
		delay_1ms(500);
		gpio_bit_set(GPIOA,GPIO_PIN_1);
		delay_1ms(500);
		gpio_bit_reset(GPIOA,GPIO_PIN_1);
		
	}
}

