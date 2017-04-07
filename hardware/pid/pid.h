#ifndef _PID_H
#define _PID_H

#include "sys.h"

void pid_init(void);
int pid(int e,int* e1,int* e2,float Kp,float Ti,float Td);


extern u16 speed_r;
extern u16 speed_l;
extern int el_1,el_2,er_1,er_2;
extern float Kp,Ti,Td;

#endif
