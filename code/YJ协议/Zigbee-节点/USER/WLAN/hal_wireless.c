#include "hal_wireless.h"

#if Lora_Open

uint8_t Slave_ID = 0;
WIRELESS_RX_TX wireless_data = {.rx_over = 0,.rx_count = 0,.tx_count = 0};
//���豸����ID��
//uint8_t Slave_GetID(void)
//{
//  //��ƷΨһ��ݱ�ʶ�Ĵ��� ����ַ�� 0x1FFF F7E8
//  uint8_t *ID96 = (uint8_t *)0x1FFFF7E8;
//	Slave_ID = CRC_8_Tab(ID96,12);//96λID��Ϊ8λID��
//	printf("******�ӻ�����ID��%d******\r\n",Slave_ID);
//  return Slave_ID;
//}
/*
USART3_TX -- PB10
USART3_RX -- PB11
*/
void USART3_Config(u32 boad)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	//1.��A�˿�ʱ�Ӻ�UART2�˿�ʱ��
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
	
	USART_InitStruct.USART_BaudRate = boad;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ����
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ȫ˫��ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//1+8 �ֳ�
	USART_Init(USART3,&USART_InitStruct);
	//�����жϵ�����
	USART3_NVICConfig();
	//5.ʹ�ܴ���3
	USART_Cmd(USART3,ENABLE);
}
void USART3_NVICConfig(void)
{
	//����Ϊ���ڽ����ж�
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	//�������ȼ��Լ�����ʹ��
	NVIC_SetPriority(USART3_IRQn,0);
	NVIC_EnableIRQ(USART3_IRQn);
}
/*
���ڷ��ͺ���
puart -- USART1 2 3 UART4 5
data -- �����׵�ַ
len  -- Ҫ�������ݵĳ���
*/
void UART_SendData(USART_TypeDef *puart,u8 *data,u16 len)
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
		while(USART_GetFlagStatus(puart,USART_FLAG_TC) != SET);
		puart->DR=data[i];
	}
	while(USART_GetFlagStatus(puart,USART_FLAG_TC) != SET);
}
void Time4_Config(u16 psc,u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//�����ж�
	NVIC_SetPriority(TIM4_IRQn,3);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM4->EGR |= (1<<0);
	//ʧ�ܶ�ʱ��4
	TIM_Cmd(TIM4,ENABLE);
}

/****�������ݵĽ����Լ��ж��Ƿ������ɴ���*****/
//�����ж�
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE))
	{
		//������жϱ�־λ
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		//��������
//		if(wireless_data.rx_over == 0)		//ע�� zigbeeʱʹ���ˣ�Loraû�м���仰
			wireless_data.rx_buff[wireless_data.rx_count++] = USART3->DR;
		//������ʱ�������������
		TIM_SetCounter(TIM4,0);
		TIM_Cmd(TIM4,ENABLE);
	}
}

//��ʱ��4������ж�
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//����жϱ�־λ
		TIM_Cmd(TIM4,DISABLE);//ʧ�ܶ�ʱ��4
		if(wireless_data.rx_count > 1)
		{
			wireless_data.rx_over = 1;//������ɱ�־��1
		}
	}
}

#endif

