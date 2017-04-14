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

//定义身份ID
#define ID_Timer					1
#define ID_NakedSystem		2



//类声明
struct TTimer;
struct TNakedSystem;

// 定义函数指针
typedef void (*tvf_nakedsystem)(pobject(TNakedSystem,obj_S));
typedef void (*tvf_nakedsystem_t)(pobject(TNakedSystem,obj_S),pobject(TTimer,obj_T));
typedef u8 (*tu8f_timer_s)(pobject(TTimer,obj),pobject(TNakedSystem,obj_S),u32 set_value);
typedef void (*tvfv)(void);
typedef u8 (*tu8fv)(void);

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
		tu8f_timer_s DELAY;			//延时函数
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


void timer_Create(pobject(TTimer,obj));
void nakedsystem_Create(pobject(TNakedSystem,obj));

#endif
