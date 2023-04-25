#include "tim6.h"



void TIM6_Delay_Init(void)
{
	RCC->APB1ENR |= 1 << 4;//ʹ��TIM6ʱ��
	
	TIM6->CR1 |= 1 << 7;//�Զ���װ��ʹ�ܣ�ʹ��Ӱ�ӼĴ�����
	
	TIM6->CR1 |= 1 << 3;//����������ģʽ
//	TIM6->CR1 &= ~(1 << 3);  //ѭ������
	
	TIM6->CR1 &= ~(1 << 2);//��������Դ
	
	TIM6->CR1 &= ~(1 << 1);//ʹ�ܸ���
	
//	TIM6->CR1 &= ~(1 << 0);//�رռ�����
}



void TIM6_Delay_Ms(uint16_t ms)  //84MHZ / 8400 = 1000HZ -- 0.1 ms   
{
	TIM6->PSC = 8400 - 1;//Ԥ��Ƶ
	
	TIM6->ARR = ms * 10;//д���Զ���װ��ֵ
	
	TIM6->EGR |= 1 << 0;//����װ��ֵ���ص�Ӱ���� �������1��Ӳ�����㣩
	
	TIM6->SR &= ~(1 << 0);//��־λ����

	
	TIM6->CR1 |= 1 << 0;//ʹ�ܼ�����
	
	while(!(TIM6->SR & (1 << 0)));//�ȴ�ʱ�䵽 ��SR�Զ���1 ��Ӳ����1��������㣩

	
	TIM6->SR &= ~(1 << 0);//��־λ����

	
	TIM6->CR1 &= ~(1 << 0);//�رռ�����


}

void TIM6_Delay_Us(uint32_t us)  //84MHZ / 84 = 1MHZ -- 1us 
{
	TIM6->PSC = 84 - 1;//Ԥ��Ƶ
	
	TIM6->ARR = us;//д���Զ���װ��ֵ
	
	TIM6->EGR |= 1 << 0;//����װ��ֵ���ص�Ӱ���� �������1��Ӳ�����㣩
	
	TIM6->SR &= ~(1 << 0);//��־λ����

	
	TIM6->CR1 |= 1 << 0;//ʹ�ܼ�����
	
	while(!(TIM6->SR & (1 << 0)));//�ȴ�ʱ�䵽 ��SR�Զ���1 ��Ӳ����1��������㣩

	
	TIM6->SR &= ~(1 << 0);//��־λ����

	
	TIM6->CR1 &= ~(1 << 0);//�رռ�����


}


void TIM6_Inter_Init(u16 psc,u32 arr)
{
	u32 Priority;
	
	RCC->APB1ENR |= 1 << 4;
	
	TIM6->CR1 |= 1 << 7;
	
	TIM6->CR1 &= ~(1 << 3);
	
	TIM6->CR1 &= ~(1 << 2);
	
	TIM6->CR1 &= ~(1 << 1);
	
	TIM6->DIER |= 1 << 0;//ʹ���ж�
	
	//�����ж�
	NVIC_SetPriorityGrouping(7-2);
	Priority = NVIC_EncodePriority(7-2,1,2);
	NVIC_SetPriority(TIM6_DAC_IRQn, Priority);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	
	
	TIM6->PSC = psc - 1;
	
	TIM6->ARR = arr;
	
	TIM6->EGR |= 1 << 0;
	
	TIM6->CR1 |= 1 << 0;
	


}

void TIM6_DAC_IRQHandler(void)
{
	if(TIM6->SR & (1 << 0))
	{
		TIM6->SR &= ~(1 << 0);//��־λ����
		GPIOC->ODR ^= 1 << 5;//LED��˸
	}

}


