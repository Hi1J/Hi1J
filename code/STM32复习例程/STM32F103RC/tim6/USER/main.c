#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "tim6.h"


//����Ч����led1 ��˸5�� Ȼ��ֹͣ��˸5��  5��������˸  ��ǰ5sLEDÿ��500s��ת  ��5sLED����ת�� 

int main()
{
	LED_Init();
	TIM6_Inter_Init(7200,5000);//500ms����һ���ж�
	while(1)
	{
		if(K == 10)     //��K==10ʱ �պ���˸5��
		{
			TIM6->CR1 &= ~(1 << 0);//�رռ�����
			K = 0;//��־λ����
			delay_s(5);//��ʱ5s
			
			TIM6->CR1 |= 1 << 0;//����������
			
		}
	
	}
	
	
}