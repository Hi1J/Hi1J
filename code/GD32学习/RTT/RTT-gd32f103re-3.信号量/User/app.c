#include "app.h"

//����1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//����2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};

//��̬��ʱ
rt_timer_t tm1;

//��̬��ʱ
struct rt_timer tm2;
int tm2_flags = 0;

//��̬�ź���
rt_sem_t sem1;

//��̬�ź���
struct rt_semaphore sem2;

//����1����
void th1_entry(void *parameter)
{
	while(1)
	{
		rt_thread_mdelay(5000);//5s
		rt_sem_take(sem1,RT_WAITING_FOREVER);//��ȡ�ź���1 һֱ�ȴ�
		flags++;
		if(flags == 100)
			flags = 0;
		rt_kprintf("th1_entry [%d]\n",flags);
		rt_sem_release(&sem2);//�ͷ��ź���2(��th2ִ��)
		
	}
	
}
//����2����
void th2_entry(void *parameter)
{
	while(1)
	{
		rt_sem_take(&sem2,RT_WAITING_FOREVER);//��ȡ�ź���2 һֱ�ȴ� (һ��ʼsem2Ϊ0��������֤��th1��ִ��)
		if(flags > 0)
			flags--;
		rt_kprintf("th2_entry [%d]\n",flags);
		rt_sem_release(sem1);//�ͷ��ź���1(��th1ִ��)
		rt_thread_mdelay(1000);
	}
	
}

//���ȹ��Ӻ���
void scheduler_hook(struct rt_thread *from,struct rt_thread *to)
{
	rt_kprintf("from:%s ---> to:%s\n",from->name,to->name);
}

//��̬��ʱ�ص�����
void tm1_callback(void *parameter)
{
	rt_kprintf("111111111111tm1_callback running....\n");
}

//��̬��ʱ�ص�����
void tm2_callback(void *parameter)
{
	rt_tick_t timeout = 1000;//ʱ���Ϊ1s
	tm2_flags++;
	if(tm2_flags == 10)//���ִ����10�� ���Ϊ���δ���
	{
		rt_timer_control(&tm2,RT_TIMER_CTRL_SET_ONESHOT,NULL);
		tm2_flags = 0;
	}
	rt_timer_control(&tm2,RT_TIMER_CTRL_SET_TIME,(void *)&timeout);//�޸�ʱ��
	rt_kprintf("[%u]222222222222tm2_callback running....[%d]\n",rt_tick_get(),tm2_flags);
}
