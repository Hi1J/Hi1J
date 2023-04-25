#include "esp8266.h"


u8 WiFi_Rx_Buff[512];//���ջ�����
u8 WiFi_Tx_Buff[512];//���ͻ�����
u8 WiFi_Rx_Flag = 0;
u32 Rev_Len=0;
uint8_t ESP8266_STATE;

/*�ײ㴮�ڳ�ʼ��*/
void WIFI_USART_Init(u32 baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	USART_InitTypeDef USART_InitStruct = {0};
	//1.�� C/D �˿�ʱ�Ӻ� UART5 �˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	//2.PC12 ����Ϊ�����������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//ͨ���������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ� 50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	//3.PD2 ����Ϊ��������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	//���ڳ�ʼ��
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	USART_InitStruct.USART_BaudRate = baud_rate;
	USART_Init(UART5,&USART_InitStruct);
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);
	USART_Cmd(UART5,ENABLE);
	NVIC_SetPriority(UART5_IRQn,1);
	NVIC_EnableIRQ(UART5_IRQn);
}
/*USART5 ����һ�ֽ�����*/
void UART5_Send_Data(uint8_t data)
{
	while(!(UART5->SR & (0x01<<7)));
	UART5->DR = data;
}
/*USART5 ����ָ����������*/
void UART5_Send_Buf(uint8_t *buf , uint16_t len)
{
	while(len--) UART5_Send_Data(*buf++);
}
/*USART5 �����ַ�������*/
void UART5_Send_Str(uint8_t *buf )
{
	while(* buf !=0) 
	{UART5_Send_Data(*buf);buf++;}
}
u8 FindStr(char* dest,char* src,u16 retry_nms)
{
	 retry_nms/=10; //��ʱʱ��
	 while((strstr(dest,src)==0) && retry_nms--)//�ȴ����ڽ�����ϻ�ʱ�˳�
	 {
		delay_ms(10);
	 }
	 retry_nms+=1;
	 if(retry_nms)
	 {
		return 1;
	 }
	return 0; 
}

//����5�жϷ�����
void UART5_IRQHandler(void)
{
	static u32 len = 0;
	uint8_t tmp;
	if(USART_GetITStatus(UART5,USART_IT_RXNE))
	{
		tmp=UART5->DR;
		WiFi_Rx_Buff[len++]=tmp;
		USART1->DR=tmp;
	}
	if(USART_GetITStatus(UART5,USART_IT_IDLE))
	{
		tmp = UART5->DR;
		WiFi_Rx_Flag = 1;
		Rev_Len = len;
		len = 0;
	}
	USART_ClearITPendingBit(UART5,USART_IT_RXNE);
}
/**
* ���ܣ���� ESP8266 �Ƿ�����
* ������None
* ����ֵ��ESP8266 ����״̬
* �� 0 ESP8266 ����
* 0 ESP8266 ������ 
*/
uint8_t Check_Esp8266(void)
{
	uint8_t check_cnt=5;
	while(check_cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); //��ս��ջ���
		UART5_Send_Str((uint8_t *)"AT\r\n"); //���� AT ����ָ��
		delay_ms(20);
		if(FindStr((char*)WiFi_Rx_Buff,"OK",200) != 0)
		{
			printf("��⵽ ESP8266\r\n");
			return 1;
		}
	}
		printf("δ��⵽ ESP8266\r\n");
	return 0;
}
/**
* ���ܣ���͸��
* ������None
* ����ֵ��ESP8266 ����״̬
* �� 0 ESP8266 ����
* 0 ESP8266 ������ 
*/
static uint8_t OpenTransmission(void)
{
//����͸��ģʽ
	uint8_t cnt=2;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 

		UART5_Send_Str((uint8_t *)"AT+CIPMODE=1\r\n"); 
		if(FindStr((char*)WiFi_Rx_Buff,"OK",200)!=0)
		{
			return 1;
		}
	}
	return 0;
}
//�˳�͸��
static void CloseTransmission(void)
{
	UART5_Send_Str((uint8_t *)"+++"); delay_ms(50);
	UART5_Send_Str((uint8_t *)"+++"); delay_ms(50);
}
/*
�ر� ESP8266 ����
*/
void close_esp8266_echo(void)
{
	UART5_Send_Str((uint8_t *)"ATE0\r\n");
}
/* ���� ESP8266*/
void RESET_ESP8266(void)
{
	UART5_Send_Str((uint8_t *)"AT+RST\r\n"); 
}
static uint8_t ConnectAP(char* ssid,char* pswd)
{
	uint8_t cnt=5;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 

		UART5_Send_Str((uint8_t *)"AT+CWMODE_CUR=1\r\n"); 
		//����Ϊ STATION ģʽ
		if(FindStr((char*)WiFi_Rx_Buff,"OK",200) != 0)
		{
		break;
		} 
	}
	if(cnt == 0) 
		return 0;
	cnt=2;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); //��շ��ͻ���
		memset(WiFi_Tx_Buff,0,512); //��ս��ջ���
		sprintf((char*)WiFi_Tx_Buff,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",ssid,pswd);//����Ŀ�� AP
		UART5_Send_Str(WiFi_Tx_Buff);
		if(FindStr((char*)WiFi_Rx_Buff,"OK",8000)!=0) 
		//���ӳɹ��ҷ��䵽 IP
		{
			return 1;
		}
	}
	return 0;
}
void Init_Esp8266(void)
{
	uint8_t flag=0;
	WIFI_USART_Init(115200); //�� WIFI ͨ�� USART ��ʼ��
	CloseTransmission(); //�˳�͸�� ��ֹϵͳ��λ��ESP8266 ������͸��ģʽ
	RESET_ESP8266(); //���� ESP8266 
	delay_ms(2000); //�� ESP8266 ��ʼ��ʱ��
	flag=Check_Esp8266();

	if(flag==0)
	{
		ESP8266_STATE = 0; 
		return ;
	}
	//close_esp8266_echo();
	flag=ConnectAP(SSID,PWD);
	if(flag==0)
	{
		ESP8266_STATE = 0; 
		return ;
	}else 
	{
		printf("WIFI ���ӳɹ�\r\n");
	}
}

