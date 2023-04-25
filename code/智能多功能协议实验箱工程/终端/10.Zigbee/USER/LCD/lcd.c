#include "lcd.h"
#include "font.h"
#include "delay.h"
#include "math.h"
#include "stdio.h"


/****************************************************
��������Delay
�βΣ� ��
����ֵ����
�������ܣ���ʱ����
****************************************************/
static void Delay(u32 time)
{    
   u16 i=0;  
   while(time--)
   {
	  i=12000;  //�Լ�����
	  while(i--) ;    
   }
}

//����LCD��Ҫ������Ĭ��Ϊ����
_lcd_dev lcddev;
/****************************************************
��������LCD_WR_REG
�βΣ�  regval:�Ĵ���ֵ
����ֵ����
�������ܣ�д�Ĵ�������
****************************************************/
void LCD_WR_REG(u16 regval)
{ 
	LCD->LCD_REG=regval;//д��Ҫд�ļĴ������	 
}
/****************************************************
��������LCD_WR_DATA
�βΣ� data:Ҫд���ֵ
����ֵ����
�������ܣ�дLCD����
****************************************************/
void LCD_WR_DATA(u16 data)
{										    	   
	LCD->LCD_RAM=data;		 
}
/****************************************************
��������LCD_RD_DATA
�βΣ���
����ֵ����ȡ��16λ��ɫֵ
�������ܣ���LCD����
****************************************************/
u16 LCD_RD_DATA(void)
{										    	   
	return LCD->LCD_RAM;		 
}					   
/****************************************************
��������LCD_WriteReg
�βΣ�LCD_Reg:�Ĵ�����ַ   LCD_RegValue:Ҫд�������
����ֵ����
�������ܣ�д�Ĵ���
****************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������		 
	LCD->LCD_RAM = LCD_RegValue;//д������	    		 
}	   
/****************************************************
��������LCD_ReadReg
�βΣ�LCD_Reg:�Ĵ�����ַ
����ֵ������������
�������ܣ����Ĵ���
****************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return LCD_RD_DATA();		//���ض�����ֵ
}   
/****************************************************
��������LCD_WriteRAM_Prepare
�βΣ���
����ֵ����
�������ܣ���ʼдGRAM
****************************************************/
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	
/****************************************************
��������LCD_WriteRAM
�βΣ�RGB_Code:��ɫֵ
����ֵ����
�������ܣ�LCDдGRAM
****************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
}
/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM4 ���� TFT����ַ��ΧΪ0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A23 ��LCD��RS(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6C00 0000
RAM����ַ = 0X6D00 0000 = 0X6C00 0000+2^23*2 = 0X6C00 0000 + 0X100 0000 = 0X6D00 0000
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/
/****************************************************
��������LCD_GPIO_Config
�βΣ���
����ֵ����
�������ܣ���ʼ��LCD���ӿڵ�GPIO��
****************************************************/
void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ��FSMCʱ��*/
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOD \
	 					  |RCC_APB2Periph_GPIOE |RCC_APB2Periph_GPIOG,\
	                       ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* ����LCD������ƹܽ�*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* ����LCD��λ���ƹܽ�*/
    //�õ���RESET�ܽ�


	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	/* ����FSMC���Ӧ��������,FSMC-D0~D15: PD 14 15 0 1 8 9 10,PE 7 8 9 10 11 12 13 14 15*/	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1\
	                              |GPIO_Pin_8| GPIO_Pin_9\
	                              |GPIO_Pin_10|GPIO_Pin_14\
	                              |GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8\
	 							| GPIO_Pin_9 | GPIO_Pin_10\
	 						    | GPIO_Pin_11 | GPIO_Pin_12\
	 						    | GPIO_Pin_13 | GPIO_Pin_14\
	   						    | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* ����FSMC���Ӧ�Ŀ�����
	* PD4-FSMC_NOE   :LCD-RD
	* PD5-FSMC_NWE   :LCD-WR
	* PG12-FSMC_NE4  :LCD-CS
	* PE2-FSMC_A23   :LCD-DC
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//PD4-FSMC_NOE   :LCD-RD

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//PD5-FSMC_NWE   :LCD-WR
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //PG12-FSMC_NE4  :LCD-CS

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //PE2-FSMC_A10   :LCD-DC���͵����ݴ������������ʾ����

 		    
}
/****************************************************
��������LCD_FSMC_Config
�βΣ���
����ֵ����
�������ܣ�FSMC�ĳ�ʼ��
****************************************************/
void LCD_FSMC_Config(void)
{
	
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  WRITE_USER; 
	FSMC_NORSRAMTimingInitTypeDef  READ_USER; 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	READ_USER.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ��
    READ_USER.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    READ_USER.FSMC_DataSetupTime = 0x0f;		 //���ݽ���ʱ��
    READ_USER.FSMC_BusTurnAroundDuration = 0x00;
    READ_USER.FSMC_CLKDivision = 0x00;
    READ_USER.FSMC_DataLatency = 0x00;
    READ_USER.FSMC_AccessMode = FSMC_AccessMode_A;	 // 

	WRITE_USER.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ��
    WRITE_USER.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    WRITE_USER.FSMC_DataSetupTime = 0x03;		 //���ݽ���ʱ��
    WRITE_USER.FSMC_BusTurnAroundDuration = 0x00;//
    WRITE_USER.FSMC_CLKDivision = 0x00;//
    WRITE_USER.FSMC_DataLatency = 0x00;//
    WRITE_USER.FSMC_AccessMode = FSMC_AccessMode_A;	 // 
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;  //ѡ����Ӵ洢����ĵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//���������ߺ͵�ַ���Ƿ���
//    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;//������Ӵ洢��������
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//����FSMC�ӿڵ����ݿ��
	
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;//���÷���ģʽ
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;//���õȴ��źŵļ���
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;//�����Ƿ�ʹ�÷Ƕ��뷽ʽ
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;//���õȴ��ź�ʲôʱ�����
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;//�����Ƿ�ʹ�õȴ��ź�
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;//�����Ƿ�����ͻ��д����
	
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//����д����ʹ��
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable ;//�����Ƿ�ʹ����չģʽ
    
	
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &READ_USER;//��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &WRITE_USER;  //дʱ��
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* ʹ�� FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);

}
/****************************************************
��������LCD_init_RGB
�βΣ���
����ֵ����
�������ܣ�LCD��ILI9341�ĳ�ʼ��
****************************************************/
void LCD_init_RGB(void)
{
	Delay(50); // delay 50 ms 
	
 	LCD_WriteReg(0x0000,0x0001);
	 
	Delay(50); // delay 50 ms 
  	lcddev.id = LCD_ReadReg(0x0000);  
	 
  	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//����ID����ȷ,����lcddev.id==0X9300�жϣ���Ϊ9341��δ����λ������»ᱻ����9300
	{	
 		//����9341 ID�Ķ�ȡ		
		LCD_WR_REG(0XD3);				   
		LCD_RD_DATA(); 				//dummy read 	
 		LCD_RD_DATA();   	    	//����0X00
  		lcddev.id=LCD_RD_DATA();   	//��ȡ93								   
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();  	//��ȡ41 	   			   
 		
		if(lcddev.id!=0X9341)lcddev.id=0x9341;//����ʶ��9341 	     
	}
	 
	if(lcddev.id==0X9341)	//9341��ʼ��  ʹ�õ�����չģʽ
	{	 
		LCD_WR_REG(0xCF);      
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81);
		
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02);
		
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20);
		
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00);
		
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0XB7); 
		
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);
		
		LCD_WR_REG(0x11); //Exit Sleep
		Delay(120);
		LCD_WR_REG(0x29); //display on	
	}
}
/****************************************************
��������LCD_Init
�βΣ���
����ֵ����
�������ܣ�LCD���ĳ�ʼ��
****************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Config();
	
	LCD_FSMC_Config();
	
	LCD_init_RGB();
		
	/* ������ */
	LCD_DisplayOn();
	
     LCD_Display_Dir(0);		 	//����
		//LCD_Display_Dir(1);		 	//����
	
	LCD_Clear(WHITE);	
}


