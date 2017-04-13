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

//定义身份ID
#define ID_LightGreen 		1
#define ID_LightRed				2
#define ID_Wireless_Com		3
#define ID_Wheel_Left			4
#define ID_Wheel_Right		5
#define ID_Timer					6
#define ID_NakedSystem		7

//定义灯状态
#define Status_Light_OFF	0
#define Status_Light_ON		1

//定义车轮方向
#define Direction_Stop				0
#define Direction_Forward			1
#define Direction_Backward		2

//类声明
struct TLight;
struct TWireless_Com;
struct TWheel;
struct TTimer;
struct TNakedSystem;

// 定义函数指针
typedef void (*tvf_light)(pobject(TLight,obj));
typedef void (*tvf_wheel)(pobject(TWheel,obj));
typedef void (*tvf_nakedsystem)(pobject(TNakedSystem,obj_S));
typedef void (*tvf_nakedsystem_t)(pobject(TNakedSystem,obj_S),pobject(TTimer,obj_T));
typedef u8 (*tu8f_wireless_com)(pobject(TWireless_Com,obj));
typedef u8 (*tu8f_timer)(pobject(TTimer,obj),u32 set_value);
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
		tvfv INIT;
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
		u8 Speed;			//速度分为100档
	Method
		tvf_wheel Run;
		tvfv INIT;
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

//定时器类
struct TTimer
{
	Const
		tu8fv ID;					// 类的类型标识
	Variable
		_Bool *BaseFlag;			//时基变量指针
		u32 Set;					//延时设置值
		u32 Counter;				//延时计数
	Method
		tu8f_timer DELAY;			//延时函数
};

//裸系统类
struct TNakedSystem
{
	Const
		tu8fv ID;		// 类的类型标识
	Variable
		_Bool Statistics_Switch;		//统计开关指针
		u32 Loop_Times_Counter;	//循环次数统计计数器
		u32 LoopTime_MAX;		//主循环最大时间，单位us
		u32 LoopTime_MIN;		//主循环最小时间，单位us
		u32 LoopTime_MEAN_1S;		//1秒内主循环平均时间，单位us,系统运行约定最大耗时为1ms,则可表示程序占用系统资源的千分比例
		_Bool T_1ms_flag;		//1ms时间标记
		_Bool T_1ms_flag_M;		//1ms时间标记影子
	Method
		tvfv Timer_INIT;		//定时器初始化函数
		tvf_nakedsystem_t RUN;		//主循环中功能运行函数
		tvf_nakedsystem SET_T_Flag;		//置位时间标识函数
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
