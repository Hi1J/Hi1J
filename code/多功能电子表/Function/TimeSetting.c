/************************************************************
��д�ˣ������
���ڣ�2022��11��19��
���ݣ��๦�ܵ��ӱ��ʱ����
�޸���־��
1-2022��11��20��16��25��-���ݣ������˵������յĺ���

**************************************************************/

#include "TimeSetting.h"





void TimeSettingA(void){
		u8 z,s,m,h,c,i;//���ñ�־��
//		delay_ms(200);  //��ENCODER_READ()����ֵ��Ӧʱ�� ��û������� ����� ������ ��ʱ��ͼ��޴�ԼΪ200ms ���ڻ���� (�����ǰ�������ʱʡ��)
		z=1;	  //ѭ����־λ
		s=rsec;m=rmin;h=rhour;	//��¼�´˿̵�ʱ��
		i=0;				 //Ĭ�ϵ�ʱģʽ
		TM1640_led(0x80);   //Ĭ�ϵ���ָʾ����  
		OLED_DISPLAY_CLEAR();  //����
		OLED_DISPLAY_16x16(2,2*16,27);	  //��
		OLED_DISPLAY_16x16(2,3*16,28);	  //��
		OLED_DISPLAY_16x16(2,4*16,29);	  //��
		OLED_DISPLAY_16x16(2,5*16,30);	  //��
		OLED_DISPLAY_16x16(2,6*16,33);	  //�루Ĭ�ϣ�
		OLED_DISPLAY_8x16(6,0*8,'A');	  //A
		OLED_DISPLAY_8x16(6,6*8,'B');	  //B
		OLED_DISPLAY_8x16(6,12*8,'C');	  //C 
		OLED_DISPLAY_16x16(6,1*16,31);	  //ʱ
		OLED_DISPLAY_16x16(6,4*16,32);	  //��
		OLED_DISPLAY_16x16(6,7*16,33);	  //��
		while(z==1)	  //
		{
			c=ENCODER_READ();	  //�ٴζ�ȡ����������ֵ	��û�������delay_ms(200);�����������ֵ��Ϊ3 ����ֱ���˳���


			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){	  //��A ��ʱ
			i=2;TM1640_led(0x01);OLED_DISPLAY_16x16(2,6*16,31);}	  
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)){	  //��B ����
			i=1;TM1640_led(0x20);OLED_DISPLAY_16x16(2,6*16,32);}	  
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)){	  //��C ����
			i=0;TM1640_led(0x80);OLED_DISPLAY_16x16(2,6*16,33);}	  



			if(c==1){	 //��ת	 ��
				if(i==0){s++;if(s==60)s=0;}
				if(i==1){m++;if(m==60)m=0;}
				if(i==2){h++;if(h==24)h=0;}		}
			
			if(c==2){	  //��ת  ��
				if(i==0){if(s==0)s=60;s--;}
				if(i==1){if(m==0)m=60;m--;}
				if(i==2){if(h==0)h=24;h--;}		 }
									
			if(c==3)z=0;		   		//��ʱ���ٴΰ��±��������˳���ʱģʽ�Ӷ�������ʱ
	
			TM1640_display(0,h/10);		//��ʾ��ֵ
			TM1640_display(1,h%10);
			TM1640_display(2,21);
			TM1640_display(3,m/10);
			TM1640_display(4,m%10);
			TM1640_display(5,21);
			TM1640_display(6,s/10);
			TM1640_display(7,s%10);
		}

		
		//rsec=s;rmin=m;rhour=h;		  
		if(RTC_Get()==0)RTC_Set(ryear,rmon,rday,h,m,s);	   //���޸ĺ��ʱ��ֵд��	 ʱ���д������Ҫ1s Ҳ����˵ �������׼ �õ���Ԥ��ֵ��1s
		OLED_DISPLAY_CLEAR(); //������
		//delay_ms(200);		//����ͬ�ϣ���Щ��������Ӱ��rtc��ʱ	
		TM1640_led(0x00);	 //�˳���ʱģʽ���ʱָʾ����	

}



