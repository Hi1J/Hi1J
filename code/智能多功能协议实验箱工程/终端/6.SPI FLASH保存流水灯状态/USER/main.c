#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "W25Q64.h"


//FLASH������ˮ��״̬
int main(void)
{
	u8 i=0;//��ˮ�Ʊ�־λ
	u8 wirtebuf[1];//����һ����ʱ�洢״̬������
	Usart1_Init(115200);//���ڳ�ʼ��
	LED_Init();//LED��ʼ��
	sFLASH_Init();//FLASH��ʼ��
	Flash_Test();//FLASH����
	i = sFLASH_Read_Byte(0x0000fd);//��ȡ�ػ�ǰ������
	
	while(1)
	{
		//------------------��ˮ�Ƴ���---------------------//
		if(i == 7)
		{
			i = 0;
			LED1_7_Port->ODR = ~(0 << 6);//��7�ƹر�
			LED8_Port->ODR = ~(1 << 13);//��8�ƿ���
		}
		else
		{
			LED1_7_Port->ODR = ~(1 << (i++));//1-7����ˮ
			LED8_Port->ODR = ~(0 << 13);//��8�ƹر�
		
		}
		//------------------��ˮ�Ƴ���---------------------//
	
		//-----------------FLASH��������-------------------//
		wirtebuf[0] = i;//������
		sFLASH_EraseSector(0x0000fd);//��������
		sFLASH_WriteBuffer(wirtebuf,0x0000fd,1);//д����
		//-----------------FLASH��������-------------------//
		
		delay_ms(300);//300ms�ӳ�
	}
	
	

}
