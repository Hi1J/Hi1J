#include "spi.h"

void SPI1_Init(void)
{
	/*GPIOA����*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;//MOSI  SCK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//MISO
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*SPI1����*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = I2S_CPOL_Low;//���߿���ʱΪ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//���ݲ����һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSSʹ���������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //�ķ�Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ��ǰ
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);//ʹ��SPI1
	
}

