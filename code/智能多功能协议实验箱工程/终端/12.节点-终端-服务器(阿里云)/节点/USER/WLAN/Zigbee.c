#include "Zigbee.h"

#if Zigbee_Open

u8 Zigbee_Rx_Buf[128] = {0};
u8 Zigbee_Rx_OK = 0;

void Usart3_Init(u32 brr)
{
	GPIO_InitTypeDef GPIO_InitStruct;
 USART_InitTypeDef USART_InitStruct;
 NVIC_InitTypeDef NVIC_InitStruct;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

	//2.PB10����Ϊ�����������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//ͨ���������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//3.PB11����Ϊ��������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//4.����USART2:RS232Э�� = 1��ʼλ+8������λ+0����żУ��λ + 1��ֹͣλ
	
	USART_InitStruct.USART_BaudRate = brr;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ����
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ȫ˫��ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//1+8 �ֳ�
	USART_Init(USART3,&USART_InitStruct);
/********************************************************************
*****/
//���ж�
 //1����������ж�
 USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
 USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
 //2�������жϵ����ȼ�
 NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
 NVIC_Init(&NVIC_InitStruct);
 //3:д�жϷ�����
 //�жϷ����������� ������

 //�ж��¼�������Ȼ��ִ��
/*****************************************************************8*/ 
 //4��ʹ�ܴ���
 USART_Cmd(USART3,ENABLE);

}
void Usart3_Send_Data(u8 data)
{
	while(!(USART3->SR & (1 << 7)));
	USART3->DR = data;
}

void UART4_IRQHandler(void)
{
 uint8_t tmp;
 static uint8_t i=0;
 uint8_t count;
 //��Ҫȥ�жϷ���������һ���ж�
 if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
 {
 //�����жϵĵ�һ���¾�������ж�
	 USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	 tmp = USART3->DR;
	 Zigbee_Rx_Buf[i++]=tmp;
	 //i++ :i ���Ȳ����������Լ�
	 //++i ��i ���Լ��ٲ�������
	 
	}
	 //�����ж�//ֻ�е��������ȫ���������Ż��ȥ(�������߿���ʱ����)
 if(USART_GetITStatus(USART3,USART_IT_IDLE)==SET)
	 {
	 tmp=USART3->DR;
	 count = i;
	 i=0;
	 Zigbee_Rx_OK = 1;

 }

}

void Zigbee_Init(void)
{
	Usart3_Init(115200);

}

void Zigbee_Send_Data(u8 *data,u16 len)
{
	u8 i = 0;
	for(i=0;i<len;i++)
	{
		Usart3_Send_Data(*(data+i));
	}

}

#endif


