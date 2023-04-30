#include "app.h"

#define DBG_TAG "app"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>



//����4
struct rt_thread th4;
rt_uint8_t th4_stack[512] = {0};

//����5
struct rt_thread th5;
rt_uint8_t th5_stack[512] = {0};

//����6
struct rt_thread th6;
rt_uint8_t th6_stack[512] = {0};

//����7
struct rt_thread th7;
rt_uint8_t th7_stack[512] = {0};

struct rt_thread th8;
rt_uint8_t th8_stack[512] = {0};

uint8_t A72_flag = 0;
uint8_t A72_Device_Connect = 0;//A72Ӳ����������־λ
uint8_t NB73_Connect = 0;

uint8_t NODE_STATUS = 0;
uint16_t NODE_LIGHT = 0;
int NODE_PEOPLE = 0;

//��̬�ź���
rt_sem_t A72_Respond;//A72��Ӧ�ź���
rt_sem_t A72_Connect;//A72�����ź���
rt_sem_t A72_Data_handle;
rt_sem_t NB73_Data_handle;




//����4����(ZIGBEE���ݴ���)
void Zigbee_Data_handle(void *parameter)
{
	while(1)
    {
		rt_sem_take(A72_Data_handle,RT_WAITING_FOREVER);//�����ȴ�
		
		A72_HANDLE_DATA();
		
		rt_thread_mdelay(5);
		
    }
}
//����5���� �ϴ������Ʒ�����
void NB73_Send_Data(void *parameter)
{
    uint8_t buff[60] = {0};
	uint8_t i = 0;
	int len = 0;
	int databuf[3];
	while(1)
	{
		rt_thread_mdelay(5000);
		
		
		databuf[0] = NODE_STATUS;
		databuf[1] = NODE_LIGHT;
		databuf[2] = NODE_PEOPLE;
		
		rt_kprintf("NB send data\n");

		len = MODBUS_Data_Reporting(databuf,3,reg_addre,buff);
		for(i=0;i<len;i++)
		{
			NB73_Send_A_Data(buff[i]);
			rt_thread_mdelay(1);
		}	
	}

}
//����6����(IOT���ݴ���)
void IOT_Data_handle(void *parameter)
{
	while(1)
	{
		rt_sem_take(NB73_Data_handle,RT_WAITING_FOREVER);//�����ȴ�
		
		NB73_HANDLE_DATA();
		
		rt_thread_mdelay(100);
		
		
	}
}

void WALN_Init(void *parameter)
{
	rt_err_t _flag;
	
	while(1)
	{
		if(A72_Device_Connect == 0)
		{
			if(A72_Init() == ERROR)
			LOG_E("A72_Init create failed..\n");
			else
			{
				LOG_D("A72_Init create successed..\n");
				A72_Device_Connect = 1;//�豸���ӳɹ���־λ��1
				
				rt_thread_startup(&th4);
			}
		}
		if(NB73_Connect == 0)
		{
			if(NB73_IOT_Init() == false)
			LOG_E("NB73_Init create failed..\n");
			else
			{
				LOG_D("NB73_Init create successed..\n");
				NB73_Connect = 1;
				rt_thread_mdelay(6000);//�ȴ�NB���ӷ������ɹ�
				rt_thread_startup(&th5);
				rt_thread_startup(&th6);
				rt_thread_startup(&th8);
			}
		}
		if(A72_Device_Connect == 1 && NB73_Connect == 1)
		{
			_flag = rt_thread_suspend(&th7);//��������
				
			if(_flag == RT_EOK)
				rt_kprintf("th7 rt_thread_suspend successed..\n");
			else
				rt_kprintf("th7 rt_thread_suspend failed..\n");
		}
		rt_thread_mdelay(3000);
	}
}	

void CHEK_NODE(void *parameter)
{
	int data[3] = {0,0,0};
	uint8_t buff1[60] = {0};
	int len = 0;
	uint8_t i = 0;
	while(1)
	{
		rt_thread_mdelay(45000);
		
		
		len = MODBUS_Data_Reporting(data,3,0x0000,buff1);
		for(i=0;i<len;i++)
		{
			NB73_Send_A_Data(buff1[i]);
			rt_thread_mdelay(1);
		}
		rt_thread_mdelay(2000);
		memset(buff1,0,60);
		MODBUS_Data_Reporting(data,3,0x0003,buff1);
		for(i=0;i<len;i++)
		{
			NB73_Send_A_Data(buff1[i]);
			rt_thread_mdelay(1);
		}
	}

}
