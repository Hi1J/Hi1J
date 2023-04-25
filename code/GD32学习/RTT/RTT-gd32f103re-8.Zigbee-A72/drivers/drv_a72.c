#include "drv_a72.h"

#define DBG_TAG "a72"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

uint8_t A72sendbuf[20];
A72_Return_Value _value;

/*
�������ܣ�A72��λ
������void
����ֵ��void
��ע��ÿ���ϵ�ʱ�踴λ
*/
void A72_RESET(void)
{
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_SET();//����
#endif
	memset(A72sendbuf,0,20);
	A72sendbuf[0] = A72_Sbit;
	A72sendbuf[1] = C_RESET >> 8;
	A72sendbuf[2] = C_RESET & 0x00FF;
	A72sendbuf[3] = L_RESET >> 8;
	A72sendbuf[4] = L_RESET & 0x00FF;
	A72sendbuf[5] = V_RESET;
	A72sendbuf[6] = A72_Ebit;
	A72_Send_Data(A72sendbuf,RESET_Bytes);
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_RESET();//����
#endif	
}


/*
�������ܣ���ȡ��ַ
������void
����ֵ��int (����0���ɹ� ����1��ʧ��)
��ע�����ã�Э�������ն˽ڵ㡢·����
*/
int A72_Read_Address(void)
{	
	uint8_t Read_Adder_OK = 0;//������ɱ�־
	uint8_t RX_Correct = 0;//������ȷ��־
	uint8_t i = 0;//��ʱ����
	uint8_t count = 0;//��ʱ����
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_SET();//����
#endif
	memset(A72sendbuf,0,20);
	A72sendbuf[0] = A72_Sbit;
	A72sendbuf[1] = C_Read_Adder >> 8;
	A72sendbuf[2] = C_Read_Adder & 0x00FF;
	A72sendbuf[3] = L_Read_Adder >> 8;
	A72sendbuf[4] = L_Read_Adder & 0x00FF;
	A72sendbuf[5] = V_Read_Adder;
	A72sendbuf[6] = A72_Ebit;
	A72_Send_Data(A72sendbuf,Read_Adder_Bytes);
	if(rt_sem_take(sem1,50) == 0)//�ȴ�������� �ȴ�ʱ��50ms
	{
		for(i=0;(Read_Adder_OK==0 && i < 20);i++)
		{
			if(A72_RX_BUF[i] == A72_Sbit && RX_Correct == 0)
				if(A72_RX_BUF[i+1] == 0x81 && A72_RX_BUF[i+2] == 0xC0)
					RX_Correct = 1;//������ȷ
			if(RX_Correct == 1)//������յ���ȷ������
			{
				i += 6;//��������λ������λ��У��λ
				for(count=0;count<L_Adder_LEN;count++)//IEEE��ַ��ȡ
					_value.L_Adder[count] = A72_RX_BUF[i++];
				for(count=0;count<S_Adder_LEN;count++)//�̵�ַ��ȡ
					_value.S_Adder[count] = A72_RX_BUF[i++];
				Read_Adder_OK = 1;//�������
			}
			if(i >= 35)
			{
#if ZIGBEE_A72_NODE
				A72_WAKE_UP_RESET();//����
#endif
				return ERROR;//����ʧ��		
			}
		}

	}
	else//���A72����Ӧ ���ȡʧ��
	{
#if ZIGBEE_A72_NODE
		A72_WAKE_UP_RESET();//����
#endif
		return ERROR;//����ʧ��
	}
	
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_RESET();//����
#endif		
	return SUCCESS;	//���سɹ�
}



void A72_Init(void)
{
	uint8_t count;
#if ZIGBEE_A72_NODE
	//����GPIO
	rcu_periph_clock_enable(RCU_GPIOA);//��GPIOAʱ��
	gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);//GPIO����ģʽ����
	gpio_bit_reset(GPIOA,GPIO_PIN_1);
#endif
	while(1)
	{
		A72_RESET();//��λ
		rt_thread_mdelay(100);
		if(A72_Read_Address() == SUCCESS)
		{
			LOG_D("A72_Read_Address successed..\n");
			rt_kprintf("IEEE: ");
			for(count=0;count<L_Adder_LEN;count++)
				rt_kprintf("%x ",_value.L_Adder[count]);
			rt_kprintf("\nshort adderss: ");
			for(count=0;count<S_Adder_LEN;count++)
				rt_kprintf("%x ",_value.S_Adder[count]);
			rt_kprintf("\n");
			break;
		}
		else
			LOG_E("A72_Read_Address failed..\n");
	
	}
	
}