//Xpos:������

//Ypos:������
/****************************************************
��������LCD_SetCursor
�βΣ�u16 Xpos -- X����, u16 Ypos -- Y����
����ֵ����
�������ܣ�����LCD������ֵ
****************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{    
      if(lcddev.id==0X9341)
      {               
				LCD_WR_REG(lcddev.setxcmd);
				LCD_WR_DATA(Xpos>>8);
				LCD_WR_DATA(Xpos&0XFF);
				LCD_WR_DATA(240>>8);
				LCD_WR_DATA(240&0XFF);
				LCD_WR_REG(lcddev.setycmd);
				LCD_WR_DATA(Ypos>>8);
				LCD_WR_DATA(Ypos&0XFF);
				LCD_WR_DATA(320>>8);
				LCD_WR_DATA(320&0XFF);
      }
} 
/****************************************************
��������LCD_Scan_Dir
�βΣ�dir:0~7,����8������(���嶨���lcd.h) 
����ֵ����
�������ܣ�����LCD���Զ�ɨ�跽��һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.��
****************************************************/  
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u8 dirreg=0;
	u16 temp;  
	if(lcddev.dir==1&&lcddev.id!=0X6804)//����ʱ����6804���ı�ɨ�跽��
	{			   
		switch(dir)//����ת��
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}
	if(lcddev.id==0x9341||lcddev.id==0X6804)//9341/6804,������
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		dirreg=0X36;
 		regval|=0X08;//BGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804��BIT6��9341�ķ���	   
		LCD_WriteReg(dirreg,regval);
 		if((regval&0X20)||lcddev.dir==1)
		{
			if(lcddev.width<lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}  
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);
		LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);
		LCD_WR_DATA((lcddev.width-1)&0XFF);
		
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
  	}
} 
/****************************************************
��������LCD_Display_Dir
�βΣ�dir:0,������1,����
����ֵ����
�������ܣ�����LCD��ʾ����
****************************************************/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//����
	{
		lcddev.dir=0;	//����
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X6804)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}
	}
	else 				//����
	{	  				
		lcddev.dir=1;	//����
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}
/****************************************************
��������LCD_DrawPoint
�βΣ�x,y:����
����ֵ��POINT_COLOR:�˵����ɫ
�������ܣ����㺯��
****************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 c)
{	 
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD->LCD_RAM=c; 
}
/****************************************************
��������LCD_ReadPoint
�βΣ�x,y:����
����ֵ���˵����ɫ
�������ܣ���ȡ��ĳ�����ɫֵ
****************************************************/
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);	 
	
	if(lcddev.id==0X9341)LCD_WR_REG(0X2E);//9341/6804/3510 ���Ͷ�GRAMָ��	    
	if(LCD->LCD_RAM)r=0;							//dummy Read	   
	Delay(2);	  
 	r=LCD->LCD_RAM;  		  						//ʵ��������ɫ
 	if(lcddev.id==0X9341)		//9341/NT35310/NT35510Ҫ��2�ζ���
 	{
		Delay(2);	  
		b=LCD->LCD_RAM; 
		g=r&0XFF;		//����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g<<=8;
	}
	b=b;//��ֹ����������
	return g;
}
/****************************************************
��������LCD_DisplayOn
�βΣ���
����ֵ����
�������ܣ���LCD����ʾ����
****************************************************/
void LCD_DisplayOn(void)//����ʾ
{
	/* ������ */
	GPIO_SetBits(GPIOB, GPIO_Pin_0);	
}
/****************************************************
��������LCD_DisplayOff
�βΣ���
����ֵ����
�������ܣ��ر�LCD����ʾ����
****************************************************/
void LCD_DisplayOff(void)//����ʾ
{
	/*�ر��� */
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}



