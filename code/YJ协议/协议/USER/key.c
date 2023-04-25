#include "key.h"


void KEY_Indenpendent_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4 | KEY5 | KEY6 | KEY7 | KEY8;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	


}

//����ɨ��
u8 Key_GetValue(void)
{
	u8 KeyValue = 0;
	u8 PinValue = 0;
	
	PinValue = GPIO_ReadInputData(GPIOA);//��ȡGPIOA״̬
	
	PinValue = PinValue & 0xFF;//��ȡĳ��ֵ
	
	if(PinValue)//����а�������
	{
		delay_ms(15);//��ʱ15ms
		
		PinValue = GPIO_ReadInputData(GPIOA);
	
		PinValue = PinValue & 0xFF;//��ȡĳ��ֵ
		
		if(PinValue)//�ٴ��ж�
		{
			for(KeyValue = 0;KeyValue < 8;KeyValue++)
			{
				if(PinValue & (1 << KeyValue))//�ж����ĸ�ֵ
				{
					KeyValue = KeyValue + 1; //����ֵ��1
					break;//����ѭ��
				}
			
			}
			do{//�ж��Ƿ����
				PinValue = GPIO_ReadInputData(GPIOA);
				PinValue = PinValue & 0xFF;
			}while(PinValue);//���û���� ������ȴ�
		}
	
	}
	return KeyValue;//�����ĸ�����
	
}