#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "usart.h"
#include "jpg_app.h"
#include "fatfs_app.h"
#include "Zigbee.h"
#include "esp8266.h"
#include "mqtt.h"




//�ڵ㣨Zigbee��---����ʪ���նȣ�-->�ն�----��ESP8266��-->OneNet---(�·�����)--->LED������
int main(void)
{

	Usart1_Init(115200);
	printf("����1��ʼ���ɹ�\r\n");
	YJ_Init();//YJЭ���ʼ��
	LED_Init();//LED��ʼ��
	LCD_Init();//lcd��ʼ��
	SD_Fatfs_Init();//sd����ʼ��
	
	LCD_Clear(LIGHTBLUE);
	JPG_ShowFunc("0:/pic2/cat2.jpg",0,0);
	JPG_ShowFunc("0:/pic1/tem03.jpg",5,160);
	JPG_ShowFunc("0:/pic1/hem03.jpg",5,220);
	JPG_ShowFunc("0:/pic1/light03.jpg",5,280);
	LCD_ShowStr(55,160+9,(u8 *)"�¶�:");
	LCD_ShowStr(55,220+9,(u8 *)"ʪ��:");
	LCD_ShowStr(55,280+9,(u8 *)"��ǿ:");
	
	LCD_ShowStr(125,160+9,(u8 *)"���϶�");
	LCD_ShowStr(125,220+9,(u8 *)"RH%");
	LCD_ShowStr(140,280+9,(u8 *)"Lux");
	Zigbee_Init();//Zigbee��ʼ��
	printf("Zigbee��ʼ����ɣ�\r\n");
	Init_Esp8266();//ESP8266��ʼ��
	printf("ESP8266��ʼ����ɣ�\r\n");
	AliIoT_Parameter_Init();//OneNet������ʼ��
	MQTT_ConectPack();//���ӷ�����
	delay_s(1);
	MQTT_Subscribe(S_TOPIC_NAME,0);//�ϴ�����
	printf("OneNet��ʼ����ɣ�\r\n");
	WiFi_Rx_Flag = 0;
	memset(WiFi_Rx_Buff,0,sizeof(WiFi_Rx_Buff));
	LED1_ON;
	
	MQTT_RxDataInPtr=MQTT_RxDataBuf[0]; 				 //ָ���ͻ�����������ݵ�ָ���λ
	MQTT_RxDataOutPtr=MQTT_RxDataInPtr; 				 //ָ���ͻ�������ȡ���ݵ�ָ���λ
	MQTT_RxDataEndPtr=MQTT_RxDataBuf[R_NUM-1];		     //ָ���ͻ�����������ָ���λ
	
	MQTT_CMDInPtr=MQTT_CMDBuf[0];                        //ָ���������������ݵ�ָ���λ
	MQTT_CMDOutPtr=MQTT_CMDInPtr;                        //ָ�����������ȡ���ݵ�ָ���λ
	MQTT_CMDEndPtr=MQTT_CMDBuf[C_NUM-1];              	 //ָ���������������ָ���λ
	
	while(1)
	{
		Zigbee_Data_Analysis();//Zigbee���ݴ���
		if(WiFi_Rx_Flag == 1)
		{
			memcpy(&MQTT_RxDataInPtr[2], WiFi_Rx_Buff, Rev_Len);  //�������ݵ����ջ�����
			MQTT_RxDataInPtr[0] = Rev_Len/256;                  	//��¼���ݳ��ȸ��ֽ�
			MQTT_RxDataInPtr[1] = Rev_Len%256;					 	//��¼���ݳ��ȵ��ֽ�
			MQTT_RxDataInPtr += RBUFF_UNIT;                                	//ָ������
			if(MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                     	//���ָ�뵽������β����
				MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                    	//ָ���λ����������ͷ
			Rev_Len = 0;                                        	//����5������������������
			WiFi_Rx_Flag = 0;
		
		}
		if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr)
		{
			printf("���յ����ݣ�\r\n");
			if(MQTT_RxDataOutPtr[2] == 0x30)
			{
				printf("�������ȼ�0����\r\n");
				MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);
			}
			MQTT_RxDataOutPtr += RBUFF_UNIT;                //ָ������
			if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //���ָ�뵽������β����
			{
				MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //ָ���λ����������ͷ              
			}	
		}
		if(MQTT_CMDOutPtr != MQTT_CMDInPtr)
		{
			printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);
			if(!memcmp(&MQTT_CMDOutPtr[2],"LED1ON",strlen("LED1ON")))
			{LED1_ON;}//LED1��
			else if(!memcmp(&MQTT_CMDOutPtr[2],"LED1OFF",strlen("LED1OFF")))
			{LED1_OFF;}//LED1��
			else printf("δָ֪��\r\n");
			
			MQTT_CMDOutPtr += CBUFF_UNIT;				//ָ������
			if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)	    //���ָ�뵽������β����
			MQTT_CMDOutPtr = MQTT_CMDBuf[0];			//ָ���λ����������ͷ	
		}			
	}

}


//�������ݺ���
void send_data(uint8_t tem,uint8_t hum,uint16_t light)
{
	static char head[3];
	static int	dataLen = 0;			//���ĳ���
	static char temp[50];				//����һ����ʱ������1,��������ͷ
	static char tempAll[100];			//����һ����ʱ������2��������������
	
	memset(head,0,3);
	memset(temp,0,50);
	memset(tempAll,0,100);
	
	sprintf(temp,"{\"tem\":\"%d\",\"hum\":\"%d\",\"light\":\"%d\"}",tem,hum,light);
	
	head[0] = 0x03;//�̶���ͷ
	head[1] = 0x00;//�̶���ͷ
	head[2] = strlen(temp);//ʣ�೤��	
	
	sprintf(tempAll,"%c%c%c%s",head[0],head[1],head[2],temp);
	
	dataLen = strlen(temp) + 3;
	
	MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, dataLen);//�������ݸ�������
}
