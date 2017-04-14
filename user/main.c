#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "pwm.h"
#include "pid.h"
#include "key.h"
#include "direction.h"
#include "speed.h"
#include "24l01.h"   
#include "led.h"   
#include "main.h"
#include "ycjobject.h"
#include "car.h"
#include "mysystem.h"

#define speed_min 2000
#define speed_max 12000
#define speed_range 100
#define pwm_min 3600
#define pwm_max 60000


extern	u8 hopping[5];
extern	u8 hopping_turn;

//申请对象空间——系统管理
object(TNakedSystem,MySystem);

int main(void)
{
	
	//申请对象空间——绿灯
	object(TLight,Light_Green);
	//红灯
	object(TLight,Light_Red);
	//无线通信
	object(TWireless_Com,Wireless_Com);
	//左车轮
	object(TWheel,Wheel_Left);
	//右车轮
	object(TWheel,Wheel_Right);
	//定时器1
	object(TTimer,TIMER1);
	//定时器_系统管理
	object(TTimer,TIMER_SYS);
	
	//创建对象——系统管理
	create(nakedsystem_Create,&MySystem);
	//绿灯
	create(light_Create,&Light_Green);
	Light_Green.ID = light_green_ID;
	Light_Green.INIT = light_green_INIT;
	//红灯
	create(light_Create,&Light_Red);
	Light_Red.ID = light_red_ID;
	Light_Red.INIT = light_red_INIT;
	//无线通信
	create(wireless_com_Create,&Wireless_Com);
	//左车轮
	create(wheel_Create,&Wheel_Left);
	Wheel_Left.ID = wheel_left_ID;
	//右车轮
	create(wheel_Create,&Wheel_Right);
	Wheel_Right.ID = wheel_right_ID;
	//定时器1
	create(timer_Create,&TIMER1);
	TIMER1.BaseFlag = &(MySystem.T_1ms_flag);
	//定时器_系统管理
	create(timer_Create,&TIMER_SYS);
	TIMER_SYS.BaseFlag = &(MySystem.T_1ms_flag);
	
	//初始化
	delay_init();
	NVIC_Configuration();
#if UART_SWITCH
	uart_init(115200);
#endif
	Wheel_Left.INIT();
	Light_Red.INIT();	//初始化与LED连接的硬件接口
	Light_Green.INIT();
	Wireless_Com.INIT();	//初始化NRF24L01
	MySystem.Timer_INIT();
	 
	while(NRF24L01_Check())	//检查NRF24L01是否在位.
	{
		while(1);
	}
	
	while(!(TIMER1.DELAY(&TIMER_SYS,&MySystem,1000)));
	
	NRF24L01_RX_Mode();
	
	while(1)
	{
		MySystem.RUN(&MySystem,&TIMER_SYS);
		
		if(Wireless_Com.Get_Message(&Wireless_Com) ==0)
		{	//一旦接收到信息
			Light_Red.ON(&Light_Red);	//红灯亮
			com_to_wheel(&Wireless_Com,&Wheel_Left,&Wheel_Right);	//分析指令转换为命令
			Wheel_Left.Run(&Wheel_Left);	//执行命令
			Wheel_Right.Run(&Wheel_Right);	//执行命令
		}
		else
		{	//未收到信号
			Light_Red.OFF(&Light_Red);	//红灯灭
		}
		
		if(TIMER1.DELAY(&TIMER1,&MySystem,100))
		{
			if(Status_Light_OFF == Light_Green.Status)
				Light_Green.ON(&Light_Green);
			else
				Light_Green.OFF(&Light_Green);
		}
		
	 }
}


