#ifndef _LORA_H_
#define _LORA_H_

#include "stm32f10x.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "hal_wireless.h"
//#include "sensor.h"

//����λ����M1 ����λ����M0
//M0 -- SPI_CS    -- PA8 (ͨ���������ģʽ)
//M1 -- SPI_SCK   -- PA11 (ͨ���������ģʽ)
//AUX - SPI_MISO  -- PA12 (��������ģʽ)
#define LORA_M0_CLK       RCC_APB2Periph_GPIOA
#define LORA_M0_PORT      GPIOA
#define LORA_M0_PIN       GPIO_Pin_8

#define LORA_M1_CLK       RCC_APB2Periph_GPIOA
#define LORA_M1_PORT      GPIOA
#define LORA_M1_PIN       GPIO_Pin_11

#define LORA_AUX_CLK       RCC_APB2Periph_GPIOA
#define LORA_AUX_PORT      GPIOA
#define LORA_AUX_PIN       GPIO_Pin_12

#define LORA_MODE_PATTERN     0x00 //һ��ģʽ
#define LORA_MODE_WARKUP      0x01 //����ģʽ
#define LORA_MODE_POWERSAVE   0x10 //ʡ��ģʽ
#define LORA_MODE_SLEEP       0x11 //����ģʽ
/*********************����Lora��������***********/
//�޸Ĳ�������
#define CMD_STOREPRM      0xC0//�����ò����ᱣ��
#define CMD_NOTSTOREPRM   0xC2//�����ò������ᱣ��
//UARTЭ������żУ��
#define PRM_UART_Parity_8N1      0x00//����żУ��
#define PRM_UART_Parity_8O1      0x01//żУ��
#define PRM_UART_Parity_8E1      0x10//��У��
//UARTЭ���в�����
#define PRM_UART_BaudRate_1200    0x00
#define PRM_UART_BaudRate_2400    0x01
#define PRM_UART_BaudRate_4800    0x02
#define PRM_UART_BaudRate_9600    0x03
#define PRM_UART_BaudRate_19200   0x04
#define PRM_UART_BaudRate_38400   0x05
#define PRM_UART_BaudRate_57600   0x06
#define PRM_UART_BaudRate_115200  0x07

//���ÿ������ݴ�������
#define PRM_Sky_Speed_0_3       0x00//0.3kbps
#define PRM_Sky_Speed_1_2       0x01//1.2kbps
#define PRM_Sky_Speed_2_4       0x02//2.4kbps
#define PRM_Sky_Speed_4_8       0x03//4.8kbps 
#define PRM_Sky_Speed_9_6       0x04//9.6kbps
#define PRM_Sky_Speed_19_2      0x05//19.2kbps

//���ô���ģʽ
#define PRM_TRAN_MODE_TT        0x00//͸������ģʽ
#define PRM_TRAN_MODE_FP        0x01//���㴫��ģʽ
//AUX RX TX IO������ʽ
#define PRM_IO_Driver_PP        0x01//�ڲ���������
#define PRM_IO_Driver_OD        0x00//�ڲ����п�·
//����ʱ��
#define PRM_WarkUp_Time_250     0x00//��λms
#define PRM_WarkUp_Time_500     0x01
#define PRM_WarkUp_Time_750     0x02
#define PRM_WarkUp_Time_1000    0x03
#define PRM_WarkUp_Time_1250    0x04
#define PRM_WarkUp_Time_1500    0x05
#define PRM_WarkUp_Time_1750    0x06
#define PRM_WarkUp_Time_2000    0x07

//FEC������
#define PRM_FEC_OPEN            0x01//��
#define PRM_FEC_CLOSE           0x00//�ر�
//���÷��书��
#define PRM_EmissionPower_20    0x00//20dbm
#define PRM_EmissionPower_17    0x01
#define PRM_EmissionPower_14    0x02
#define PRM_EmissionPower_10    0x03

//����Ĭ�ϵĲ�����Ҳ�������������ò�����
#define CMD_DEFAULT                    CMD_STOREPRM
#define PRM_UART_Parity_DEFAULT        PRM_UART_Parity_8N1
#define PRM_UART_BaudRate_DEFAULT      PRM_UART_BaudRate_9600
#define PRM_Sky_Speed_DEFAULT          PRM_Sky_Speed_2_4
#define PRM_CHANNEL_DEFAULT            0x17//00-73h�仯 410~525MHz
#define PRM_TRAN_MODE_DEFAULT          PRM_TRAN_MODE_TT
#define PRM_WarkUp_Time_DEFAULT        PRM_WarkUp_Time_250
#define PRM_FEC_DEFAULT                PRM_FEC_OPEN
#define PRM_EmissionPower_DEFAULT      PRM_EmissionPower_20

typedef struct
{
	u8 head;
	u8 addh;
	u8 addl;
	u8 sped;
	u8 chan;
	u8 option;
}LORA_WORKPRM;//��������

typedef union 
{
	LORA_WORKPRM lora_workprm;
	u8 buff[6];
}LORA_WORKDATA;//���ջ����͹�������


void Lora_Config(void);              //Lora��ʼ��
void Lora_SendData(u8 *data,u16 len);//Lora��������
void Lora_SendCmd(u8 *data,u16 len);
void Lora_SetMode(u8 mode);          //����Loraģ���ģʽ
void Lora_Reset(void);               //Loraģ����и�λ
void Lora_CurrentPram(void);         //��ȡLoraģ�鵱ǰ�Ĳ���
void Lora_WorkParameters(void);      //��ʾLoraģ��Ĳ���
void Lora_SetWorkParameters(void);   //����Loraģ��Ĳ���
void Lora_DataAnalysis(void);        //���ݵĽ���

void DataAnalysis_Func00(void);       //������0x00�Ľ���
void DataAnalysis_Func03(void);       //������0x03�Ľ���
void DataAnalysis_Func04_SetFlag(void);//������0x04�Ľ���
void DataAnalysis_Func04_TxAck(void);
#endif
