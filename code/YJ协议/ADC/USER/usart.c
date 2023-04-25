#include "usart.h"

u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[123];

u8 rx_ok;

u16 USART1_RX_STA=0;       //����״̬���	

//���ڳ�ʼ������
void USART1_Config(void)
{
//1.����GPIO��������������������������������������������
	//�ٿ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//������GPIOģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
		//PA9�����ͣ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//�۳�ʼ���Ĵ�����PA9��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		//PA10�����գ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	//�ܳ�ʼ���Ĵ�����PA10��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
//����NVIC
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	
	
//2.���ô��ڡ�������������������������������������������
	//�ٿ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//�����ô���ģʽ
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = General_BaudRate;//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//���÷��ͺͽ��գ�ȫ˫����
	USART_InitStructure.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//һ֡�д�������8������λ
	//�۳�ʼ���Ĵ���
	USART_Init(USART1,&USART_InitStructure);
	//���ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//��ʹ�ܴ���
	USART_Cmd(USART1,ENABLE);
	
	


}

//3.���ݷ��ͺ����ͽ��պ����ı�д������������������������
  //���ض���printf����
int fputc(int ch,FILE *fp)
{
	USART1 -> DR = ch;//���������ɣ�������ݷ���DR�Ĵ�����
	while((USART1 -> SR & ( 1 << 7 )) == 0);//���SR�Ĵ����еĵ���λTCΪ0����ʾ����δ������ɣ����һֱ�ڴ˵ȴ�
	return ch;
}


//����1�жϺ���
//���ղ���������--��ʱ���ж�
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART1->SR & (1 << 5))
	{
		res = USART1->DR;
		printf("%c",res);
		USART1->SR &= ~(1 << 5);//����жϱ�־λ
		
	}
	
}





//***************************************USART2**************************************//

void USART2_Config(void)
{
//1.����GPIO��������������������������������������������
	//�ٿ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//������GPIOģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
		//PA2�����ͣ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//�۳�ʼ���Ĵ�����PA9��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		//PA3�����գ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	//�ܳ�ʼ���Ĵ�����PA10��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//�ж�
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	
	
//2.���ô��ڡ�������������������������������������������
	//�ٿ�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//�����ô���ģʽ
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = General_BaudRate;//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//���÷��ͺͽ��գ�ȫ˫����
	USART_InitStructure.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//һ֡�д�������8������λ
	//�۳�ʼ���Ĵ���
	USART_Init(USART2,&USART_InitStructure);
	//�ܿ��ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//��ʹ�ܴ���
	USART_Cmd(USART2,ENABLE);

}


//����2�����ַ�������
void Usart2_Send_String(uint8_t * str)
{
	while( *str != '\0')
	{
		USART2->DR = *str;
		while(!(USART2->SR & (1 << 7)));//�ȴ��������
		str++;
	}

}

//����2�жϺ���
void USART2_IRQHandler(void)
{
	uint8_t tmp;//������ʱ�洢��
	static uint8_t YJ_Rx_Count=1;//����ֵ ��8��0
	static uint8_t YJ_Rx_head = 0;//��ʼλ���ջ�����
	static uint8_t YJ_Rx_flag = 0;//��ʼ���ձ�־λ
	if(USART2->SR & (1 << 5))
	{
		USART2->SR &= ~(1 << 5); //��ձ�־λ
		
		tmp = UART4->DR;//��ʱ��¼���յ�ֵ
	 
	 //������׸�������Ϊ��ʼλ ���������ʼλ���ջ�����
	 if((tmp == 0xfa) && (YJ_Rx_flag == 0))YJ_Rx_head = tmp;

	 //����ڶ���������Ϊ��ַλ 
	 if((tmp == 0xff) && (YJ_Rx_head == 0xfa) && (YJ_Rx_flag == 0))
	 {
			YJ_Rx_flag = 1;//���ձ�־λ��1
//			if(Zigbee_Rx_OK == 1)P_RX = P_RX->next;//���������������֡ �����ݱ��浽��һ��buf
//			else P_RX->next = YJ_Link1;//����ָ��buf1
			USART2_RX_BUF[0] = YJ_Rx_head;//����ʼλ����buf[0]����
			YJ_Rx_head = 0;//��ʼλ���ջ�������0
	 }

	//������ձ�־λΪ1 ��ʼ����
	 if(YJ_Rx_flag == 1)USART2_RX_BUF[YJ_Rx_Count++] = tmp;
	 
	 //������һ��bufʱ Ĭ��һ֡���ݽ������ ����־λ��λ �Խ�����һ֡����
	 if(YJ_Rx_Count == 8)
	 {
		 YJ_Rx_flag = 0;//���տ�ʼλ��0
		 YJ_Rx_Count = 1;//����ֵ��λ
		 YJ_Rx_head = 0;//��ʼλ��������λ ��ֹ����
		 rx_ok = 1;//������8�����ݺ� ���ձ�־λ��1
//		 for(u8 i=0;i<8;i++)
//		 {
//			(P_RX->next)->YJ_RX_Buf[i] = P_RX->YJ_RX_Buf[i];
//		 }
		 
	 }
	
	}

}

