#include "mq2.h"


u16 mq2_val = 0;
/*****************************************************
�������ƣ�MQ2_Gpio_Init
�������ܣ�MQ2������ʹ�õ�GPIO��ʼ��
������������
��������ֵ����
��ע��
	���ڰ����м�Ĵ������ӿھ���
	PB1-----ADC12_IN9
	���ڰ����ұߵĴ������ӿھ���
	PA0-----ADC12_IN0
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/		

void MQ2_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(MQ2_CLK|RCC_APB2Periph_ADC1,ENABLE);
	
	//GPIO������Ϊģ��ģʽ
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = MQ2_PIN;
	GPIO_Init(MQ2_PORT,&GPIO_InitStruct);
	//ADC1������
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//ADC����ģʽ
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//����ѭ��ģʽ��������ѭ��ģʽ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;     //��ͨ�����ǵ�ͨ��
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//���¼����������������
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ��� -- ��12λ
	ADC_InitStruct.ADC_NbrOfChannel = 1;//˳��ת���Ĺ������ͨ����Ŀ
	
	ADC_Init(ADC1,&ADC_InitStruct);     //����ؼĴ�����ֵ
	
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1,MQ2_ADC_CHANNEL,1,ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1,ENABLE);               //����ADC1
	
	ADC_ResetCalibration(ADC1);         //ʹ�ø�λУ׼�����ǳ�ʼ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	ADC_StartCalibration(ADC1);         //A/DУ׼
	while(ADC_GetCalibrationStatus(ADC1) == SET);
}

/*****************************************************
�������ƣ�MQ2_GetValue
�������ܣ���ȡMQ2����������
������������ȡ����ADC����
��������ֵ���ɹ�����0��ʧ�ܷ���-1
��ע��
	���ڰ����м�Ĵ������ӿھ���
	PB1-----ADC12_IN9
	���ڰ����ұߵĴ������ӿھ���
	PA0-----ADC12_IN0
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/	
int MQ2_GetValue(u16 *adc_val)
{
	u32 timeout = 0;
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//����ת��
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET)
	{
		timeout++;
		if(timeout>0xffff)
			return -1;
	}
	*adc_val = ADC_GetConversionValue(ADC1);
	return 0;
}

/*****************************************************
�������ƣ�MQ2_Show_Usart
�������ܣ���ȡMQ2���������ݲ����ڴ�����ʾ
������������
��������ֵ��ʧ�ܷ���-1���ɹ�����MQ2Ũ��ֵ
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/	
u16 MQ2_Show_Usart(void)
{
	if(MQ2_GetValue(&mq2_val))
	{
		printf("MQ2���������ݶ�ȡʧ��\r\n");
		return (u16)-1;
	}
	else
	{
		printf("MQ2���������ݶ�ȡ�ɹ�\r\n");
		printf("adc_val:%d\r\n",mq2_val);
		return mq2_val;
	}
}



