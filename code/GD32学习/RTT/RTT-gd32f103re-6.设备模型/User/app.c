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





//����1����
void th1_entry(void *parameter)
{
	while(1)
    {

        
    }
}
//����2����
void th2_entry(void *parameter)
{
	while(1)
    {

		
    }
}
//����3����
void th3_entry(void *parameter)
{
	while(1)
    {

		
    }
}
