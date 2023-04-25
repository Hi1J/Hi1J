#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "lora.h"
#include "hal_wireless.h"
#include "DHT11.h"
#include "illu.h"
#include "string.h"

//LORA������ʪ����ǿ��

u8 T;
u8 H;

int main(void)
{

	u8 buf[4];
	u16 illuvalu = 0;
	Usart1_Init(115200);
	printf("����1��ʼ����ɣ�\r\n");
	Lora_Config();
	printf("Lora��ʼ���ɹ���\r\n");
	DHT11_Config(1);
	ILLU_Gpio_Init();
	
	while(1)
	{
		Get_DHT11_Data(&T,&H);
		illuvalu = ILLU_ReadData();
//		printf("%d\r\n",illuvalu);
//		sprintf((char *)buf,"%2d%2d%4d",T,H,illuvalu);
		buf[0] = T;//�¶�
		buf[1] = H;//ʪ��
		buf[2] = illuvalu >> 8;//��ǿ��8λ
		buf[3] = illuvalu & 0x00FF;//��ǿ��8λ
////		for(u8 i=0;i<4;i++)
////		{
////			printf("%x ",buf[i]);
////		}
////		printf("\r\n");
		Lora_SendData(buf,sizeof(buf));//��������
		delay_ms(100);
	}

}