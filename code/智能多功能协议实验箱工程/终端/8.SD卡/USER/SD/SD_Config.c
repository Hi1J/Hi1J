#include "SD_Config.h"

/*
SPI2�ӿ�
SD_CS -- PD12
*/
void SD_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//SD_CS�ܽ� -- PD12
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	//CS�ܽ�����
	GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET);
	//SPI1��ʼ��
	SPI2_Init();
}





