#ifndef __STEERING_CONTROL_H
#define __STEERING_CONTROL_H

#include "stm32f10x.h"



#define Left_Max 1700    //��ת���ֵ
#define Mid_value 1515  //��ֵ
#define Right_Max 1320 //��ת���ֵ

extern u16 Steering_current;

void Car_Steering(u8 value);

#endif
