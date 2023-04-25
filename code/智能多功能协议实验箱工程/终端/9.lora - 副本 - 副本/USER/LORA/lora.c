#include "lora.h"

/*
Lora �ܽŵĳ�ʼ��
Tx -- USART4_RX -- PC10(���� 4 ����)
Rx -- USART4_TX -- PC11(���� 4 ����)
M0 -- SPI_CS -- PA8 (ͨ���������ģʽ)
M1 -- SPI_SCK -- PA11 (ͨ���������ģʽ)
AUX - SPI_MISO -- PA12 (��������ģʽ)
*/


uint8_t usart4_buf[128]={0};
u8 Rx_OK = 0;

void Init_USART4_Lora(uint32_t brr)
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

void Send_data_Usart4(uint8_t c)
{
	 while(!(UART4->SR & (0x01<<7)));
	 UART4->DR = c;
}
//lora��������
void Send_Data_Lora(uint8_t *data ,uint8_t size)
{
	 uint8_t i=0;
	 for(i=0;i<size;i++)
	 {
	 Send_data_Usart4(*(data+i));
	 }
}

void Send_String_lora(uint8_t *data)
{
	 while(*data !=0)
	 {
	 Send_data_Usart4(*(data));
	 data+=1;
	 }
}

void Init_Lora(void)
{
 //1:��Ҫ���� Lora �Ĺ���ģʽ
 //��Ҫ�� M0 �� M1 ��״̬
 //���� M0 �� M1 Ϊ���졢AUX Ϊ��������
/* PC10 uart4-tx
 PC11 uart4-rx
 M0 --CS--PA8
 M1 --SCK--PA11
 AUX --SIP1-MISO-->PA12
*/
	 u8 RxCount = 0;
	 u8 rx_buf[6];
	 GPIO_InitTypeDef GPIO_InitTStruct;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	 
	 GPIO_InitTStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;
	 GPIO_InitTStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	 GPIO_InitTStruct.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA,&GPIO_InitTStruct);
	 GPIO_InitTStruct.GPIO_Pin=GPIO_Pin_12;
	 GPIO_InitTStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA,&GPIO_InitTStruct);
	 
	 //1:���� Lora �Ĺ���ģʽ
	 Init_USART4_Lora(9600);//Ĭ��ͨ���ٶ��� 9600
	 //һ��ģʽ
	 while(AUX_Sta==RESET);//�ȴ� AUX ��������
	 M0(1);M1(1);//����ģʽ
	 delay_ms(20);//1ms ֮��ͻṤ������ģʽ
	 //Ҫȥ���� ���Ĺ������� ����ð�ģ�����ó�����״̬
	 Send_data_Usart4(0xc0);//���籣��
	 Send_data_Usart4(0xFF);//��ַ��λ
	 Send_data_Usart4(0xFF);//��ַ��λ
	 Send_data_Usart4(0x1A);//9600 �����ʣ�����ż���顢19.2kb
	 Send_data_Usart4(0x17);//
	 Send_data_Usart4(0xC4);//FEC ����򿪡�20dbm��������
	 
	 delay_ms(20);//�ȴ�
	 
	 //��ⷢ�������Ƿ�д��LORA
	 printf("����ָ��:C1 C1 C1\r\n");
	 
	 Send_data_Usart4(0xc1);
	 Send_data_Usart4(0xc1);
	 Send_data_Usart4(0xc1);
	 

	 
	 
	 while(AUX_Sta==RESET);//�ȴ� AUX ��������
	 M0(0);M1(0);//һ��ģʽ
	 delay_ms(20);//1ms ֮��ͻṤ������ģʽ
	 

	 
	 //���ڵ���
	 if(Rx_OK == 1)
	 {
		 printf("��Ӧ���ݣ�\r\n");
		for(u8 i=0;i < 6;i++)
		 {
			printf("%x ",usart4_buf[i]);
		 }
		 printf("\r\n");
		 Rx_OK = 0;
		 memset(usart4_buf,0,sizeof(usart4_buf));//�������
	 }
	 else
	 {
		memset(usart4_buf,0,sizeof(usart4_buf));//�������
	 }
	 
	 
}

void UART4_IRQHandler(void)
{
 uint8_t tmp;
 static uint8_t i=0;
 uint8_t count;
 //��Ҫȥ�жϷ���������һ���ж�
 if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
 {
 //�����жϵĵ�һ���¾�������ж�
	 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	 tmp = UART4->DR;
	 usart4_buf[i++]=tmp;
	 //i++ :i ���Ȳ����������Լ�
	 //++i ��i ���Լ��ٲ�������
	 
	}
	 //�����ж�//ֻ�е��������ȫ���������Ż��ȥ(�������߿���ʱ����)
 if(USART_GetITStatus(UART4,USART_IT_IDLE)==SET)
	 {
	 tmp=UART4->DR;
	 count = i;
	 i=0;
	 Rx_OK = 1;
//	 printf("%s\r\n",usart4_buf);
//	 memset(usart4_buf,0,sizeof(usart4_buf));//�������
 }

}

