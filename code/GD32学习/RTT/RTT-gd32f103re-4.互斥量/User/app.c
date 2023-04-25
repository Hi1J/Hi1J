#include "app.h"

//����1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//����2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};

//��̬������
rt_mutex_t mutex1;

//��̬������
struct rt_mutex mutex2;




//����1����
void th1_entry(void *parameter)
{
	while(1)
    {
		rt_mutex_take(mutex1,RT_WAITING_FOREVER);//���� �����ȴ�
		flag1++;
		rt_thread_mdelay(1000);
		flag2++;
		rt_mutex_release(mutex1);//����			
        
    }
}
//����2����
void th2_entry(void *parameter)
{
	while(1)
    {
		rt_mutex_take(mutex1,RT_WAITING_FOREVER);//���� �����ȴ�
		flag1++;
		flag2++;
		rt_mutex_release(mutex1);//����	
		rt_kprintf("flag1:%d,flag2:%d\n",flag1,flag2);
		rt_thread_mdelay(1000);
		
    }
}
