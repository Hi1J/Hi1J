#include "tim4.h"


void TIM4_CH3_Init(u16 psc,u16 arr)
{
	//����GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//����TIM4
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�Ӳ��ָ�
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = arr;//�Զ���װ����
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc-1;//Ԥ��Ƶ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	//����PWM
	TIM_OCInitTypeDef TIM_OCInitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//CNT > CCRʱ�����Ч��ƽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�ߵ�ƽ��Ч
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	
	//ʹ��Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	//ʹ��
	TIM_Cmd(TIM4,ENABLE);
	
	
	
	
}