#include "adc.h"


void ADC1_CH4_Init()
{
	/*����GPIOA*/
	RCC->AHB1ENR |= 1 << 0;//����GPIOAʱ��
	
	GPIOA->MODER &= ~(3 << (2 * 4));//����
	GPIOA->MODER |= 3 << (2 * 4);//ģ��ģʽ
	
	/*����ADC1*/
	RCC->APB2ENR |= 1 << 8;//����ADC1ʱ��
	
	ADC1->CR1 &= ~(3 << 24);//12λ�ֱ���
	
	ADC1->CR2 &= ~(1 << 11);//�Ҷ���
	
	ADC1->CR2 |= 1 << 10;//ʹ�ܹ���EOC��־λ
	
	ADC1->CR2 &= ~(1 << 1);//����ת��ģʽ
	
	ADC->CCR &= ~(3 << 16);//����
	ADC->CCR |= 1 << 16;//4��Ƶ
	
	ADC1->SMPR2 &= ~(7 << (4 * 3));//����
	ADC1->SMPR2 |= 7 << (4 * 3);//480������
	
	
	ADC1->SQR1 &= ~(15 << (4 * 4));//1��ת��
	
	
	ADC1->SQR3 |= 4 << (0 * 4);//һ��ת��
	
	
	ADC1->CR2 |= 1 << 0;   //ʹ��ADC1


}

u16 Get_ADC1_CH4(void)
{
	u16 data;
	
	ADC1->CR2 |= 1 << 30;//չ��ת������
	
	while(!(ADC1->SR & (1 << 1)));//�ȴ�ת�����
	
	ADC1->SR &= ~(1 << 1); //��־λ����
	
	data = ADC1->DR;  //��ȡ����
	
	
	return data;

}