void  TimeSettingB(void){
		u8 m,d,i,c,j=0;
		u16 y;
		i=0;
		y=ryear;m=rmon;d=rday;		//��¼��ǰ������
	
		OLED_DISPLAY_CLEAR();  //����
		OLED_DISPLAY_16x16(2,1*16,27);	  //��
		OLED_DISPLAY_16x16(2,2*16,28);	  //��
		OLED_DISPLAY_16x16(2,3*16,29);	  //��
		OLED_DISPLAY_16x16(2,4*16,30);	  //��
		OLED_DISPLAY_8x16(6,0*8,'A');	  //A
		OLED_DISPLAY_8x16(6,6*8,'B');	  //B
		OLED_DISPLAY_8x16(6,12*8,'C');	  //C
		OLED_DISPLAY_16x16(6,1*16,12);	  //��
		OLED_DISPLAY_16x16(6,4*16,13);	  //��
		OLED_DISPLAY_16x16(6,7*16,14);	  //��

		OLED_DISPLAY_16x16(2,5*16,14);	  //�գ�Ĭ�ϣ�

		TM1640_led(0x80); 

		while(1){

			c=ENCODER_READ();
			if((y%4==0&&y%100!=0)||y%400==0)i=1;//�ж��Ƿ�Ϊ����	1Ϊ���꣬0Ϊƽ��
			else i=0;

			//����1
			if(m==4||m==6||m==9||m==11)if(d==31)d=30;		//������С��ʱ��������ǰ����Ϊ31 ������Ϊ30
			if(m==2){if(d>28){if(i==1)d=29;if(i==0)d=28;}}	//������2��ʱ��������ǰ���ڴ���28��������Ϊ29��ƽ����Ϊ28

			//����2
//			if(m==1||m==3||m==5||m==7||m==8||m==10||m==12)if(d>31)d=1;		 //����ʱ������31��Ϊ0��������������ֵ��
//			if(m==4||m==6||m==9||m==11)if(d>30)d=1;							 //С��ʱ������31��Ϊ0��������������ֵ��
//			if(m==2&&i==1)if(d>29)d=1;										 //
//			if(m==2&&i==0)if(d>28)d=1;										 //

			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){	  //��A ����
			j=2;TM1640_led(0x01);OLED_DISPLAY_16x16(2,5*16,12);}	  
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)){	  //��B ����
			j=1;TM1640_led(0x20);OLED_DISPLAY_16x16(2,5*16,13);}	  
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)){	  //��C ����
			j=0;TM1640_led(0x80);OLED_DISPLAY_16x16(2,5*16,14);}

			if(c==1){	   //��ת��
				if(j==0){                          //d++;	   //����2					     		  
					switch(m){						 //����1
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						d++;if(d==32)d=1;break;			  //31����·�
	
					case 4:
					case 6:
					case 9:
					case 11:
						d++;if(d==31)d=1;break;			  //30����·�
	
					case 2:								  
						if(i==1){d++;if(d==30)d=1;};		 //����
						if(i==0){d++;if(d==29)d=1;};break;	 //ƽ��

					default:break;
					}
				}
				if(j==1){m++;if(m==13)m=1;}					//����
				if(j==2){y++;if(y==2100)y=1970;}			//����
			}
			
			if(c==2){
				if(j==0){              //{if(d==0)d=31;d--;}	 //����2
					switch(m){
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						if(d==1)d=32;d--;break;			  //31����·�
	
					case 4:
					case 6:
					case 9:
					case 11:
						if(d==1)d=31;d--;break;			  //30����·�
	
					case 2:								  
						if(i==1){if(d==1)d=30;d--;};		 //����
						if(i==0){if(d==1)d=29;d--;};break;	 //ƽ��

					default:break;
					}
				}
				if(j==1){if(m==1)m=13;m--;}					  //����
				if(j==2){if(y==1970)y=2100;y--;}			  //����
			}
			if(c==3)break;	  //��������ȷ���˳�

			TM1640_display(0,y/1000);		//��ʾ��ֵ
			TM1640_display(1,(y/100)%10);
			TM1640_display(2,(y/10)%10);
			TM1640_display(3,y%10);
			TM1640_display(4,m/10);
			TM1640_display(5,m%10);
			TM1640_display(6,d/10);
			TM1640_display(7,d%10);
			
			

		}

		if(RTC_Get()==0)RTC_Set(y,m,d,rhour,rmin,rsec+1); //д��	  +1��Ϊ�˱�֤ʱ��ӽ�ͬ�� ��Ϊд��ʱ����Ϊ1s
		OLED_DISPLAY_CLEAR(); //������
		TM1640_led(0x00);		 //ָʾ����
		
		
		
	
	
	
	
	}

