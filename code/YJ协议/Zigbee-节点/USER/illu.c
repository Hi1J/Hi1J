#include "illu.h"

u8  BUF[8];
u16 illu_val = 0;
/*****************************************************
�������ƣ�Illu_Gpio_Init
�������ܣ�����ǿ�ȴ�����BH1750ʹ�õ�GPIO��ʼ��
			IICͨ��
������������
��������ֵ����
��ע��
	���ڰ����м�Ĵ������ӿھ���
	PB12---SCL
	PB13---SDA
	���ڰ����ұߵĴ������ӿھ���
	PB6----SCL
	PB7----SDA
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
void ILLU_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SDA_CLK|SCL_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_Init(SDA_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SCL_PIN;
	GPIO_Init(SCL_PORT,&GPIO_InitStructure);
}

/*****************************************************
�������ƣ�SDA_Gpio_init
�������ܣ�IICͨ��SDA����ģʽ����
����������int flag
		flag == 1--��SDA��������Ϊ���ģʽ
		flag == 0--��SDA��������Ϊ����ģʽ
��������ֵ����
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
void SDA_Gpio_init(int flag)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	if(flag == 1)
	{
		//���
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	}
	else if(flag == 0)
	{
		//����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	}
	GPIO_Init(SDA_PORT,&GPIO_InitStructure);
}

/*****************************************************
�������ƣ�IIC_Start
�������ܣ�IICͨ����ʼ�ź�
������������
��������ֵ����
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
static void IIC_Start(void)
{
  SDA_Gpio_init(1);    //SDAΪдģʽ
  LIGHT_SDA(1);//SDA����
  LIGHT_SCL(1);//SCL����

  delay_us(5); //��ʱ5us

  LIGHT_SDA(0);//SDA����
  delay_us(5); //��ʱ5us
  LIGHT_SCL(0);//SCL����

}

/*****************************************************
�������ƣ�IIC_Start
�������ܣ�IICͨ��ֹͣ�ź�
������������
��������ֵ����
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
static void IIC_Stop(void)
{
  SDA_Gpio_init(1);    //SDAΪдģʽ
  LIGHT_SDA(0);//SDA����
  LIGHT_SCL(1);//SCL����
  delay_us(5); //��ʱ5us
  LIGHT_SDA(1);//SDA���� 
  delay_us(5); //��ʱ5us
}


/*****************************************************
�������ƣ�IIC_SendACK
�������ܣ�IICͨ��Ӧ���ź�
����������u8 ack
			0:ACK 
			1:NAK
��������ֵ����
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
void IIC_SendACK(u8 ack)
{
    SDA_Gpio_init(1);
    LIGHT_SDA(ack);                  //дӦ���ź�
    LIGHT_SCL(1);                    //����ʱ����
    delay_us(5);             //��ʱ5us
    LIGHT_SCL(0);                    //����ʱ����
    delay_us(5);             //��ʱ
}

/*****************************************************
�������ƣ�IIC_RecvACK
�������ܣ�IICͨ�Ž���Ӧ���ź�
������������
��������ֵ������Ӧ���ź�
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
u8 IIC_RecvACK(void)
{
    u8 ack = 0;
    
    SDA_Gpio_init(0);                    //SDAΪ��ģʽ

    LIGHT_SCL(1);              //����ʱ����
    delay_us(10);       //��ʱ
    ack = IN_SDA;            //��Ӧ���ź�
    LIGHT_SCL(0);              //����ʱ����
    delay_us(10);       //��ʱ

    SDA_Gpio_init(1);
    return ack;
}


/*****************************************************
�������ƣ�IIC_SendByte
�������ܣ���IIC���߷���һ���ֽ�����
����������u8 dat Ҫ���͵�����
��������ֵ������Ӧ���ź�
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
u8 IIC_SendByte(u8 dat)
{
    u8 ack = 0;
    u8 i;
    SDA_Gpio_init(1);                    //SDAΪдģʽ
    for (i=0; i<8; i++)         //8λ������
    {
      if(dat & 0x80)            //�����ݿ�
        LIGHT_SDA(1);
      else
        LIGHT_SDA(0);
      dat <<= 1;                //�Ƴ����ݵ����λ              
      LIGHT_SCL(1);            //����ʱ����
      delay_us(5);     //��ʱ        
      LIGHT_SCL(0);            //����ʱ����
      delay_us(5);     //��ʱ
    }
    ack = IIC_RecvACK();
    return ack;
}


/*****************************************************
�������ƣ�IIC_RecvByte
�������ܣ���IIC���߽���һ���ֽ�����
������������
��������ֵ�����ض�ȡ��������
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
u8 IIC_RecvByte(void)
{
    unsigned char i;
    unsigned char dat = 0;

    LIGHT_SDA(1);                    //ʹ���ڲ�����,׼����ȡ����,
    SDA_Gpio_init(0);                    //SDAΪ��ģʽ
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        LIGHT_SCL(1);                //����ʱ����
        delay_us(5);             //��ʱ
        dat |= GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN);             //������               
        LIGHT_SCL(0);                //����ʱ����
        delay_us(5);             //��ʱ
    }
    SDA_Gpio_init(1);
    return dat;
}

/*****************************************************
�������ƣ�Single_Write_BH1750
�������ܣ���BH1750������д��һ�ֽ�����
����������u8 REG_Address
			Ҫд��ļĴ�����ַ
��������ֵ����
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
void Single_Write_BH1750(u8 REG_Address)
{
    IIC_Start();                  //��ʼ�ź�
    IIC_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�              //û�н��յ�Ӧ���źţ�����0xff
    IIC_SendByte(REG_Address);   //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
                     //û���յ�Ӧ���źţ�����0xfe

  //  BH1750_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
    IIC_Stop();                   //����ֹͣ�ź�
}



/*****************************************************
�������ƣ�Multiple_Read_BH1750
�������ܣ���������BH1750�ڲ�����
������������
��������ֵ����
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
void Multiple_Read_BH1750(void)
{   
    u8 i;	
    IIC_Start();                          //��ʼ�ź�
    IIC_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
     
	for (i=0; i<2; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = IIC_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 1)
        {
           IIC_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {		
          IIC_SendACK(0);                //��ӦACK
        }
   }
   IIC_Stop();                          //ֹͣ�ź�
   delay_ms(5);                           //��ʱ5ms
}


/*****************************************************
�������ƣ�ILLU_ReadData
�������ܣ�BH1750��ȡ����ǿ������
������������
��������ֵ������ֵ
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
u16 ILLU_ReadData(void)
{
   Single_Write_BH1750(0x01);   // power on
   Single_Write_BH1750(0x10);   // H- resolution mode
   delay_ms(180);               //��ʱ180ms
   Multiple_Read_BH1750();      //�����������ݣ��洢��BUF��  

   return  (u16)(((BUF[0]<<8)+BUF[1])/1.2);
} 

/**********************


        

*******************************
�������ƣ�ILLU_Show_Usart
�������ܣ���ȡ�������ݲ����ڴ�����ʾ
������������
��������ֵ������ֵ�ǹ���ǿ��
��ע��
�汾��V1.0
���ߣ�ZZXYD
*******************************************************/
u16 ILLU_Show_Usart(void)
{
	illu_val = ILLU_ReadData();
	printf("illu_val = %dlx\r\n",illu_val);
	return illu_val;
}



