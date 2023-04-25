#include "usart.h"

u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u16 USART1_RX_STA=0;       //����״̬���	




//���ڳ�ʼ��
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	// �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// �򿪴��������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = General_BaudRate;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART1, &USART_InitStructure);
	// �����ж����ȼ�����
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
#if USE_USART_DMA_RX 
	// ���� ���ڿ���IDEL �ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  
  // ��������DMA����
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); 
	/* ʹ�ܴ���DMA */
	USARTx_DMA_Rx_Config();
#else
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
#endif
 
#if USE_USART_DMA_TX 
	// ��������DMA����
//	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE); 
	USARTx_DMA_Tx_Config();
#endif
 
	// ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);	    
}




//�ض���printf����
int fputc(int ch,FILE *fp)
{
	while((USART1 -> SR & ( 1 << 7 )) == 0);//���SR�Ĵ����еĵ���λTCΪ0����ʾ����δ������ɣ����һֱ�ڴ˵ȴ�
	USART1 -> DR = ch;//���������ɣ�������ݷ���DR�Ĵ�����
	return ch;
}




#if USE_USART_DMA_RX
void Receive_DataPack(void)
{
	/* ���յ����ݳ��� */
	uint32_t buff_length;
	
	/* �ر�DMA ����ֹ���� */
	DMA_Cmd(DMA1_Channel5, DISABLE);  /* ��ʱ�ر�dma��������δ���� */ 
	
	/* ��DMA��־λ */
	DMA_ClearFlag( DMA1_FLAG_TC5 );  
	
	/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
	buff_length = USART1_REC_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);
  
    /* ��ȡ���ݳ��� */
    USART1_RX_STA = buff_length;
 
	printf("buff_length = %d\n ",buff_length);
	
	/* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
	DMA1_Channel5->CNDTR = USART1_REC_LEN;    
  
	/* �˴�Ӧ���ڴ����������ٴ򿪣����� DataPack_Process() ��*/
	DMA_Cmd(DMA1_Channel5, ENABLE);      
	
	/* (OS)�����ź� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ */
	
    /* ��ǽ�����ɣ��� DataPack_Handle ����*/
    USART1_RX_STA |= 0xC000;
  
    /* 
    DMA �������ȴ����ݡ�ע�⣬����жϷ�������֡�����ʺܿ죬MCU����������˴ν��յ������ݣ�
    �ж��ַ������ݵĻ������ﲻ�ܿ������������ݻᱻ���ǡ���2�ַ�ʽ�����
 
    1. �����¿�������DMAͨ��֮ǰ����Rx_Buf��������������ݸ��Ƶ�����һ�������У�
    Ȼ���ٿ���DMA��Ȼ�����ϴ����Ƴ��������ݡ�
 
    2. ����˫���壬��������DMA_MemoryBaseAddr�Ļ�������ַ����ô�´ν��յ������ݾͻ�
    ���浽�µĻ������У������ڱ����ǡ�
	*/
}
#endif



void USART1_IRQHandler(void)
{
#if USE_USART_DMA_RX
	/* ʹ�ô���DMA */
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)
	{		
		/* �������� */
		Receive_DataPack();
		// ��������жϱ�־λ
		USART_ReceiveData(USART1);
	}	
#else
  /* �����ж� */
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
    Receive_DataPack();
	}
#endif
}






