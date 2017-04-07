#ifndef __SPEED_H
#define __SPEED_H

#include "sys.h"

extern u8  TIM2CH3_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM2CH3_CAPTURE_VAL;	//输入捕获值
extern u8  TIM2CH4_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM2CH4_CAPTURE_VAL;	//输入捕获值
extern u16 s_temp_l;						//当前左轮速度
extern u16 s_temp_r;						//当前右轮速度


void speed_init(void);

#endif
