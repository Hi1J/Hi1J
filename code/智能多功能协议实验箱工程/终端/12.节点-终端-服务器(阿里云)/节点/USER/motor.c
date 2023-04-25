#include "motor.h"

//���洰����״̬
unsigned char curtain_state = CURTAIN_OPEN;
//����  
const unsigned int eighttb[8][4]=
{
	{0,1,1,0},
	{0,1,0,0},
	{0,1,0,1},
	{0,0,0,1},
	{1,0,0,1},
	{1,0,0,0},
	{1,0,1,0},
	{0,0,1,0}
};

/*****************************************************
�������ƣ�Motor_Gpio_Init
�������ܣ�˿�˵��ʹ�õ�GPIO��ʼ��
������������
��������ֵ����
��ע��
	���ڰ����м�Ĵ������ӿھ���
	PB12---B+
	PB13---A+
	PB1----B-
	PA8----A-
	���ڰ����ұߵĴ������ӿھ���
	PB6----B+
	PB7----A+
	PA0----B-
	PB5----A-
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
void Motor_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(A1_CLK|A0_CLK|B1_CLK|B0_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_InitStruct.GPIO_Pin = A1_PIN;
	GPIO_Init(A1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = A0_PIN;
	GPIO_Init(A0_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = B1_PIN;
	GPIO_Init(B1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = B0_PIN;
	GPIO_Init(B0_PORT,&GPIO_InitStruct);
	
	//ȫ������
	A1_OPT(0);A0_OPT(0);B1_OPT(0);B0_OPT(0);
	
}

/*****************************************************
�������ƣ�Motorcw
�������ܣ�����˿�˵��ת��
����������int anti
		anti == 0---����ת
		anti == 1---����ת
��������ֵ����
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
u8 Motorcw(unsigned char anti)  
{  
	
  int i; 	
	int pos = 0;
	for(i=0;i<8;i++)
	{
		switch(anti)
		{
			case 0:pos = i;break;
			case 1:pos = 7 - i;break;
			default:return 0;
		}
		A1_OPT(eighttb[pos][0]);
		A0_OPT(eighttb[pos][1]);
		B1_OPT(eighttb[pos][2]);
		B0_OPT(eighttb[pos][3]);
		delay_ms(10);
	}
	return 1;
	
	
}  



