#include "adc.h"


void ADC1_CH4_Init(void)
{
	/*GPIOA����*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//ADC1_CH4
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*����ADC1������ʽ*/
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//����ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�����������
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//��������
	ADC_InitStructure.ADC_NbrOfChannel = 1;//���й���ת����ADC ͨ����Ϊ1
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//����ģʽ
	ADC_Init(ADC1,&ADC_InitStructure);
	
	
	/*����ADC1*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);//ͨ��4 
	
	ADC_Cmd(ADC1,ENABLE);//ʹ��ADC1
}

u16 ADC1_CH4_GetValue(void)
{
	u16 value;
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�������
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�SRӲ����1
	
	value = ADC_GetConversionValue(ADC1);//���ع�����ת����� ����ȡDR�Ĵ�����ֵ
	
	return value;
}

