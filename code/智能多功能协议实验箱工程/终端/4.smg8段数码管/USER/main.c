#include "stm32f10x.h"
#include "delay.h"
#include "smg.h"

//8�������SMG��ʾ����
int main(void)
{
	char data[8] = {'2','0','0','1','0','4','1','2'};
	u8 data_code[8] = {0};//��һ�������������data���ڵ�code��
	Smg_Config();//smg��ʼ��
	
	Seg_Tran(data,data_code);//ת��
	
	
	while(1)
	{
		Digitron_Display(data_code,8);//��̬��ʾ
		delay_ms(2);//1ms-2ms����һ�ξͿ���������ʾ
		
		
		
	}
	
	

}
