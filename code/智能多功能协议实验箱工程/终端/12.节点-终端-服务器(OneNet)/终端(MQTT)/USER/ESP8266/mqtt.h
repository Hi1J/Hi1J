#ifndef __MQTT_H_
#define __MQTT_H_

#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "esp8266.h"
#include "utils_hmac.h"

typedef struct {
    uint32_t total[2];          /*!< number of bytes processed  */
    uint32_t state[5];          /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */
} iot_sha1_context;

#define KEY_IOPAD_SIZE 64
#define MD5_DIGEST_SIZE 16
#define SHA1_DIGEST_SIZE 20

#define  R_NUM               5     //���ջ���������
#define  RBUFF_UNIT          300   //���ջ���������

#define  C_NUM               5     //�����������
#define  CBUFF_UNIT          300   //�����������


extern unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];       //�ⲿ�������������ݵĽ��ջ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
extern unsigned char *MQTT_RxDataInPtr;                        //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_RxDataOutPtr;                       //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_RxDataEndPtr;                       //�ⲿ����������ָ�򻺳���������λ��
extern unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];          //�ⲿ�����������������ݵĽ��ջ�����
extern unsigned char *MQTT_CMDInPtr;                           //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_CMDOutPtr;                          //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_CMDEndPtr;                          //�ⲿ����������ָ�򻺳���������λ��


#define CLIENTID "1065278300"//�豸ID
#define USERNAME "589281"   //��ƷID
#define IP    "183.230.40.39"  //IP��ַ
#define PORT 6002         //�˿�

#define PASSWARD "123456"  //��Ȩ��Ϣ

#define S_TOPIC_NAME  "topic_one" //����·��

#define  Data_TOPIC_NAME      "$dp"

#define WIFI_Send_Buf UART5_Send_Buf

void AliIoT_Parameter_Init(void);
void MQTT_ConectPack(void);
void MQTT_Subscribe(char *topic_name, int QoS);
void MQTT_PublishQs0(char *topic, char *data, int data_len);
void MQTT_DealPushdata_Qs0(unsigned char *redata);
void CMDBuf_Deal(unsigned char *data, int size);

void send_data(uint8_t tem,uint8_t hum,uint16_t light);

extern uint8_t data_buf[256];


#endif