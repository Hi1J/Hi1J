#include "led.h"





void LED_Init(void)
{
	//��ʱ��
 RCC->AHB1ENR |= 1<<2;//GPIOC
	
	//����GPIO
	 //����ģʽ
	GPIOC->MODER &= ~(3 << (2*4));//����
	GPIOC->MODER |= 1 << (2*4); //����Ϊͨ��ģʽ
	 //�����������
	GPIOC->OTYPER &=~(1 << 4);//�������
	 //����ٶ�
	GPIOC->OSPEEDR &=~(3<<(2*4));//����
	GPIOC->OSPEEDR |=2<<(2*4);//50HMz
	 //����������
	GPIOC->PUPDR &=~(3<<(2*4));//��������
	
	//���ó�ʼ��ƽΪ�ߵ�ƽ(LED��)
	GPIOC->ODR |= 1<< 4;
	
	
	
	
	GPIOC->MODER &= ~(3<<(2*5));
	GPIOC->MODER |= 1<<(2*5);
	GPIOC->OTYPER &=~(1<<5);
	GPIOC->OSPEEDR &= ~(3<<(2*5));
	GPIOC->OSPEEDR |= 2<<(2*5);
	GPIOC->PUPDR &= ~(3<<(2*5));
	GPIOC->ODR |= 1<<5;
	
	GPIOC->MODER &= ~(3<<(2*6));
	GPIOC->MODER |= 1<<(2*6);
	GPIOC->OTYPER &=~(1<<6);
	GPIOC->OSPEEDR &= ~(3<<(2*6));
	GPIOC->OSPEEDR |= 2<<(2*6);
	GPIOC->PUPDR &= ~(3<<(2*6));
	GPIOC->ODR |= 1<<6;
	
	
	GPIOC->MODER &= ~(3<<(2*7));
	GPIOC->MODER |= 1<<(2*7);
	GPIOC->OTYPER &=~(1<<7);
	GPIOC->OSPEEDR &= ~(3<<(2*7));
	GPIOC->OSPEEDR |= 2<<(2*7);
	GPIOC->PUPDR &= ~(3<<(2*7));
	GPIOC->ODR |= 1<<7;
	
	
	
	
	
	
	
	
	
	
	



}