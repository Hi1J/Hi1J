#ifndef __YJ_H_
#define __YJ_H_

//����ͷ�ļ�--------
#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
//����ͷ�ļ�--------

//�û�ͷ�ļ�--------
#include "delay.h"
#include "Zigbee.h"
#include "mqtt.h"
//�û�ͷ�ļ�--------


//�û��ɸ�����------
#define Address  0xff //��ַ
#define Start_bit 0xfa  //��ʼλ
#define Data_Frame_Max  8//����֡���ֵ 
#define System_Frequency_MHz 72  //�û���Ƭ����Ƶ����λMHz��
//�û��ɸ�����------


#define YJ_ON  1  
#define YJ_OFF 0 								
						

//��������������
struct Yj_node{
	u8 YJ_RX_Buf[Data_Frame_Max];
	struct Yj_node* next;
};

//��������
extern uint8_t YJ_RX_Aframe_OK;
//extern uint8_t Multipe_data_frames_Flag;

extern struct Yj_node Yjnode1;
extern struct Yj_node Yjnode2;
extern struct Yj_node Yjnode3;

//��������
void YJ_Init(void);
void YJ_Tset(void);
void YJ_Data_reception(uint8_t data);
void YJ_Data_Analysis(void);
void YJ_TASK_START(void);
void YJ_TASK_END(void);
uint8_t YJ_TASK_STATUS(void);
//void YJ_Delay_us(uint16_t time);
void YJ_Delay_Ms(uint16_t time);








#endif
