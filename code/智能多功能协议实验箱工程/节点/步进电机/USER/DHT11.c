#include "dht11.h"

u16 tem = 0,hum = 0;

/*******************************
�������ƣ�DHT11_Config
�������ܣ�DHT11 gpio�ڳ�ʼ��
����������flag 	-- 0 ����ģʽ
				        -- 1 ������
��������ֵ����
��ע��
	���ڰ����м�Ĵ������ӿھ���
	PB12
	���ڰ����ұߵĴ������ӿھ���
	PB6
����:ZZXYD
*******************************/
void DHT11_Config(u8 flag)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(DHT11_CLK,ENABLE);
	if(flag == 1)
	{
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else if(flag == 0)
	{
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
	GPIO_Init(DHT11_PORT,&GPIO_InitStruct);
}


/***************************************
�������ƣ�Get_DHT11_Data
�������ܣ���ȡ��ʪ������
����������
	u16 *t--����¶�����
	u16* h--���ʪ������
��������ֵ���ɹ�����0��ʧ�ܷ���-1
��ע��
����:ZZXYD
****************************************/
int Get_DHT11_Data(u16 *t,u16* h)
{
	u8 i = 0,j=0;
	u32 timeout = 0;
	u8 DHT_DATA[5]={0};
	DHT11_Config(1);//���
	DATA(1);//����
	DATA(0);//����
	delay_ms(20);
	DATA(1);
	delay_us(30);
	DHT11_Config(0);//����
	timeout = 0;
	while(DATA_IN()==1)//�ȴ��ź�����
	{
		timeout++;
		if(timeout>0XFFFFF)
			return -1;
	}
	timeout = 0;
	while(DATA_IN()==0)//�ȴ��ź�����
	{
		timeout++;
		if(timeout>0XFFFFF)
			return -1;
	}
	/*��ȡ����*/
	for(i=0;i<5;i++)
	{
		for(j=0;j<8;j++)
		{
			timeout = 0;
			while(DATA_IN()==1)//�ȴ��ź�����
			{
				timeout++;
				if(timeout>0XFFFFF)
					return -1;
			}
			timeout = 0;
			while(DATA_IN()==0)//�ȴ��ź�����
			{
				timeout++;
				if(timeout>0XFFFFF)
					return -1;
			}
			delay_us(40);
			if(DATA_IN()==1)//����Ϊ1
			{	
				DHT_DATA[i]<<=1;
				DHT_DATA[i]|=1;
			}
			else if(DATA_IN()==0)//����Ϊ0
					DHT_DATA[i]<<=1;
		}
	}
	/*����У��*/
	if(DHT_DATA[4]==(DHT_DATA[0]+DHT_DATA[1]+DHT_DATA[2]+DHT_DATA[3]))
	{
		*t = DHT_DATA[2];
		*h = DHT_DATA[0];
		return 0;
	}
	return -1;
}

/***************************************
�������ƣ�DHT11_Show_Usart
�������ܣ���ȡ��ʪ�����ݲ���ͨ���������
����������	
			u16 *t-->����¶�
			u16 *h-->���ʪ��
��������ֵ���ɹ�����0��ʧ�ܷ���-1
��ע��
����:ZZXYD
****************************************/
int DHT11_Show_Usart(u16 *t,u16 *h)
{
	if(Get_DHT11_Data(&tem,&hum))
	{
		printf("��ȡ��ʪ������ʧ��\r\n");
		return -1;
	}
	else
	{
		printf("��ȡ��ʪ�����ݳɹ�\r\n");
		printf("�¶�:%d��\r\nʪ��:%d%%RH\r\n",tem,hum);
		if(t != NULL) *t = tem;
		if(h != NULL) *h = hum;
		return 0;
	}
}




	
