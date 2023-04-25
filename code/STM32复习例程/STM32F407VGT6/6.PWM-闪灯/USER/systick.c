#include "systick.h"


void delay_ms(u32 ms)
{
	SysTick->CTRL &= ~(1 << 2); //ʱ��Դѡ���ⲿʱ��Դ
	
	SysTick->LOAD = SysTick_ * ms * 1000;//д����װ��ֵ   = �ⲿʱ��Ƶ�� * us 
	
	SysTick->VAL = 0;//����������
	
	SysTick->CTRL |= 1 << 0; //ʹ�ܼ�����
	
	while(!(SysTick->CTRL & (1 << 16)));//�ȵ�����0���жϱ�־λ��
	
	SysTick->CTRL &= ~(1 << 0);//�رռ�����
	
		

}


//�жϷ�ʽ 
void delay_Inter_ms(u32 ms)
{
	SysTick->CTRL &= ~(1 << 2);//ʱ��Դѡ���ⲿʱ��Դ
	
	SysTick->LOAD =SysTick_ * ms * 1000;
	
	SysTick->VAL = 0;//����������
	
	SysTick->CTRL |= 1 << 1; //���ж�
	
	SysTick->CTRL |= 1 << 0;//ʹ�ܼ�����
	
	
	//���ﲻ��Ҫ���ж� ��������Ҫÿ������ms��ִ���жϷ����� �� ��������������while��
//	SysTick->CTRL &= ~(1 << 0);
	
	

}

//�жϷ�����
//���ﲻ��Ҫ����NVIC ��ΪSysTick��NVICͬ���ں���
void SysTick_Handler()
{
	if(SysTick->CTRL & (1 << 16))
	{
		//���ﲻ��Ҫ���־λ����ΪCTRL�ĵ�16λ���Զ�Ӳ������
		
		//�жϺ�������
		GPIOC->ODR ^= 1 << 7;//led4��ת  
	
	
	}
}