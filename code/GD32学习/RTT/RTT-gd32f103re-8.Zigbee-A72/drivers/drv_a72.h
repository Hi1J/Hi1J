#ifndef __DRV_A72_H_
#define __DRV_A72_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>
#include "app.h"



#define ZIGBEE_A72_NODE 1          //�ն˽ڵ�
#define ZIGBEE_A72_COORDINATOR 0   //Э����

#if ZIGBEE_A72_NODE

#define A72_WAKE_UP_SET()     gpio_bit_reset(GPIOA,GPIO_PIN_1);
#define A72_WAKE_UP_RESET()   gpio_bit_set(GPIOA,GPIO_PIN_1);

#endif

/*�������ض���*/
#define A72_Send_Data USART1_Send_Data
#define A72_RX_BUF USART1_RX_BUF
#define A72_Delay_ms TIM6_Delay_ms

/*��ʼ����λ*/
#define A72_Sbit 0x55
#define A72_Ebit 0xAA

/*��λ*/
#define RESET_Bytes 7  //ָ���ֽڸ���
#define C_RESET 0x000B//����
#define L_RESET 0x0000//����
#define V_RESET 0x00//У��

/*��ȡ��ַ*/
#define Read_Adder_Bytes 7 //ָ���ֽڸ���
#define C_Read_Adder 0x01C0//����
#define L_Read_Adder 0x0000//����
#define V_Read_Adder 0x00//У��
#define L_Adder_LEN 8    //IEEE��ַ����
#define S_Adder_LEN 2    //�̵�ַ����

/*A72������Ϣ�ṹ�嶨��*/
typedef struct
{
	uint8_t L_Adder[L_Adder_LEN];//IEEE
	uint8_t S_Adder[S_Adder_LEN];//�̵�ַ
	uint8_t Type_Device;//�豸����
	uint8_t Channel;//�ŵ�
	uint8_t N_nodes;//�ڵ�����(Э����)
	uint8_t S_nodes;//�ڵ�״̬(Э����)
	uint8_t S_Network;//����״̬(�ն˽ڵ�)
	uint8_t Sleep_Time;//����ʱ��(�ն˽ڵ�)
	
}A72_Return_Value;

/*��������*/
extern A72_Return_Value _value;

/*��������*/
void A72_Init(void);
int A72_Read_Address(void);
void A72_RESET(void);

#endif
