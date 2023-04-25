#ifndef __ESP8266_H_
#define __ESP8266_H_

#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "mqtt.h"

#define SSID "510" //WIFI �ȵ���
#define PWD "510510510" //����



extern u8 WiFi_Rx_Buff[512];//���ջ�����
extern u8 WiFi_Tx_Buff[512];//���ͻ�����
extern u8 WiFi_Rx_Flag;
extern u32 Rev_Len;
extern uint8_t ESP8266_STATE;


void Init_Esp8266(void);
u8 FindStr(char* dest,char* src,u16 retry_nms);
void UART5_Send_Str(uint8_t *buf );
void UART5_Send_Buf(uint8_t *buf , uint16_t len);
uint8_t ConnectServer(char* mode,char* ip,uint16_t port);




#endif