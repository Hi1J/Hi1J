#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.h"



rt_thread_t start = NULL;

int flags = 0;//����һ��ȫ�ֱ��� Ŀ�������߳�1���߳�2���õ�


void AppTaskStart(void *parameter)
{
	int ret = 0;//��ʱ����
	
	//��̬��ʱ����
//	tm1 = rt_timer_create("tm1_demo",tm1_callback,NULL,3000, //3sִ��һ��   
//	RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);//������ ��ʱ
//	if(tm1 == RT_NULL)
//		LOG_E("rt_timer_create failed..\n");
//	else
//		LOG_D("rt_timer_create successed..\n");
	
	//��̬��ʱ����
//	rt_timer_init(&tm2,"tm2_demo",tm2_callback,NULL,3000,//3sִ��һ�� 
//	RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);//������ ��ʱ 
	
	//��̬�ź�������
	sem1 = rt_sem_create("sem1",1,RT_IPC_FLAG_FIFO);//�����Ƚ��ȳ���ʽ��ȡ��Դ
	if(sem1 == RT_NULL)
		LOG_E("dynamic sem1 rt_sem_create failed..\n");
	else
		LOG_D("dynamic sem1 rt_sem_create successed..\n");
	
	//��̬�ź�������
	ret = rt_sem_init(&sem2,"sem2",0,RT_IPC_FLAG_FIFO);//�����Ƚ��ȳ���ʽ��ȡ��Դ
	if(ret < 0)
		LOG_E("static sem2 rt_sem_create failed..\n");
	else
		LOG_D("static sem2 rt_sem_create successed..\n");
		
	
	//����1����
	ret = rt_thread_init(&th1,"th1_",th1_entry,NULL,th1_stack,sizeof(th1_stack),20,5);
	if(ret < 0)
		LOG_E("th1 create failed..\n");
	else
		LOG_D("th1 create successed..\n");
	
	//����2����
	ret = rt_thread_init(&th2,"th2_",th2_entry,NULL,th2_stack,sizeof(th2_stack),19,5);
	if(ret < 0)
		LOG_E("th2 create failed..\n");
	else
		LOG_D("th2 create successed..\n");
	
	//��������
	rt_thread_startup(&th1);
	rt_thread_startup(&th2);
	
	//��ʱ����
//	rt_timer_start(tm1);
//	rt_timer_start(&tm2);
}





//�ź���
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

