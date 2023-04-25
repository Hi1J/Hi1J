#include "tim6.h"

void TIM6_Delay_Init()
{
	RCC->APB1ENR |= 1 << 4;//TIM6ʱ��ʹ��
	
	TIM6->CR1 |= 1 << 7;//�Զ���װ��ʹ�� ��ʹ��Ӱ�ӼĴ�����
	
	TIM6->CR1 |= 1 << 3;//ʹ�õ�����ģʽ
	
	TIM6->CR1 &= ~(1 << 2);//��������Դ
	
	TIM6->CR1 &= ~(1 << 1);//ʹ�ܸ���
	
	

}

void TIM6_Delay_ms(u16 ms)// 72000000MHZ/7200=10000HZ --- 0.1ms 
{
	TIM6->PSC = 7200 - 1;//Ԥ��ƵֵΪ8400
	
	TIM6->ARR = ms * 10;//1ms����10��
	
	TIM6->EGR |= 1 << 0;//����װ��ֵ���ص�Ӱ���� �������1��Ӳ�����㣩
	
	TIM6->SR &= ~(1 << 0);//��־λ����
	
	TIM6->CR1 |= 1 << 0;//����������
	
	while(!(TIM6->SR & (1 << 0)));//�ȴ�ʱ�䵽 ��SR�Զ���1 ��Ӳ����1��������㣩
	
	TIM6->SR &= ~(1 << 0);//��־λ����
	
	TIM6->CR1 &= ~(1 << 0);//�رռ�����
	
	
}

void TIM6_Delay_us(u16 us)// 72000000MHZ/72=1MHZ --- 1us 
{
	TIM6->PSC = 72 - 1;//Ԥ��ƵֵΪ8400
	
	TIM6->ARR = us;//1us����1��
	
	TIM6->EGR |= 1 << 0;//����װ��ֵ���ص�Ӱ���� �������1��Ӳ�����㣩
	
	TIM6->SR &= ~(1 << 0);//��־λ����
	
	TIM6->CR1 |= 1 << 0;//����������
	
	while(!(TIM6->SR & (1 << 0)));//�ȴ�ʱ�䵽 ��SR�Զ���1 ��Ӳ����1��������㣩
	
	TIM6->SR &= ~(1 << 0);//��־λ����
	
	TIM6->CR1 &= ~(1 << 0);//�رռ�����
	
	
}

void TIM6_Inter_Init(u16 psc,u32 arr)
{
	u32 Priority;
	
	RCC->APB1ENR |= 1 << 4;//ʹ��ʱ��
	
	TIM6->CR1 |= 1 << 7;//�Զ���װ��ֵʹ��
	
	TIM6->CR1 &= ~(1 << 3);//ѭ������
	
	TIM6->CR1 &= ~(1 << 2);//ʹ�ܸ���
	
	TIM6->CR1 &= ~(1 << 1);//ʹ�ܸ���
	
	TIM6->DIER |= 1 << 0;//ʹ���ж�
	
	//�ж�����
	NVIC_SetPriorityGrouping(7-2);
	Priority = NVIC_EncodePriority(7-2,1,2);
	NVIC_SetPriority(TIM6_IRQn , Priority);
	NVIC_EnableIRQ(TIM6_IRQn );
	
	TIM6->PSC = psc - 1;//Ԥ����
	
	TIM6->ARR = arr;//�Զ���װ��
	
	TIM6->EGR |= 1 << 0;//����װ��ֵ���ص�Ӱ���� �������1��Ӳ�����㣩
	
	TIM6->CR1 |= 1 << 0;//ʹ��
	
	
}
u8 K = 0;
void TIM6_IRQHandler(void)
{
	if(TIM6->SR & (1 << 0))
	{

		LED1_TOGGLE;  //LED1��ת
		K ++;  //��־λ��1 500ms��һ�� ��5s�ӵ�10��
		TIM6->SR &= ~(1 << 0);//��־λ����
    

        
		}
		
}
