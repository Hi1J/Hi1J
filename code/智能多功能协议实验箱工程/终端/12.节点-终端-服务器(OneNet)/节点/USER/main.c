#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "DHT11.h"
#include "illu.h"
#include "string.h"
#include "Zigbee.h"

//Zigbee������ʪ����ǿ��
u8 T;
u8 H;

int main(void)
{

	u8 buf[8];
	u16 illuvalu = 0;
	Usart1_Init(115200);
	printf("����1��ʼ����ɣ�\r\n");
	Zigbee_Init();
	DHT11_Config(1);
	ILLU_Gpio_Init();
	buf[0] = 0xfa;
	buf[1] = 0xff;
	buf[2] = 0x03;
	buf[7] = 0x00;
	while(1)
	{
		Get_DHT11_Data(&T,&H);
		illuvalu = ILLU_ReadData();
		
		buf[3] = T;//�¶�
		buf[4] = H;//ʪ��
		buf[5] = illuvalu >> 8;//��ǿ��8λ
		buf[6] = illuvalu & 0x00FF;//��ǿ��8λ

		Zigbee_Send_Data(buf,sizeof(buf));//��������
		delay_s(1);
	}

}