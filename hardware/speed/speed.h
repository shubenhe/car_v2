#ifndef __SPEED_H
#define __SPEED_H

#include "sys.h"

extern u8  TIM2CH3_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM2CH3_CAPTURE_VAL;	//���벶��ֵ
extern u8  TIM2CH4_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM2CH4_CAPTURE_VAL;	//���벶��ֵ
extern u16 s_temp_l;						//��ǰ�����ٶ�
extern u16 s_temp_r;						//��ǰ�����ٶ�


void speed_init(void);

#endif
