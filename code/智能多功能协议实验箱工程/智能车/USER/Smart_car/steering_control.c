#include "steering_control.h"
//ת����� �������

u16 Steering_current = Mid_value;

u16 Steering_Control(u8 value)
{
	switch(value)
	{
		case 0://��ת
			Steering_current--;
			if(Steering_current <= Right_Max)
			{Steering_current = Right_Max;}
			break;
		case 1://��ת
			Steering_current++;
			if(Steering_current >= Left_Max)
			{Steering_current = Left_Max;}
			break;
		case 2://ֱ��
			Steering_current = Mid_value;
			break;
		case 3://���򲻱�
			break;
	}
	return Steering_current;

}


void Car_Steering(u8 value)
{
	switch(value)
	{
		case 0x00://ֱ��
			TIM3->CCR3 = Steering_Control(2);
		break;
		case 0x01://��ת
			TIM3->CCR3 = Steering_Control(1);
		break;
		case 0x10://��ת
			TIM3->CCR3 = Steering_Control(0);
		break;
		case 0x11:
			TIM3->CCR3 = Steering_Control(3);
		break;
		default:
			break;
	
	}

}