/**
* ���ܣ�ʹ��ָ��Э��(TCP/UDP)���ӵ�������
* ������
* mode:Э������ "TCP","UDP"
* ip:Ŀ������� IP
* port:Ŀ���Ƿ������˿ں�
* ����ֵ��
* ���ӽ��,�� 0 ���ӳɹ�,0 ����ʧ��
* ˵����
* ʧ�ܵ�ԭ�������¼���(UART ͨ�ź� ESP8266 ���������)
* 1. Զ�̷����� IP �Ͷ˿ں�����
* 2. δ���� AP
* 3. �������˽�ֹ���(һ�㲻�ᷢ��)
*/
uint8_t ConnectServer(char* mode,char* ip,uint16_t port)
{
	uint8_t cnt;
	 

	 // CloseTransmission(); //����������˳�͸��
	 delay_ms(500);
	//���ӷ�����
	cnt=2;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 
		memset(WiFi_Tx_Buff,0,512); 
		sprintf((char*)WiFi_Tx_Buff,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",mode,ip,port);
		UART5_Send_Str(WiFi_Tx_Buff);
		if(FindStr((char*)WiFi_Rx_Buff,"CONNECT",8000) !=0 )
		{
			break;
		}
	}
	if(cnt == 0) 
			return 0;
	//����͸��ģʽ
	if(OpenTransmission()==0) return 0;
	//��������״̬
	cnt=2;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 
		UART5_Send_Str((uint8_t *)"AT+CIPSEND\r\n");//��ʼ����͸������״̬
		if(FindStr((char*)WiFi_Rx_Buff,">",200)!=0)
		{
			return 1;
		}

	}
	return 0;
}
/**
* ���ܣ������ͷ������Ͽ�����
* ������None
* ����ֵ��
* ���ӽ��,�� 0 �Ͽ��ɹ�,0 �Ͽ�ʧ��
*/
uint8_t DisconnectServer(void)
{
	uint8_t cnt;
	CloseTransmission(); //�˳�͸��
	delay_ms(500);
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 
		UART5_Send_Str((uint8_t *)"AT+CIPCLOSE\r\n");//�ر�����
		if(FindStr((char*)WiFi_Rx_Buff,"CLOSED",200)!=0)//�����ɹ�,�ͷ������ɹ��Ͽ�
		{
			break;
		}
	}
	if(cnt) return 1;
	return 0;
}

void Smart_Config(void)
{

	uint8_t flag=0;
	CloseTransmission(); //�˳�͸�� ��ֹϵͳ��λ��ESP8266 ������͸��

	RESET_ESP8266(); //���� ESP8266 
	delay_ms(2000); //�� ESP8266 ��ʼ��ʱ��
	flag=Check_Esp8266();
	if(flag==0)
	{
		ESP8266_STATE = 0; 
		return ;
	}
	memset(WiFi_Rx_Buff,0,512);
	UART5_Send_Str((uint8_t *)"AT+CWSTARTSMART\r\n");//��������
	//smartconfig connected wifi
	while(FindStr((char*)WiFi_Rx_Buff,"smartconfig connected wifi",200)==0)
	{
		delay_ms(1);
	}
	printf("�������\r\n");
}
