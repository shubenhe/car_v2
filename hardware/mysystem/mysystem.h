#ifndef _MYSYSTEM_H
#define _MYSYSTEM_H

#include "sys.h"
#include "ycjobject.h"
#include "led.h"
#include "24l01.h"
#include "pwm.h"
#include "direction.h"
#include "usart.h"
#include "main.h"

//�������ID
#define ID_Timer					1
#define ID_NakedSystem		2



//������
struct TTimer;
struct TNakedSystem;

// ���庯��ָ��
typedef void (*tvf_nakedsystem)(pobject(TNakedSystem,obj_S));
typedef void (*tvf_nakedsystem_t)(pobject(TNakedSystem,obj_S),pobject(TTimer,obj_T));
typedef u8 (*tu8f_timer_s)(pobject(TTimer,obj),pobject(TNakedSystem,obj_S),u32 set_value);
typedef void (*tvfv)(void);
typedef u8 (*tu8fv)(void);

//��ʱ����
struct TTimer
{
	Const
		tu8fv ID;					// ������ͱ�ʶ
	Variable
		_Bool *BaseFlag;			//ʱ������ָ��
		u32 Set;					//��ʱ����ֵ
		u32 Counter;				//��ʱ����
	Method
		tu8f_timer_s DELAY;			//��ʱ����
};

//��ϵͳ��
struct TNakedSystem
{
	Const
		tu8fv ID;		// ������ͱ�ʶ
	Variable
		_Bool Statistics_Switch;		//ͳ�ƿ���ָ��
		u32 Loop_Times_Counter;	//ѭ������ͳ�Ƽ�����
		u32 LoopTime_MAX;		//��ѭ�����ʱ�䣬��λus
		u32 LoopTime_MIN;		//��ѭ����Сʱ�䣬��λus
		u32 LoopTime_MEAN_1S;		//1������ѭ��ƽ��ʱ�䣬��λus,ϵͳ����Լ������ʱΪ1ms,��ɱ�ʾ����ռ��ϵͳ��Դ��ǧ�ֱ���
		_Bool T_1ms_flag;		//1msʱ����
		_Bool T_1ms_flag_M;		//1msʱ����Ӱ��
	Method
		tvfv Timer_INIT;		//��ʱ����ʼ������
		tvf_nakedsystem_t RUN;		//��ѭ���й������к���
		tvf_nakedsystem SET_T_Flag;		//��λʱ���ʶ����
};


void timer_Create(pobject(TTimer,obj));
void nakedsystem_Create(pobject(TNakedSystem,obj));

#endif
