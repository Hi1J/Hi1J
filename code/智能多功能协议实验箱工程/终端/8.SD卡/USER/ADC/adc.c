#include "adc.h"


//PB1 -- ADC1 CH9
void ADC1_CH9_Init(void)
{
	//ʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1,ENABLE);
	
	//GPIOB����
	GPIO_InitTypeDef x;
	x.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	x.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB,&x);
	
	//ADC1����
	ADC_InitTypeDef y;
	y.ADC_ContinuousConvMode = DISABLE;//����ģʽ
	y.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
	y.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�������
	y.ADC_Mode = ADC_Mode_Independent;//��������
	y.ADC_NbrOfChannel = 1;//���й���ת����ADC ͨ����Ϊ1
	y.ADC_ScanConvMode = DISABLE;//����ģʽ����ɨ�裩
	ADC_Init(ADC1,&y);
	
	/*����ADC1*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,1,ADC_SampleTime_239Cycles5);//ADC1 , ͨ��9 ���������˳��1 
	
	//ʹ��ADC1
	ADC_Cmd(ADC1,ENABLE);
}

u16 MQ_2_GetValue(void)
{
	u16 value;
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�������
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�SRӲ����1
	
	value = ADC_GetConversionValue(ADC1);//���ع�����ת����� ����ȡDR�Ĵ�����ֵ
	
	return value;


}




