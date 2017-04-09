#ifndef _CAR_H
#define _CAR_H

#include "sys.h"
#include "ycjobject.h"
#include "led.h"
#include "24l01.h"

extern u8 CH[RX_PLOAD_WIDTH];

//定义身份ID
#define ID_LightGreen 	1
#define ID_LightRed			2
#define ID_Wireless_Com	3
#define ID_Wheel_Left		4
#define ID_Wheel_Right	5

//定义灯状态
#define Status_Light_OFF	0
#define Status_Light_ON		1

//类声明
struct TLight;
struct TWireless_Com;
struct TWheel;

// 定义函数指针
typedef void (*tvf_light)(pobject(TLight,obj));
typedef void (*tvf_wheel)(pobject(TWheel,obj));
typedef u8 (*tu8f_wireless_com)(pobject(TWireless_Com,obj));
typedef void (*tvfv)(void);
typedef u8 (*tu8fv)(void);

// 指示灯类
struct TLight
{
	Const
		tu8fv ID;
	Variable
		_Bool Status;
	Method
		tvf_light ON;
		tvf_light OFF;
};

// 车轮类
struct TWheel
{
	Const
		tu8fv ID;
	Variable
		u8 Direction;
		u8 Speed;
	Method
	 tvf_wheel Run;
};

// 无线通信类
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

// 车类
struct TCar
{
	Const
		tu8fv ID;					// 类的类型标识
	Variable
		
};


void light_Create(pobject(TLight,obj));
u8 light_green_ID(void);
u8 light_red_ID(void);
void wireless_com_Create(pobject(TWireless_Com,obj));
void wheel_Create(pobject(TWheel,obj));
u8 wheel_left_ID(void);
u8 wheel_right_ID(void);

#endif
