#include "usart.h"

void usart0_init(uint32_t brr)
{
	//ʱ������
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART0);
	
	//GPIOģʽ����
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);//TX-PA9
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//RX-PA10
	
	//���ڹ���ģʽ����
	usart_deinit(USART0);//��λ����USART
	usart_baudrate_set(USART0,brr);//���ò�����
	usart_word_length_set(USART0,USART_WL_8BIT);//8λ����λ
	usart_stop_bit_set(USART0,USART_STB_1BIT);//1��ֹͣλ
	usart_parity_config(USART0,USART_PM_NONE);//����żУ��
	usart_hardware_flow_cts_config(USART0,USART_CTS_DISABLE);//��Ӳ��������
	usart_hardware_flow_rts_config(USART0,USART_RTS_DISABLE);//��Ӳ��������
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);//����ʹ��
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//����ʹ��
	
	//�ж�����
	nvic_irq_enable(USART0_IRQn,0,0);//�ж�ʹ��
	usart_interrupt_enable(USART0,USART_INT_RBNE);//�����ж�ʹ��
	
	//ʹ�ܴ���
	usart_enable(USART0);
	
}

//����0printf�ض���
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}


//����0�жϷ�����
void USART0_IRQHandler(void)
{
	uint8_t res;
	if(usart_interrupt_flag_get(USART0,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE);
		res = usart_data_receive(USART0);
		printf("%c",res);
		
	}

}



void usart1_init(uint32_t brr)
{
	//ʱ������
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);
	
	//GPIOģʽ����
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);//TX-PA2
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3);//RX-PA3
	
	//���ڹ���ģʽ����
	usart_deinit(USART1);//��λ����USART
	usart_baudrate_set(USART1,brr);//���ò�����
	usart_word_length_set(USART1,USART_WL_8BIT);//8λ����λ
	usart_stop_bit_set(USART1,USART_STB_1BIT);//1��ֹͣλ
	usart_parity_config(USART1,USART_PM_NONE);//����żУ��
	usart_hardware_flow_cts_config(USART1,USART_CTS_DISABLE);//��Ӳ��������
	usart_hardware_flow_rts_config(USART1,USART_RTS_DISABLE);//��Ӳ��������
	usart_receive_config(USART1,USART_RECEIVE_ENABLE);//����ʹ��
	usart_transmit_config(USART1,USART_TRANSMIT_ENABLE);//����ʹ��
	
	//�ж�����
	nvic_irq_enable(USART1_IRQn,0,0);//�ж�ʹ��
	usart_interrupt_enable(USART1,USART_INT_RBNE);//�����ж�ʹ��
	
	//ʹ�ܴ���
	usart_enable(USART1);
	
}


void USART1_Send_A_Data(uint8_t data)
{
	usart_data_transmit(USART1, data);
	while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
}


void USART1_Send_Data_buf(uint8_t *data,uint16_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		USART1_Send_A_Data(*data);
		data++;
	}
}

void USART1_IRQHandler(void)
{
	uint8_t res;
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);
		res = usart_data_receive(USART1);
		USART1_Send_A_Data(res);
		
	}

}
