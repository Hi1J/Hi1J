#ifndef _MQ2_H_
#define _MQ2_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
//#include "sensor_config.h"

//�궨��˿ں͹ܽţ��ײ��޸���������ˣ�
//#if(MQ2_POS == MIDDLE_POS)
#define MQ2_CLK   RCC_APB2Periph_GPIOB //�궨��˿�ʱ��
#define MQ2_PORT  GPIOB                //�궨��˿�
#define MQ2_PIN   GPIO_Pin_1           //�궨��ܽ�
#define MQ2_ADC_CHANNEL ADC_Channel_9  //�궨��ADCͨ��
//#elif(MQ2_POS == RIGHT_POS)
//#define MQ2_CLK   RCC_APB2Periph_GPIOA //�궨��˿�ʱ��
//#define MQ2_PORT  GPIOA                //�궨��˿�
//#define MQ2_PIN   GPIO_Pin_0           //�궨��ܽ�
//#define MQ2_ADC_CHANNEL ADC_Channel_0  //�궨��ADCͨ��
//#endif

extern u16 mq2_val;

void MQ2_Gpio_Init(void);
int MQ2_GetValue(u16 *adc_val);
u16 MQ2_Show_Usart(void);

#endif

