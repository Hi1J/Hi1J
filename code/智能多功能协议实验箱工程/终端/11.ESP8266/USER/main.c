#include "stm32f10x.h"
#include "delay.h"
//#include "lcd.h"
#include "usart.h"
//#include "jpg_app.h"
//#include "fatfs_app.h"
#include "esp8266.h"
#include "mqtt.h"





//Zigbee������ʪ����ǿ��
int main(void)
{

	Usart1_Init(115200);
	printf("����1��ʼ���ɹ�\r\n");
//	LCD_Init();//lcd��ʼ��
//	SD_Fatfs_Init();//sd����ʼ��
	Init_Esp8266();
	AliIoT_Parameter_Init();
	MQTT_ConectPack();
	delay_s(1);
	MQTT_Subscribe(S_TOPIC_NAME,0);
	printf("��ʼ����ɣ�\r\n");
	
	while(1)
	{
		
		
	}

}
