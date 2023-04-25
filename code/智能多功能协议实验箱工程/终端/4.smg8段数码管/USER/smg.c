#include "smg.h"



/****************************************
8λ8�ι����������
���룺
	A 	B 	C 	D 	E 	F 	G 	H
	PF0 PF1 PF2 PF3 PF4 PF5 PF6 PF7
λ�룺 
	1 	 2 	  3    4    5    6    7   8 
	PF15 PF14 PF13 PF12 PF11 PF10 PF9 PF8
ģʽ��ͨ���������
****************************************/
void Smg_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//Ĭ��ȫ��Ϩ��:ȫ������ߵ�ƽ
	GPIO_Write(GPIOF,0XFFFF);
}

//��ѡ����ʾ�ַ�
void Smg_SlectSegment(uint8_t code)
{
	uint8_t i = 0;
	for(i=0; i<8; i++)
	{
		if((code & 0x01))
			GPIOF->ODR |= (1<<i);
		else
			GPIOF->ODR &= ~(1<<i);
		code>>=1;
	}
}

//λѡ��ѡ��λ��
void Smg_SlectPosition(uint8_t num)
{
	GPIOF->ODR |= (0xFF<<8);
	GPIOF->ODR &= ~ (1<<(8+num));
}


//����ܶ�̬��ʾ   1ms-2ms����һ�ξͿ���������ʾ
//buff ��ʾ����    len ��ʾ����
void Digitron_Display(uint8_t * buff, uint8_t len)
{
	static uint8_t i = 0;
	if(len == 0)
		return;
	Smg_SlectSegment(0xFF);//����
	Smg_SlectPosition(7-i);
	Smg_SlectSegment(buff[i]);
	i++;
	i%=len;
}


//��smgcodebuff�ڴ��벻ͬ��������ֶΣ�����ʾ��ͬ�Ķ��� 01-23-45
uint8_t smgcodebuff[8] = {0xc0, 0xf9, 0xbf, 0xa4, 0xb0, 0xbf, 0x99, 0x92};
char smgshowbuff[17] = {0};//��smgshowbuff�ڴ��벻ͬ���ַ�������

// ��ʾת��  ���ַ�������ת�����������ʾ������
void Seg_Tran(char *pucSeg_Buf, uint8_t *pucSeg_Code)
{   
  uint8_t i, j=0, temp;

  for(i=0; i<8; i++, j++)
  {
    switch(pucSeg_Buf[j]) 
    { // �͵�ƽ�����Σ�����[MSB...LSB]��Ӧ��˳��Ϊ[dp g f e d c b a]
      case '0': temp = 0xc0; break;
      case '1': temp = 0xf9; break;
      case '2': temp = 0xa4; break;
      case '3': temp = 0xb0; break;
      case '4': temp = 0x99; break;
      case '5': temp = 0x92; break;
      case '6': temp = 0x82; break;
      case '7': temp = 0xf8; break;
      case '8': temp = 0x80; break;
      case '9': temp = 0x90; break;
      case 'A': temp = 0x88; break;
      case 'B': temp = 0x83; break;
      case 'C': temp = 0xc6; break;
      case 'D': temp = 0xA1; break;
      case 'E': temp = 0x86; break;
      case 'F': temp = 0x8E; break;
      case 'H': temp = 0x89; break;
      case 'L': temp = 0xC7; break;
      case 'N': temp = 0xC8; break;
      case 'P': temp = 0x8c; break;
      case 'U': temp = 0xC1; break;
      case '-': temp = 0xbf; break;
      case ' ': temp = 0xff; break;
			case '_': temp = 0xf7; break;
      default: temp = 0xff;
    }	
    if(pucSeg_Buf[j+1] == '.')
    {
      temp = temp&0x7f;
      j++;
    }
    pucSeg_Code[i] = temp;
  }
}


