#include "lora.h"


#define LORA_SETPRAM_TIME 100   //�ȴ�Loraģ���ڲ��������
u16 lora_addr = 0xffff;//lora��ַ

LORA_WORKDATA lora_data;
char *air_sped[] = {"0.3","1.2","2.4","4.8","9.6","19.2","19.2","19.2"};
char *baudrate[] = {"1200","2400","4800","9600","19200","38400","57600","115200"};
char *uartmode[] = {"8N1","801","8E1","8N1"};
char *Tran_power[] = {"20","17","14","10"};

/*
Lora�ܽŵĳ�ʼ��
Tx -- USART4_RX -- PC10(����4����)
Rx -- USART4_TX -- PC11(����4����)
M0 -- SPI_CS    -- PA8 (ͨ���������ģʽ)
M1 -- SPI_SCK   -- PA11 (ͨ���������ģʽ)
AUX - SPI_MISO  -- PA12 (��������ģʽ)
*/
void Lora_Config(void)
{
	//��عܽų�ʼ��
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(LORA_M0_CLK|LORA_M1_CLK|LORA_AUX_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = LORA_M0_PIN;
	GPIO_Init(LORA_M0_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LORA_M1_PIN;
	GPIO_Init(LORA_M1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = LORA_AUX_PIN;
	GPIO_Init(LORA_AUX_PORT,&GPIO_InitStruct);	
	
	UART4_Config(9600);//����3��ʼ��+�����жϳ�ʼ��
	Time4_Config(7200,200);//��ʱ��4��ʼ��
	
	Lora_SetWorkParameters();//����Loraģ��Ĳ���
	Lora_CurrentPram();//��ȡLoraģ��Ĳ���
}
void Lora_SendData(u8 *data,u16 len)
{
	uint8_t buff[256] = {0xff,0xff,PRM_CHANNEL_DEFAULT};
	for(u8 i = 0;i<len;i++)
		buff[i+3] = data[i];
	UART_SendData(UART4, buff, len+3);
}
void Lora_SendCmd(u8 *data,u16 len)
{
	UART_SendData(UART4, data, len);
}
//����Lora��ģʽ
void Lora_SetMode(u8 mode)
{
	if((mode != LORA_MODE_PATTERN) && (mode != LORA_MODE_WARKUP) && (mode != LORA_MODE_POWERSAVE) && (mode != LORA_MODE_SLEEP))  return ;
	GPIO_WriteBit(LORA_M0_PORT,LORA_M0_PIN,(BitAction)(mode & 0x1));
	GPIO_WriteBit(LORA_M1_PORT,LORA_M1_PIN,(BitAction)((mode>>4) & 0x1));
	delay_ms(100);
}
//Loraģ�鸴λ
void Lora_Reset(void)
{
	u8 tx_buff[] = {0xC4,0xC4,0xC4};
	Lora_SetMode(LORA_MODE_SLEEP);//��������ģʽ
	Lora_SendCmd(tx_buff,sizeof(tx_buff));//������������
}
//��ȡ��ǰ����
void Lora_CurrentPram(void)
{
	uint32_t timeout = 0x00ffffff;
	u8 RxBuff[6] = {0};
	u8 RxCount = 0;
	u8 tx_buff[] = {0xC1,0xC1,0xC1};
		//�رմ���3�ж�
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
//	NVIC_DisableIRQ(UART4_IRQn);
	
	Lora_SetMode(LORA_MODE_SLEEP);//��������ģʽ
	Lora_SendCmd(tx_buff,sizeof(tx_buff));//���ͻ�ȡ��������
	memset(lora_data.buff,0,6);
	//���ڵȴ���ȡ����
	do{
		if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE) == SET)
		{
			lora_data.buff[RxCount++] = UART4->DR;
		}
		if(RxCount == 6) break;//���������������˳�
	}while(timeout--);
	
	printf("\r\n���յ�������Ϊ��\r\n");
	for(u8 i = 0;i<6;i++)
			printf("%x ",lora_data.buff[i]);
	printf("\r\n");
	
	if(RxCount != 6) 
		printf("Loraģ���ȡ������ʱ��\r\n");
	else//����Lora�Ĳ���
		Lora_WorkParameters();
	Lora_SetMode(LORA_MODE_PATTERN);//����һ��ģʽ
	//���ж�
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
//	NVIC_EnableIRQ(UART4_IRQn);
}


void Lora_SetWorkParameters(void)
{
	//�رմ���4�ж�
	USART_ITConfig(UART4,USART_IT_RXNE,DISABLE);
//	NVIC_DisableIRQ(UART4_IRQn);
	
	Lora_SetMode(LORA_MODE_SLEEP);//��������ģʽ

	memset(lora_data.buff,0,sizeof(lora_data.buff));
	//�޸ĵ�ַ���ŵ�
  lora_data.lora_workprm.head = CMD_STOREPRM;
	lora_data.lora_workprm.addh = (lora_addr>>8) & 0xff;
	lora_data.lora_workprm.addl = lora_addr & 0xff;
	lora_data.lora_workprm.sped = (PRM_UART_Parity_8N1<<6)|(PRM_UART_BaudRate_9600<<3)|(PRM_Sky_Speed_2_4);
	lora_data.lora_workprm.chan = PRM_CHANNEL_DEFAULT;
	lora_data.lora_workprm.option = ((PRM_TRAN_MODE_FP<<7)|(PRM_IO_Driver_PP<<6)|
											             (PRM_WarkUp_Time_250<<3)|(PRM_FEC_OPEN<<2)|(PRM_EmissionPower_20));
  
	printf("д�������Ϊ��\r\n");
	for(u8 i = 0;i<6;i++)
		printf("%x ",lora_data.buff[i]);
	
	Lora_SendCmd(lora_data.buff,sizeof(lora_data.buff));

	delay_ms(2);//��ʱ�ȴ�Lora�ڲ��������
	Lora_SetMode(LORA_MODE_PATTERN);//����һ��ģʽ

	//���ж�
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);

//	NVIC_EnableIRQ(UART4_IRQn);
}

void Lora_WorkParameters(void)
{
	//��ӡԭʼ����
	printf("ԭʼ���ݣ�");
	for(u8 i = 0;i<6;i++)
		printf("%x ",lora_data.buff[i]);
	printf("\r\n");
	//��������
	printf("Ƶ��\t��ַ\t�ŵ�\t��������\t������\t���ڸ�ʽ\t���书��\r\n");
	printf("%dMHz\t",lora_data.lora_workprm.chan+410);//����Ƶ��
	printf("0x%x\t",((lora_data.lora_workprm.addh<<8) + lora_data.lora_workprm.addl));//��ַ
	printf("0x%x\t",lora_data.lora_workprm.chan);//�ŵ�
	printf("%skbps\t",air_sped[(lora_data.lora_workprm.sped & 0x7)]);//��������
	printf("%s\t",baudrate[((lora_data.lora_workprm.sped>>3)& 0x7)]);//������
	printf("%s\t",uartmode[(lora_data.lora_workprm.sped>>6) & 0x3 ]);//���ڸ�ʽ
	printf("%sdBm\t",Tran_power[lora_data.lora_workprm.option & 0x3]);//���书��
  printf("\r\n");
}


