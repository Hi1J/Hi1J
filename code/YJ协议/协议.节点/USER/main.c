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

	u8 buf[16];
	u8 buf1[8];
	u16 illuvalu = 0;
	Usart1_Init(115200);
	printf("����1��ʼ����ɣ�\r\n");
	Zigbee_Init();
//	DHT11_Config(1);
//	ILLU_Gpio_Init();
	buf[0] = 0xfa;//��ʼλ
	buf[1] = 0xff;//Ŀ���ַ��λ
	buf[2] = Zigbee_Address;//������ַ
	buf[3] = 0x03;//������
	buf[4] = 0x33;//�¶�
	buf[5] = 0x34;//ʪ��
	buf[6] = 0x35;//��ǿ��8λ
	buf[7] = 0x36;//��ǿ��8λ
	buf[8] = 0xfa;//��ʼλ
	buf[9] = 0xff;//Ŀ���ַ��λ
	buf[10] = Zigbee_Address;//������ַ
	buf[11] = 0x03;//������
	buf[12] = 0x38;//�¶�
	buf[13] = 0x3a;//ʪ��
	buf[14] = 0x4a;//��ǿ��8λ
	buf[15] = 0x5a;//��ǿ��8λ
	Zigbee_Send_Data(buf,16);//��������
//	Zigbee_Send_Data(buf1,sizeof(buf));//��������
	while(1)
	{
//		Get_DHT11_Data(&T,&H);
//		illuvalu = ILLU_ReadData();


		
		delay_s(2);
	}

}