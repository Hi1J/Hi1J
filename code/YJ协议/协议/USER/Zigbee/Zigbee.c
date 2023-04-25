#include "Zigbee.h"

#if Zigbee_Open

u8 Zigbee_Rx_Buf[128] = {0};
u8 Zigbee_Rx_OK = 0;

void Uart4_Init(u32 brr)
{
	GPIO_InitTypeDef GPIO_InitStruct;
 USART_InitTypeDef USART_InitStruct;
 NVIC_InitTypeDef NVIC_InitStruct;
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;//����
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
 GPIO_Init(GPIOC,&GPIO_InitStruct);
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;//����
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOC,&GPIO_InitStruct);
 
 //3:���� USART4
 USART_InitStruct.USART_BaudRate=brr;//������
 
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//û��Ӳ������
 //RS232 ��Ӳ�����ſ��ƽ��պͷ���
 USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
 //TX(����) RX�����գ� TX|RX(���պͷ���)
 USART_InitStruct.USART_Parity=USART_Parity_No;//û��У��λ
 USART_InitStruct.USART_StopBits=USART_StopBits_1;
 USART_InitStruct.USART_WordLength=USART_WordLength_8b;
 USART_Init(UART4,&USART_InitStruct);
/********************************************************************
*****/
//���ж�
 //1����������ж�
 USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
 //2�������жϵ����ȼ�
 NVIC_InitStruct.NVIC_IRQChannel=UART4_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
 NVIC_Init(&NVIC_InitStruct);
 //3:д�жϷ�����
 //�жϷ����������� ������

 //�ж��¼�������Ȼ��ִ��
/*****************************************************************8*/ 
 //4��ʹ�ܴ���
 USART_Cmd(UART4,ENABLE);

}
void Uart4_Send_Data(u8 data)
{
	while(!(UART4->SR & (1 << 7)));
	UART4->DR = data;
}

//UART4�жϷ�����
void UART4_IRQHandler(void)
{
	uint8_t tmp;//������ʱ�洢��
	static uint8_t YJ_Rx_Count=1;//����ֵ ��8��0
	static uint8_t YJ_Rx_head = 0;//��ʼλ���ջ�����
	static uint8_t YJ_Rx_flag = 0;//��ʼ���ձ�־λ
 //�����ж�
 if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
 {
 //�����жϵĵ�һ���¾�������ж�
	 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	 
	 tmp = UART4->DR;//��ʱ��¼���յ�ֵ
	 
	 //������׸�������Ϊ��ʼλ ���������ʼλ���ջ�����
	 if((tmp == 0xfa) && (YJ_Rx_flag == 0))YJ_Rx_head = tmp;

	 //����ڶ���������Ϊ��ַλ 
	 if((tmp == Zigbee_Address) && (YJ_Rx_head == 0xfa) && (YJ_Rx_flag == 0))
	 {
			YJ_Rx_flag = 1;//���ձ�־λ��1
			if(Zigbee_Rx_OK == 1)P_RX = P_RX->next;//���������������֡ �����ݱ��浽��һ��buf
			else P_RX->next = YJ_Link1;//����ָ��buf1
			P_RX->YJ_RX_Buf[0] = YJ_Rx_head;//����ʼλ����buf[0]����
//			Zigbee_Rx_Buf[0] = YJ_Rx_head;//����ʼλ����buf[0]����
			YJ_Rx_head = 0;//��ʼλ���ջ�������0
	 }

	//������ձ�־λΪ1 ��ʼ����
	 if(YJ_Rx_flag == 1)P_RX->YJ_RX_Buf[YJ_Rx_Count++] = tmp;
	 
	 //������һ��bufʱ Ĭ��һ֡���ݽ������ ����־λ��λ �Խ�����һ֡����
	 if(YJ_Rx_Count == 8)
	 {
		 YJ_Rx_flag = 0;//���տ�ʼλ��0
		 YJ_Rx_Count = 1;//����ֵ��λ
		 YJ_Rx_head = 0;//��ʼλ��������λ ��ֹ����
		 Zigbee_Rx_OK = 1;//������8�����ݺ� ���ձ�־λ��1
		 for(u8 i=0;i<8;i++)
		 {
			(P_RX->next)->YJ_RX_Buf[i] = P_RX->YJ_RX_Buf[i];
		 }
		 
	 }

	
	}

 
	

}
//Zigbee��ʼ��
void Zigbee_Init(void)
{
	Uart4_Init(115200);

}

void Zigbee_Send_Data(u8 *data,u16 len)
{
	u8 i = 0;
	for(i=0;i<len;i++)
	{
		Uart4_Send_Data(*(data+i));
	}

}
/*
Zigbee���ݷ�������

Zigbee����֡��ʽ�淶��
-------------------------------------------------------------------------------------------
|	    1    |   /     2               3         		  4                5          6-n
| ��ַ��λ / Ŀ���ַ��λ     ���͵�ַ��λ     ���͵�ַ��λ        ������     ��������


�����룺
0x03:��ʪ���նȷ���
0x04:���Ƶ�����

�������ݣ�
0x03:
5:�¶�
6:ʪ��
7:���ո�λ
8:���յ�λ

0x04:


*/
void Zigbee_Data_Analysis(void)
{
	if(Zigbee_Rx_OK == 1)//������յ���������������
		{
			if(Zigbee_Rx_Buf[3] == 0x03)//�������������ʪ��ǿ��
			{
				
				//д������
				sprintf(Tembuf,"%2d",Zigbee_Rx_Buf[4]);
				sprintf(Humbuf,"%2d",Zigbee_Rx_Buf[5]);
				sprintf(Lxbuf,"%-5d",(Zigbee_Rx_Buf[6]<<8) | Zigbee_Rx_Buf[7]);
				
				//LCD��ʾ����
				LCD_ShowStr(97,160+9,(u8 *)Tembuf);
				LCD_ShowStr(97,220+9,(u8 *)Humbuf);
				LCD_ShowStr(97,280+9,(u8 *)Lxbuf);
			
			}
			else if(Zigbee_Rx_Buf[3] == 0x04)
			{
				
			}
			else
			{
				
			}
			
			Zigbee_Rx_OK = 0;//��ձ�־λ
			memset(Zigbee_Rx_Buf,0,sizeof(Zigbee_Rx_Buf));//��ս�������
		}

}

#endif