//��������
//color:Ҫ���������ɫ
/****************************************************
��������LCD_Clear
�βΣ�u16 color -- ��������ɫֵ
����ֵ����
�������ܣ�����
****************************************************/
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	   
	}
}  

/****************************************************
��������my_itoa_n
�βΣ�  num(Ҫת������)
        str(ת���õ�����ĵ�ַ)
        radix(���� 2 8 10 16)
����ֵ��
�������ܣ�������ת��Ϊ�ַ���
****************************************************/
char *my_itoa_n(int num,char *str,int radix)
{
	const char table[] = "0123456789ABCDEF";
	char *ptr = str;
	char *start = NULL;
    char temp;
	char negative = 0;
	if(num == 0)
	{
		*ptr++ = '0';
//		*ptr='\0';
		return str;
	}
	if(num<0)
	{
		*ptr++='-';
		num = num*(-1);
		negative = 1;
	}
	while(num)
	{
		*ptr++ = table[num % radix];
		num/=radix;
	}
//	*ptr = '\0';
	start = (negative ? str+1:str);
	ptr--;
	while(start<ptr)
	{
		temp = *start;
		*start = *ptr;
		*ptr = temp;
		start++;
		ptr--;
	}
	return str;
}

//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//��ʾ��ɫ 	    
	} 
}

