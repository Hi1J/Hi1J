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



//��̬�ź���
rt_sem_t sem1;



//����1����
void th1_entry(void *parameter)
{
	while(1)
    {
//		rt_kprintf("hello\n");
        rt_thread_mdelay(1000);
    }
}
//����2����
void th2_entry(void *parameter)
{
	while(1)
    {
//		if(A72_Read_Address() == SUCCESS)
//			rt_kprintf("successed..\n");
		rt_thread_mdelay(1000);
		
    }
}

