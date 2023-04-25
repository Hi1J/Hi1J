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
 USART_ITConfig(UART4,USART_IT_IDLE,ENABLE);
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

//void UART4_IRQHandler(void)
//{
// uint8_t tmp;
// static uint8_t i=0;
// static u8 flag = 0;//������ȷ��־λ
// //��Ҫȥ�жϷ���������һ���ж�
// if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
// {
// //�����жϵĵ�һ���¾�������ж�
//	 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
//	 tmp = UART4->DR;
//	 Zigbee_Rx_Buf[i++]=tmp;
//	 
//	 //�ж������Ƿ��Ƿ�������
//	 if(flag == 0)
//	 {
//		 if(Zigbee_Rx_Buf[0] == (Zigbee_Address >> 8))//�жϵ�ַ�߰�λ
//		 {
//				if(i == 2)//
//				{
//					if(Zigbee_Rx_Buf[1] == (Zigbee_Address & 0x00ff))//�жϵ�ַ�Ͱ�λ
//					{
//						flag = 1;//ȷ�������Ƿ���������
//					}
//				}
//		 }
//	 }
//	 
//	}
//	 //�����ж�//ֻ�е��������ȫ���������Ż��ȥ(�������߿���ʱ����)
// if(USART_GetITStatus(UART4,USART_IT_IDLE)==SET)
//	 {
//	 tmp=UART4->DR;
//	 i=0;
//		 if(flag == 1)//���������ȷ
//		 {
//				Zigbee_Rx_OK = 1;//���ձ�־λ��1
//				flag = 0;//������ȷ��־λ����
//		 }
//		 else
//			 {
//				memset(Zigbee_Rx_Buf,0,sizeof(Zigbee_Rx_Buf));
//			 }
//		}

//}
void UART4_IRQHandler(void)
{
 uint8_t tmp;
 static uint8_t i=0;
 static u8 flag = 1;//������ȷ��־λ
 //��Ҫȥ�жϷ���������һ���ж�
 if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
 {
 //�����жϵĵ�һ���¾�������ж�
	 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	 
	 if(flag == 1)//���������ȷ�ŷ���������� ��ֹ���������������
	 {
			tmp = UART4->DR;
			Zigbee_Rx_Buf[i++]=tmp;
	 }
	 
	 //�ж������Ƿ�����ȷZigbee������
	 if(Zigbee_Rx_Buf[0] == 0xFA)//�����ʼλ��ȷ ������ȷZigbee������
	 {
		if(i == 2)//�ж��Ƿ��Ƿ���������
		{
			if(!(Zigbee_Rx_Buf[1] == Zigbee_Address))//������Ƿ���������
			{
				flag = 0;//������Ƿ��������� �򲻽��մ�����֡
			}
		}
	 }
	 else  //���������ȷZigbee������
	 {
			flag = 0;//���������ȷZigbee������ �򲻽��մ�����֡
	 }
	
	}
	 //�����ж�//һ֡���ݽ�����ɺ� ������ȷ��� ���ڴ˺������жϴ���
 if(USART_GetITStatus(UART4,USART_IT_IDLE)==SET)
	 {
	 tmp=UART4->DR;
	 i=0;
		 if(flag == 1)//���������ȷ��ȷ������ȷZigbee���������Ƿ��������ģ�
		 {
				Zigbee_Rx_OK = 1;//���ձ�־λ��1
		 }
		 else//������ݲ���ȷ
			 {
					flag = 1;//һ������֡������� ����������ȷ�� ��flag������1�Խ�����һ����֡
					memset(Zigbee_Rx_Buf,0,sizeof(Zigbee_Rx_Buf));//�������
			 }
		}

}//Zigbee��ʼ��
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
			if(Zigbee_Rx_Buf[2] == 0x03)//�������������ʪ��ǿ��
			{
				
				//д������
				sprintf(Tembuf,"%2d",Zigbee_Rx_Buf[3]);
				sprintf(Humbuf,"%2d",Zigbee_Rx_Buf[4]);
				sprintf(Lxbuf,"%-5d",(Zigbee_Rx_Buf[5]<<8) | Zigbee_Rx_Buf[6]);
				
				//LCD��ʾ����
				LCD_ShowStr(97,160+9,(u8 *)Tembuf);
				LCD_ShowStr(97,220+9,(u8 *)Humbuf);
				LCD_ShowStr(97,280+9,(u8 *)Lxbuf);
			
			}
			else if(Zigbee_Rx_Buf[2] == 0x04)
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