/*
�������ܣ�����
����������x1,y1:�������
					x2,y2:�յ�����
					color����ɫֵ

����ֵ����
*/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol,color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}





//������ʾ����
void LCD_Address_Set(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(x_start>>8);
	LCD_WR_DATA(x_start&0XFF); 
	LCD_WR_DATA(x_end>>8);
	LCD_WR_DATA(x_end&0XFF); 
	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(y_start>>8);
	LCD_WR_DATA(y_start&0XFF);
	LCD_WR_DATA(y_end>>8);
	LCD_WR_DATA(y_end&0XFF); 	
  LCD_WriteRAM_Prepare();
}



/*
x -- x����ʼ����
y -- y����ʼ���� 
width -- ģ�Ŀ��
high -- ģ�ĸ߶�
p -- ��ģ
*/

uint16_t LCD_Color = BLACK;
uint16_t LCD_BackColor= WHITE;

void LCD_ShowxXx(uint16_t x, uint16_t y, uint16_t width, uint16_t high, char *p)
{
	uint8_t i = 0,j=0,k=0;
	LCD_Address_Set(x,y,x+width-1,y+high-1);
	for(i=0;i<high;i++)
	{
		for(j=0;j<width/8;j++)
		{
			for(k=0;k<8;k++)
			{
				if(*p & (0x80 >> k))
					LCD_WR_DATA(LCD_Color);
        else
					LCD_WR_DATA(LCD_BackColor);
			}
			p++;
		}
	}
}

/*
col -- ��ʼ����
page - ��ʼ��ҳ
p    - ָ��Ҫ��ʾ���ַ���
*/
/*
col -- ��ʼ����
page - ��ʼ��ҳ
p    - ָ��Ҫ��ʾ���ַ���
*/
#include "ff.h"

struct _FONT *PHZ = &font_hz1616;//���ú��ֵ�����
struct _FONT *PZF = &font_zf816; //�����ַ�������



void LCD_ShowStr(uint16_t col,uint16_t page,uint8_t *p)
{
	uint8_t hz_buff[72]="\0";//���溺�ֵ�ģ
	uint16_t newcol = col;
	uint16_t newpage = page;
	uint32_t offset = 0;
	struct _FONT *pfont = NULL;
	uint8_t *pfontaddr = NULL;
	FIL fil;            /* Pointer to a file object */
	UINT  rb; 
  u8 hz_flag = 0;//�����Ƿ�򿪱�־ 0 -- ��ʧ��  1 -- �򿪳ɹ�
  if (f_open(&fil, PHZ->hz_zf_addr.hzaddr, FA_READ) == FR_OK) 
	{
//		printf("��%d*%d���ֿ�ɹ���\r\n",PHZ->width,PHZ->high);
		hz_flag = 1;
	}
//	else 
//		printf("��%d*%d���ֿ�ʧ�ܣ�û�취��ʾ���֣�\r\n",PHZ->width,PHZ->high);
	while(*p)//�ж��ַ����Ƿ�Ϊ�� -- '\0'
	{
		if((*p > 0xA1) && hz_flag)//����
		{ 
      pfont = PHZ;	
      //�ֿ�ƫ��λ��         
			offset = ((*p-0xa1)*94+(*(p+1)-0xa1))*pfont->high*pfont->width/8;;//ģ��ƫ��λ��
			f_lseek(&fil,offset);
			f_read(&fil,hz_buff,pfont->high*pfont->width/8,&rb);
//			printf("����%d���ֽ�\r\n",rb);
			pfontaddr = hz_buff;
			p+=2;
		}
		else
		{
			pfont = PZF;
			offset = Font_ZfFind(*p);
			if(offset != 0xff) 
			{
				offset *= pfont->width*pfont->high/8;
				pfontaddr = (uint8_t *)(pfont->hz_zf_addr.zfaddr+offset);
			}
			p++;//ָ������ƶ�
		}
		LCD_ShowxXx(newcol,newpage,pfont->width,pfont->high,(char *)pfontaddr);
		newcol += pfont->width;
		if(newcol > (LCD_WIDTH - pfont->width))
		{
			newcol = 0;
			newpage += pfont->high;
		}
	}
	f_close(&fil);
}



