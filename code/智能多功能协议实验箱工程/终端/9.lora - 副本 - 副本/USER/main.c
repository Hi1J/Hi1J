#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"
#include "jpg_app.h"
#include "fatfs_app.h"
#include "lora.h"
//LORA������ʪ����ǿ��
int main(void)
{
	char Tembuf[64] = "\0";
	char Humbuf[64]= "\0";
	char Lxbuf[64] = "\0";
	Usart1_Init(115200);
	printf("����1��ʼ���ɹ�\r\n");
	LCD_Init();//lcd��ʼ��
	SD_Fatfs_Init();//sd����ʼ��
	
	LCD_Clear(LIGHTBLUE);
	JPG_ShowFunc("0:/pic2/cat2.jpg",0,0);
	JPG_ShowFunc("0:/pic1/tem03.jpg",5,160);
	JPG_ShowFunc("0:/pic1/hem03.jpg",5,220);
	JPG_ShowFunc("0:/pic1/light03.jpg",5,280);
	LCD_ShowStr(55,160+9,(u8 *)"�¶�:");
	LCD_ShowStr(55,220+9,(u8 *)"ʪ��:");
	LCD_ShowStr(55,280+9,(u8 *)"��ǿ:");
	
	LCD_ShowStr(125,160+9,(u8 *)"���϶�");
	LCD_ShowStr(125,220+9,(u8 *)"RH%");
	LCD_ShowStr(140,280+9,(u8 *)"Lux");
	
	
	Init_Lora();//lora��ʼ��
	
	while(1)
	{
		if(Rx_OK == 1)//������յ�����
		{
			
			//д������
			sprintf(Tembuf,"%2d",usart4_buf[0]);
			sprintf(Humbuf,"%2d",usart4_buf[1]);
			sprintf(Lxbuf,"%-5d",(usart4_buf[2]<<8) | usart4_buf[3]);
			
			//LCD��ʾ����
			LCD_ShowStr(97,160+9,(u8 *)Tembuf);
			LCD_ShowStr(97,220+9,(u8 *)Humbuf);
			LCD_ShowStr(97,280+9,(u8 *)Lxbuf);
			
			Rx_OK = 0;//���ձ�־λ����
			memset(usart4_buf,0,sizeof(usart4_buf));//��ս�������
			
		}
	}

}
