#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "new.h"

//YJͨ��Э��
int main(void)
{
	Usart1_Init(115200);//���ڳ�ʼ��
	printf("����1��ʼ�����\r\n");
	YJ_Init();//ͨ��Э���ʼ��
	while(1)
	{
		if(YJ_RX_Aframe_OK == 1)
		{
			YJ_Data_Analysis();
		}
		
		
	}
	
	

}
