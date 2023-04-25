#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.h"



rt_thread_t start = NULL;

int flag1 = 0, flag2 = 0;


void AppTaskStart(void *parameter)
{
	int ret = 0;//��ʱ����
	
	//��̬����������
	mutex1 = rt_mutex_create("mutex1",RT_IPC_FLAG_FIFO);//�Ƚ��ȳ�
	if(mutex1 == RT_NULL)
		LOG_E("mutex1 rt_mutex_create create failed..\n");
	else
		LOG_D("mutex1 rt_mutex_create create successed..\n");
	
	//��̬����������
	ret = rt_mutex_init(&mutex2,"mutex2",RT_IPC_FLAG_FIFO);//�Ƚ��ȳ�
	if(ret < 0)
		LOG_E("mutex2 rt_mutex_create create failed..\n");
	else
		LOG_D("mutex2 rt_mutex_create create successed..\n");
	
	//����1����
	ret = rt_thread_init(&th1,"th1_",th1_entry,NULL,th1_stack,sizeof(th1_stack),20,5);
	if(ret < 0)
		LOG_E("th1 create failed..\n");
	else
		LOG_D("th1 create successed..\n");
	
	//����2����
	ret = rt_thread_init(&th2,"th2_",th2_entry,NULL,th2_stack,sizeof(th2_stack),20,5);
	if(ret < 0)
		LOG_E("th2 create failed..\n");
	else
		LOG_D("th2 create successed..\n");
	
	//��������
	rt_thread_startup(&th1);
	rt_thread_startup(&th2);
	
	//������ɾ��
//	rt_mutex_delete(mutex1);
	rt_mutex_detach(&mutex2);
}





//������
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

