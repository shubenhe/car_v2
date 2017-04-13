#ifndef _CAR_H
#define _CAR_H

#include "sys.h"
#include "ycjobject.h"
#include "led.h"
#include "24l01.h"
#include "pwm.h"
#include "direction.h"
#include "usart.h"
#include "main.h"

//�������ID
#define ID_LightGreen 		1
#define ID_LightRed				2
#define ID_Wireless_Com		3
#define ID_Wheel_Left			4
#define ID_Wheel_Right		5
#define ID_Timer					6
#define ID_NakedSystem		7

//�����״̬
#define Status_Light_OFF	0
#define Status_Light_ON		1

//���峵�ַ���
#define Direction_Stop				0
#define Direction_Forward			1
#define Direction_Backward		2

//������
struct TLight;
struct TWireless_Com;
struct TWheel;
struct TTimer;
struct TNakedSystem;

// ���庯��ָ��
typedef void (*tvf_light)(pobject(TLight,obj));
typedef void (*tvf_wheel)(pobject(TWheel,obj));
typedef void (*tvf_nakedsystem)(pobject(TNakedSystem,obj_S));
typedef void (*tvf_nakedsystem_t)(pobject(TNakedSystem,obj_S),pobject(TTimer,obj_T));
typedef u8 (*tu8f_wireless_com)(pobject(TWireless_Com,obj));
typedef u8 (*tu8f_timer)(pobject(TTimer,obj),u32 set_value);
typedef void (*tvfv)(void);
typedef u8 (*tu8fv)(void);

// ָʾ����
struct TLight
{
	Const
		tu8fv ID;
	Variable
		_Bool Status;
	Method
		tvfv INIT;
		tvf_light ON;
		tvf_light OFF;
};

// ������
struct TWheel
{
	Const
		tu8fv ID;
	Variable
		u8 Direction;
		u8 Speed;			//�ٶȷ�Ϊ100��
	Method
		tvf_wheel Run;
		tvfv INIT;
};

// ����ͨ����
struct TWireless_Com
{
	Const
		tu8fv ID;
	Variable
		u8 *Channel;
		u8 Status;
	Method
		tvfv INIT;
		tu8f_wireless_com Get_Message;
};

// ����
struct TCar
{
	Const
		tu8fv ID;					// ������ͱ�ʶ
	Variable
		
};

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
		tu8f_timer DELAY;			//��ʱ����
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


void light_Create(pobject(TLight,obj));
u8 light_green_ID(void);
u8 light_red_ID(void);
void wireless_com_Create(pobject(TWireless_Com,obj));
void wheel_Create(pobject(TWheel,obj));
u8 wheel_left_ID(void);
u8 wheel_right_ID(void);
void light_green_INIT(void);
void light_red_INIT(void);
void com_to_wheel(pobject(TWireless_Com,obj_Wl),pobject(TWheel,obj_l),pobject(TWheel,obj_r));
void timer_Create(pobject(TTimer,obj));
void nakedsystem_Create(pobject(TNakedSystem,obj));

#endif
