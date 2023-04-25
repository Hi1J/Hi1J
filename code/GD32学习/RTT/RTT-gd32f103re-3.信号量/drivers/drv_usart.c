#include "drv_usart.h"


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
	
	
	
	//ʹ�ܴ���
	usart_enable(USART0);
	
}

