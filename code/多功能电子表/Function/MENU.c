/************************************************************
��д�ˣ������
���ڣ�2022��11��19��
���ݣ��๦�ܵ��ӱ�˵���ʾ����
�޸���־��
1-2022��11��20��16��26�֡����ݣ���������ں�����D�˵�


************************************************************/
#include "MENU.h"

void Main_MENU(u8 *b,u8 *c){
	u8 buffer[3];	//���ڴ��LM75A�������¶����� 
	while(*b==0){  //���˵���ʾ
		LM75A_GetTemp(buffer); //��ȡLM75A���¶�����

		OLED_DISPLAY_16x16(2,5*8,18);	//oled  �������¶�:���͡����ڡ� 
		OLED_DISPLAY_16x16(2,7*8,19);
		OLED_DISPLAY_16x16(6,0*16,16);  
		OLED_DISPLAY_16x16(6,1*16,17);
		OLED_DISPLAY_16x16(6,2*16,8);
		OLED_DISPLAY_16x16(6,3*16,9);
		OLED_DISPLAY_8x16(6,8*8,':');
	
		
		
		//��ʾ�¶�	
		if(buffer[0])OLED_DISPLAY_8x16(6,9*8,'-'); //�����1��Ϊ1���Ǹ��¶�
		else  OLED_DISPLAY_8x16(6,9*8,'+');		   //����Ϊ���¶�
		OLED_DISPLAY_8x16(6,10*8,buffer[1]/10+0x30);//��ʾ�¶�ֵ
		OLED_DISPLAY_8x16(6,11*8,buffer[1]%10+0x30);//
		OLED_DISPLAY_8x16(6,12*8,'.');//
		OLED_DISPLAY_8x16(6,13*8,buffer[2]/10+0x30);//
		OLED_DISPLAY_8x16(6,14*8,buffer[2]%10+0x30);//
		OLED_DISPLAY_8x16(6,15*8,'C');//
	
		//��ʾʱ��
		if(RTC_Get()==0){
		//oled��ʾ��
		OLED_DISPLAY_8x16(0,1*8,ryear/1000+0x30); //��һλ
		OLED_DISPLAY_8x16(0,2*8,(ryear/100)%10+0x30);		
		OLED_DISPLAY_8x16(0,3*8,(ryear/10)%10+0x30);
		OLED_DISPLAY_8x16(0,4*8,ryear%10+0x30);
		OLED_DISPLAY_16x16(0,5*8,12);//��
		OLED_DISPLAY_8x16(0,7*8,rmon/10+0x30);
		OLED_DISPLAY_8x16(0,8*8,rmon%10+0x30);
		OLED_DISPLAY_16x16(0,9*8,13);//��
		OLED_DISPLAY_8x16(0,11*8,rday/10+0x30);
		OLED_DISPLAY_8x16(0,12*8,rday%10+0x30);
		OLED_DISPLAY_16x16(0,13*8,14);//��
		//�������ʾ��
		TM1640_display(0,rhour/10);
		TM1640_display(1,rhour%10);
		TM1640_display(2,21);
		TM1640_display(3,rmin/10);
		TM1640_display(4,rmin%10);
		TM1640_display(5,21);
		TM1640_display(6,rsec/10);
		TM1640_display(7,rsec%10);
	
		}
	
		//��ʾ���ڼ�
		switch(rweek){
			case 1:OLED_DISPLAY_16x16(2,9*8,20);break;
			case 2:OLED_DISPLAY_16x16(2,9*8,21);break;
			case 3:OLED_DISPLAY_16x16(2,9*8,22);break;
			case 4:OLED_DISPLAY_16x16(2,9*8,23);break;
			case 5:OLED_DISPLAY_16x16(2,9*8,24);break;
			case 6:OLED_DISPLAY_16x16(2,9*8,25);break;
			case 0:OLED_DISPLAY_16x16(2,9*8,26);break;
			default:break;
		}
	
		//��ʱ
//			if(ENCODER_READ()==3)TimeSettingA();//����ť���£������ʱģʽ
	
	
		//��ˮ�� 
		TM1640_led(*c);
		*c<<=1;
		if(*c==0x00)*c=0x01;
	
		if(k==1){*b=1;k=0;}
		delay_ms(150); //�ӳ�

	}
} 

void Auxiliary_MENU(u8 *b,u8 *c){
	while(*b==1){	 //��ʱ�˵���ʾ
		OLED_DISPLAY_8x16(0,0*8,'A');		 //A
		OLED_DISPLAY_8x16(0,1*8,':');		 //:
		OLED_DISPLAY_16x16(0,2*16,29);		 //��
		OLED_DISPLAY_16x16(0,3*16,30);		 //��
		OLED_DISPLAY_16x16(0,4*16,31);		 //ʱ
		OLED_DISPLAY_16x16(0,5*16,32);		 //��
		OLED_DISPLAY_16x16(0,6*16,33);		 //��

		OLED_DISPLAY_8x16(2,0*8,'B');		 //B
		OLED_DISPLAY_8x16(2,1*8,':');		 //:
		OLED_DISPLAY_16x16(2,2*16,29);		 //��
		OLED_DISPLAY_16x16(2,3*16,30);		 //��
		OLED_DISPLAY_16x16(2,4*16,12);		 //ʱ
		OLED_DISPLAY_16x16(2,5*16,13);		 //��
		OLED_DISPLAY_16x16(2,6*16,14);		 //��
		
		OLED_DISPLAY_8x16(6,0*8,'D');		 //D
		OLED_DISPLAY_8x16(6,1*8,':');		 //:
		OLED_DISPLAY_16x16(6,2*16,34);		 //��
		OLED_DISPLAY_16x16(6,3*16,35);		 //��

		if(RTC_Get()==0){
		//�������ʾ��
		TM1640_display(0,rhour/10);
		TM1640_display(1,rhour%10);
		TM1640_display(2,21);
		TM1640_display(3,rmin/10);
		TM1640_display(4,rmin%10);
		TM1640_display(5,21);
		TM1640_display(6,rsec/10);
		TM1640_display(7,rsec%10);
		}
		
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))TimeSettingA();	//��A�������ʱ����ģʽ
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))TimeSettingB();	//��A�������������ģʽ

		//��ˮ��
		TM1640_led(*c);	  
		*c<<=1;
		if(*c==0x00)*c=0x01;
	
		if(k==1){*b=0;k=0;}
		delay_ms(150); //�ӳ�	
	}
}

