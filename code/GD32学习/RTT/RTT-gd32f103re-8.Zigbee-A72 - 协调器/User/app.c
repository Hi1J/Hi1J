#include "app.h"

#define DBG_TAG "app"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

//����1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//����2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};
rt_err_t A72_Cheak;

//����3
struct rt_thread th3;
rt_uint8_t th3_stack[512] = {0};

//����4
struct rt_thread th4;
rt_uint8_t th4_stack[512] = {0};

uint8_t A72_flag = 0;
uint8_t A72_Device_Connect = 0;//A72Ӳ����������־λ


//��̬�ź���
rt_sem_t A72_Respond;//A72��Ӧ�ź���
rt_sem_t A72_Connect;//A72�����ź���
rt_sem_t A72_Data_handle;



////����1 �������״̬(����ǰ����)
//void Zigbee_Network_Status_Cheak_front(void *parameter)
//{
//	while(1)
//    {
//		if(A72_SEND_ORDER(&Connect_status,A72_Read_Connect_status) == ERROR)
//		{
//			
//			rt_kprintf("A72_Read_Connect_status failed..\n");
//			
//			rt_thread_mdelay(5000);
//		}
//		else
//		{
//			
//			rt_kprintf("A72_Read_Connect_status successed..\n");
//			
//			if(_value.S_Network == 0)//���δ���� 5����ٴγ���
//			{
//				rt_kprintf("A72_Connect_status: OFF\n");
//				
//				rt_thread_mdelay(5000);
//			}
//			else//�������� �������� ͬʱ�������ͺ���
//			{
//				rt_kprintf("A72_Connect_status: ON\n");
//				rt_kprintf("A72_Connect successed..\n");
//				
//				rt_sem_release(A72_Connect);
//				
//				
//				A72_Cheak = rt_thread_suspend(&th1);//��������
//				
//				if(A72_Cheak == RT_EOK)
//					rt_kprintf("th3 rt_thread_suspend successed..\n");
//				else
//					rt_kprintf("th3 rt_thread_suspend failed..\n");
//					
//			 }
//		}	
//    }
//}


////����2 �������״̬(��������)
//void Zigbee_Network_Status_Cheak_after(void *parameter)
//{
//	while(1)
//	{
//		
//		if(_value.S_Network == 1)//�������
//		{
//			
//			rt_thread_mdelay(45000);//45sһ���
//			
//			rt_kprintf("Connection status Cheaking...\nOnce every 45s\n");
//			
//			if(A72_SEND_ORDER(&Connect_status,A72_Read_Connect_status) == ERROR)
//			{
//				rt_kprintf("Cheak Connect failed..\n");
//			}
//			else
//			{
//				rt_kprintf("Cheak Connect successed..\n");
//				
//				if(_value.S_Network == 0)//���A72�Ͽ�
//				{
//					rt_thread_resume(&th1);//�ָ�th1 A72�����߳�
//				}
//				
//			}
//		}
//		else
//		{
//			rt_thread_mdelay(3000);//λ����ʱ3����һ��
//		}
//	}
//}

////����3���� ��������
//void Zigbee_Send_Data(void *parameter)
//{
//	
//	uint8_t buf[2] = {0x11,0x10};
//	
//	while(1)
//    {
//		rt_sem_take(A72_Connect,RT_WAITING_FOREVER);//�����ȴ�
//		
//		if(A72_SEND_DATA(_COORDINATOR,&A72_SEND_MODE_SHORT,buf,2) == SUCCESS)
//		{
//			rt_kprintf("A72_Send_Data successed..\n");
//		}
//		else
//		{
//			rt_kprintf("A72_Send_Data fialed..\n");
//		}

//        rt_thread_mdelay(6000);//����Ƶ��6s
//		
//		if(_value.S_Network == 1)//���Zigbee�Ѿ�����
//			rt_sem_release(A72_Connect);//����
//    }
//}

//����4����(���ݴ���)
void Zigbee_Data_handle(void *parameter)
{
	while(1)
    {
		rt_sem_take(A72_Data_handle,RT_WAITING_FOREVER);//�����ȴ�
		
		A72_HANDLE_DATA();
		
		rt_thread_mdelay(500);
		
    }
}


