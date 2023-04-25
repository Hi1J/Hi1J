#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.h"



rt_thread_t start = NULL;



void AppTaskStart(void *parameter)
{
	int ret = 0;//��ʱ����
	rt_err_t ret1 = 0;
	
	//����1��ʼ��
	u1_dev = rt_device_find("usart1");
	if(u1_dev == RT_NULL)
		LOG_E("rt_device_find[usart1] failed..\n");
	else
	{
		LOG_D("rt_device_find[usart1] successed..\n");
		ret1 = rt_device_init(u1_dev);
		if(ret1 < 0)
			LOG_E("rt_device_init[usart1] failed..\n");
		else
			LOG_D("rt_device_init[usart1] successed..\n");
		ret1 = rt_device_open(u1_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret1 < 0)
			LOG_E("rt_device_open[usart1] failed..\n");
		else
			LOG_D("rt_device_open[usart1] successed..\n");		
	}
	
	
	//��̬�ź�������
	A72_Respond = rt_sem_create("A72_Respond",0,RT_IPC_FLAG_FIFO);//�����Ƚ��ȳ���ʽ��ȡ��Դ
	if(A72_Respond == RT_NULL)
		LOG_E("dynamic A72_Respond rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Respond rt_sem_create successed..\n");	
	
	A72_Connect = rt_sem_create("A72_Connect",0,RT_IPC_FLAG_FIFO);//�����Ƚ��ȳ���ʽ��ȡ��Դ
	if(A72_Connect == RT_NULL)
		LOG_E("dynamic A72_Connect rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Connect rt_sem_create successed..\n");
	
	A72_Data_handle = rt_sem_create("A72_Data_handle",0,RT_IPC_FLAG_FIFO);//�����Ƚ��ȳ���ʽ��ȡ��Դ
	if(A72_Data_handle == RT_NULL)
		LOG_E("dynamic A72_Data_handle rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Data_handle rt_sem_create successed..\n");
	

//	//����1����
//	ret = rt_thread_init(&th1,"Zigbee_Network_Status_Cheak_front",Zigbee_Network_Status_Cheak_front,NULL,th1_stack,sizeof(th1_stack),21,5);
//	if(ret < 0)
//		LOG_E("th1 create failed..\n");
//	else
//		LOG_D("th1 create successed..\n");
//		
//	//����2����
//	ret = rt_thread_init(&th2,"Zigbee_Network_Status_Cheak_after",Zigbee_Network_Status_Cheak_after,NULL,th2_stack,sizeof(th2_stack),20,5);
//	if(ret < 0)
//		LOG_E("th2 create failed..\n");
//	else
//		LOG_D("th2 create successed..\n");
//	
//	//����3����
//	ret = rt_thread_init(&th3,"Zigbee_Send_Data",Zigbee_Send_Data,NULL,th3_stack,sizeof(th3_stack),22,5);
//	if(ret < 0)
//		LOG_E("th3 failed..\n");
//	else
//		LOG_D("th3 create successed..\n");
	
	//����4����
	ret = rt_thread_init(&th4,"Zigbee_Data_handle",Zigbee_Data_handle,NULL,th4_stack,sizeof(th4_stack),19,5);
	if(ret < 0)
		LOG_E("th4 create failed..\n");
	else
		LOG_D("th4 create successed..\n");
	
	//A72��ʼ��
	if(A72_Init() == ERROR)
		LOG_E("A72_Init create failed..\n");
	else
	{
		LOG_D("A72_Init create successed..\n");
		A72_Device_Connect = 1;//�豸���ӳɹ���־λ��1
//		rt_thread_startup(&th1);//���A72�����ʼ���ɹ� ����th1
	}
//	A72_SEND_ORDER(&READ_ADDERSS,A72_Read_Adderss);
		
	
	//��������
//	rt_thread_startup(&th2);
//	rt_thread_startup(&th3);
	rt_thread_startup(&th4);

}





//ZIGBEE-A72
int main()
{
	//��ʼ���񴴽�
  start = rt_thread_create("App Task Start", AppTaskStart, NULL,1024, 20, 5);
  if(start == RT_NULL){
     LOG_E("App Task Start create failed..\n");
     return -RT_ENOMEM;
   }

   LOG_D("App Task Start create successed..\n");
   rt_thread_startup(start);//����
}

