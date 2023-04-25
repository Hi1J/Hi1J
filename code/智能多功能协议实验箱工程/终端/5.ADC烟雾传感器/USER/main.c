#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

//��������MQ2
int main(void)
{
	u16 value = 0;//����һ��ֵ�����MQ2
	Usart1_Init(115200);//���ڳ�ʼ��
	ADC1_CH9_Init();//ADC1ͨ��9��ʼ��
	
	while(1)
	{
		value = MQ_2_GetValue();//��ȡ����
		printf("����������%d\r\n",value);//��ӡ
		delay_s(1);
			
		
		
	}
	
	

}
