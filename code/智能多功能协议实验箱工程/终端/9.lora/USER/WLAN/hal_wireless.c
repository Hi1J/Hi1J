#include "hal_wireless.h"
//#include "task.h"

WIRELESS_RX_TX wireless_data = {.rx_over = 0,.rx_count = 0,.tx_count = 0};
/*
UART4_TX -- PC10
UART4_RX -- PC11
*/
void UART4_Config(u32 boad)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	//1.��c�˿�ʱ�Ӻ�UART2�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	//2.Pc10����Ϊ�����������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//ͨ���������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	//3.Pc11����Ϊ��������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	//4.����USART4:RS232Э�� = 1��ʼλ+8������λ+0����żУ��λ + 1��ֹͣλ
	
	USART_InitStruct.USART_BaudRate = boad;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ����
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ȫ˫��ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//1+8 �ֳ�
	USART_Init(UART4, &USART_InitStruct);
	//�����жϵ�����
	UART4_NVICConfig();
	//5.ʹ�ܴ���4
	USART_Cmd(UART4,ENABLE);
}
void UART4_NVICConfig(void)
{
	//����Ϊ���ڽ����ж�
	USART_ITConfig(UART4, USART_IT_RXNE,ENABLE);
	//�������ȼ��Լ�����ʹ��
	NVIC_SetPriority(UART4_IRQn,0);
	NVIC_EnableIRQ(UART4_IRQn);
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
	TIM_Cmd(TIM4,DISABLE);
}

/****�������ݵĽ����Լ��ж��Ƿ������ɴ���*****/
//�����ж�
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE))
	{
		//������жϱ�־λ
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		//��������
//		if(wireless_data.rx_over == 0)		//ע�� zigbeeʱʹ���ˣ�Loraû�м���仰
		if(wireless_data.rx_over == 0){
			
			if(wireless_data.rx_count == 0)//��һ��֡���ݵĵ�һ�����ݵ���ʱ ����TIM4
			{
				TIM_SetCounter(TIM4,0);
				TIM_Cmd(TIM4,ENABLE);
			}
			wireless_data.rx_buff[wireless_data.rx_count++] = UART4->DR;
		}
			
		//������ʱ�������������
		
		
	}
}

//��ʱ��4������ж�
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//����жϱ�־λ
		TIM_Cmd(TIM4,DISABLE);//ʧ�ܶ�ʱ��4
//		wireless_data.rx_over = 1;//������ɱ�־��1
		
		if(wireless_data.rx_count > 1)
		{
			wireless_data.rx_over = 1;//������ɱ�־��1
		}
		else //�������ݣ��˳���
		{
			wireless_data.rx_count = 0;
		}
	}
}


