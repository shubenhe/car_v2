#ifndef _KEY_H
#define _KEY_H

#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY0 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)

#include "sys.h"


void key_init(void);



#endif
