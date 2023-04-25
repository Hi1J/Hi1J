#include "app.h"

//����1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//����2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};

//����3
struct rt_thread th3;
rt_uint8_t th3_stack[512] = {0};

//��̬�¼���
rt_event_t event1;

//��̬�¼���
struct rt_event event2;




//����1����
void th1_entry(void *parameter)
{
	while(1)
    {
		rt_event_recv(event1,EVENT_FLAGS_1,RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND,//�����¼�1
		RT_WAITING_FOREVER,NULL);//�����ȴ�
		rt_kprintf("111111th1_entry...\n");
		rt_event_send(event1,EVENT_FLAGS_2);//�����¼�2
		rt_thread_mdelay(1000);
        
    }
}
//����2����
void th2_entry(void *parameter)
{
	while(1)
    {
		rt_event_recv(event1,EVENT_FLAGS_2,RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND,//�����¼�2
		RT_WAITING_FOREVER,NULL);//�����ȴ�
		rt_kprintf("222222th2_entry...\n");
		rt_event_send(event1,EVENT_FLAGS_3);//�����¼�3
		rt_thread_mdelay(1000);
		
    }
}
//����3����
void th3_entry(void *parameter)
{
	while(1)
    {
		rt_event_recv(event1,EVENT_FLAGS_3,RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND,//�����¼�3
		RT_WAITING_FOREVER,NULL);//�����ȴ�
		rt_kprintf("333333th3_entry...\n");
		rt_event_send(event1,EVENT_FLAGS_1);//�����¼�1
		rt_thread_mdelay(1000);
		
    }
}
