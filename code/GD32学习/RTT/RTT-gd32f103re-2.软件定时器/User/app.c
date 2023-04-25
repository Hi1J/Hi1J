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


//����1����
void th1_entry(void *parameter)
{
	while(1)
    {
		if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 0)
		{gpio_bit_set(GPIOA,GPIO_PIN_1);rt_kprintf("LED_ON\n");}
		else if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 1)
		{gpio_bit_reset(GPIOA,GPIO_PIN_1);rt_kprintf("LED_0FF\n");}
					
        rt_thread_mdelay(500);
    }
}
//����2����
void th2_entry(void *parameter)
{
	while(1)
    {
        rt_kprintf("th2_enter running...\n");	
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
