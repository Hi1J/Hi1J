#include "spi.h"

/*
FLASH_CS   -- PD11
FLASH_SCK  -- SPI2_SCK  - PB13 -- ӳ�书��
FLASH_MISO -- SPI2_MISO - PB14 -- ӳ�书��
FLASH_MOSI -- SPI2_MOSI - PB15 -- ӳ�书��
//�½��ط��ͣ������ؽ���
*/

void SPI2_Init(void)
{
	//ʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	//GPIOB����
	GPIO_InitTypeDef x;
	x.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	x.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;//SCK MOSI
	x.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&x);
	x.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	x.GPIO_Pin = GPIO_Pin_14;//MISO
	GPIO_Init(GPIOB,&x);
	
	//SPI2����
	SPI_InitTypeDef y;
	y.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//�ķ�Ƶ
	y.SPI_CPHA = SPI_CPHA_1Edge;//��һ��ʱ���زɼ�
	y.SPI_CPOL = SPI_CPOL_Low;//����ʱΪ�͵�ƽ
	y.SPI_CRCPolynomial = 7;//��CRCУ��
	y.SPI_DataSize = SPI_DataSize_8b;//8λ����λ
	y.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��˫��ȫ˫��
	y.SPI_FirstBit = SPI_FirstBit_MSB;//��λ��ǰ
	y.SPI_Mode = SPI_Mode_Master;//����ģʽ
	y.SPI_NSS = SPI_NSS_Soft;//NSS���������
	SPI_Init(SPI2,&y);
	
	//ʹ��SPI2
	SPI_Cmd(SPI2,ENABLE);

}
//SPI2���ͺͽ���
u8 SPI2_Send_Recv(u8 data)
{
	//�ȴ�������ɣ�����ǰ��ֹDR�л�������δ���ͣ�
	while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)));
	//����
	SPI_I2S_SendData(SPI2,data);
	//�ȴ��������
	while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)));
	//���ؽ��յ�������
	return SPI_I2S_ReceiveData(SPI2);

}
