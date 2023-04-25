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
	int ret = 0;//临时变量
	
	//任务1创建
	ret = rt_thread_init(&th1,"th1_",th1_entry,NULL,th1_stack,sizeof(th1_stack),19,5);
	if(ret < 0)
			LOG_E("th1 create failed..\n");
	else
			LOG_D("th1 create successes..\n");
	
	//任务2创建
	ret = rt_thread_init(&th2,"th2_",th2_entry,NULL,th2_stack,sizeof(th2_stack),20,5);
	if(ret < 0)
			LOG_E("th2 create failed..\n");
	else
			LOG_D("th2 create successes..\n");
	
	//任务启动
	rt_thread_startup(&th1);
	rt_thread_startup(&th2);
}





//任务创建
int main()
{
	//开始任务创建
	start = rt_thread_create("App Task Start", AppTaskStart, NULL,1024, 20, 5);
  if(start == RT_NULL){
     LOG_E("App Task Start create failed..\n");
     return -RT_ENOMEM;
   }

   LOG_D("App Task Start create successes..\n");
   rt_thread_startup(start);//启动
	 
	

}

