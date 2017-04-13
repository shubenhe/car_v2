#include "car.h"


// 功能：返回绿灯标识
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：绿灯标识
// 备注：
u8 light_green_ID(void)
{
	return ID_LightGreen;
}

// 功能：返回红灯标识
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：红灯标识
// 备注：
u8 light_red_ID(void)
{
	return ID_LightRed;
}

// 功能：红灯初始化
// 参数：无
// 返回：无
// 备注：
void light_red_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

// 功能：绿灯初始化
// 参数：无
// 返回：无
// 备注：
void light_green_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}

// 功能：打开灯
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：无
// 备注：
void light_ON(pobject(TLight,obj))
{
	
	obj->Status = Status_Light_ON;
	
	if(obj->ID() == ID_LightGreen)
	{
		LED1 = 0;
	}
	else if(obj->ID() == ID_LightRed)
	{
		LED0 = 0;
	}
}

// 功能：关闭灯
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：无
// 备注：
void light_OFF(pobject(TLight,obj))
{
	obj->Status = Status_Light_OFF;
	
	if(obj->ID() == ID_LightGreen)	LED1 = 1;
	else if(obj->ID() == ID_LightRed)	LED0 = 1;
}

// 功能：灯类创建
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：无
// 备注：
void light_Create(pobject(TLight,obj))
{
	//常量初始化
	
	//变量初始化
	obj->Status = Status_Light_OFF;
	
	//行为初始化
	obj->ON = light_ON;
	obj->OFF = light_OFF;
};

// 功能：返回无线通信标识
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：绿灯标识
// 备注：
u8 wireless_com_ID(void)
{
	return ID_Wireless_Com;
}

// 功能：无线通信初始化
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：无
// 备注：
void wireless_com_INIT(void)
{
	NRF24L01_Init();
}

u8 CH_message[RX_PLOAD_WIDTH];

// 功能：获取无线通信数据
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：	0，获得数据
//				1，未获得数据
// 备注：
u8 get_message(pobject(TWireless_Com,obj))
{
	return NRF24L01_RxPacket(obj->Channel);
}

// 功能：无线通信类创建
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：无
// 备注：
void wireless_com_Create(pobject(TWireless_Com,obj))
{
	//常量初始化
	obj->ID = wireless_com_ID;
	
	//变量初始化
	obj->Channel = CH_message;
	
	//行为初始化
	obj->INIT = wireless_com_INIT;
	obj->Get_Message = get_message;
}

// 功能：返回左车轮标识
// 参数：无
// 返回：左车轮标识
// 备注：
u8 wheel_left_ID(void)
{
	return ID_Wheel_Left;
}

// 功能：返回右车轮标识
// 参数：无
// 返回：右车轮标识
// 备注：
u8 wheel_right_ID(void)
{
	return ID_Wheel_Right;
}

// 功能：车轮控制初始化
// 参数：无
// 返回：无
// 备注：
void wheel_INIT(void)
{
	 pwm_init();
	 direction_init();
}

// 功能：车轮运行
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：无
// 备注：
void wheel_Run(pobject(TWheel,obj))
{
	if(obj->ID() == ID_Wheel_Left)
	{		//左轮
		TIM_SetCompare2(TIM8,(obj->Speed)*600u);
		
		if(obj->Direction == Direction_Stop)
		{
			PCout(6) = 0;
			PBout(0) = 0;
		}
		else if(obj->Direction == Direction_Forward)
		{
			PCout(6) = 1;
			PBout(0) = 0;
		}
		else if(obj->Direction == Direction_Backward)
		{
			PCout(6) = 0;
			PBout(0) = 1;
		}
	}
	else if(obj->ID() == ID_Wheel_Right)
	{		//右轮
		TIM_SetCompare3(TIM8,(obj->Speed)*600u);
		
		if(obj->Direction == Direction_Stop)
		{
			PBout(1) = 0;
			PBout(2) = 0;
		}
		else if(obj->Direction == Direction_Forward)
		{
			PBout(1) = 1;
			PBout(2) = 0;
		}
		else if(obj->Direction == Direction_Backward)
		{
			PBout(1) = 0;
			PBout(2) = 1;
		}
	}
}

// 功能：车轮类创建
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：无
// 备注：
void wheel_Create(pobject(TWheel,obj))
{
	//常量初始化
	
	//变量初始化
	obj->Direction = Direction_Stop;
	obj->Speed = 0;
	//行为初始化
	obj->Run = wheel_Run;
	obj->INIT = wheel_INIT;
}

// 功能：转化命令为轮子动作
// 参数：	obj_L,TWheel类指针，左轮
//			obj_R,TWheel类指针，右轮
//			obj_Wl，TWireless_Com类指针
//			obj_Wl->Channel[0],油门，0-250
//			obj_Wl->Channel[1]，前后方向，0-250
//			obj_Wl->Channel[2]，左右方向，0-250
// 返回：无
// 备注：
void com_to_wheel(pobject(TWireless_Com,obj_Wl),pobject(TWheel,obj_L),pobject(TWheel,obj_R))
{
	u16 s_l_temp = 0;
	u16 s_r_temp = 0;
	
	s_l_temp = s_r_temp = obj_Wl->Channel[2]*100u/250;

	if(obj_Wl->Channel[0]>135)
	{//往前
		obj_L->Direction =  obj_R->Direction = Direction_Forward;
		
		if(obj_Wl->Channel[1]>135)//往左
		{
			s_r_temp = ((obj_Wl->Channel[0]-135u)+(obj_Wl->Channel[1]-135u))*s_r_temp/(250-135);
			s_l_temp = (obj_Wl->Channel[0]-135u)*s_l_temp/(250-135);
		}
		else if(obj_Wl->Channel[1]<115)//往右
		{
			s_l_temp = ((obj_Wl->Channel[0]-135u)+(115u-obj_Wl->Channel[1]))*s_l_temp/(250-135);
			s_r_temp = (obj_Wl->Channel[0]-135u)*s_r_temp/(250-135);
		}
		else
		{
			s_l_temp = (obj_Wl->Channel[0]-135u)*s_l_temp/(250-135);
			s_r_temp = (obj_Wl->Channel[0]-135u)*s_r_temp/(250-135);
		}
	}
	else if(obj_Wl->Channel[0]<115)
	{//往后
		obj_L->Direction =  obj_R->Direction = Direction_Backward;

		if(obj_Wl->Channel[1]>135)//往左
		{
			s_l_temp = ((115u-obj_Wl->Channel[0])+(obj_Wl->Channel[1]-135u))*s_l_temp/(250-135);
			s_r_temp = (115u-obj_Wl->Channel[0])*s_r_temp/(250-135);
		}
		else if(obj_Wl->Channel[1]<115)//往右
		{
			s_r_temp = ((115u-obj_Wl->Channel[0])+(115u-obj_Wl->Channel[1]))*s_r_temp/(250-135);
			s_l_temp = (115u-obj_Wl->Channel[0])*s_l_temp/(250-135);
		}
		else
		{
			s_l_temp = (115u-obj_Wl->Channel[0])*s_l_temp/115;
			s_r_temp = (115u-obj_Wl->Channel[0])*s_r_temp/115;
		}
	}
	else
	{// 原地
		if(obj_Wl->Channel[1]>135)
		{// 左转
			obj_L->Direction = Direction_Stop;
			obj_R->Direction = Direction_Forward;
			s_r_temp = (obj_Wl->Channel[1]-135u)*s_r_temp/(250-135);
			s_l_temp = 0;
		}
		else if(obj_Wl->Channel[1]<115)
		{// 右转
			obj_L->Direction = Direction_Forward;
			obj_R->Direction = Direction_Stop;
			s_l_temp = (115u-obj_Wl->Channel[1])*s_l_temp/115;
			s_r_temp = 0;
		}
		else
		{// 停
			obj_L->Direction =  obj_R->Direction = Direction_Stop;
			s_l_temp = s_r_temp = 0;
		}
	}
	obj_L->Speed = s_l_temp;
	obj_R->Speed = s_r_temp;
}

// 功能：返回计时器标识
// 参数：无
// 返回：左车轮标识
// 备注：
u8 timer_ID(void)
{
	return ID_Timer;
}

// 功能：计时器延时函数
// 参数：	obj，TTimer类型，对类成员本身的引用
//			set_value，u32类型，设置值
// 返回：	0，延时未到
//			1，延时到
// 备注：
u8 timer_delay(pobject(TTimer,obj_T),pobject(TNakedSystem,obj_S),u32 set_value)
{
	_Bool T_flag = 0;
	
	if(0 == obj_S->Statistics_Switch)
	{	//未进入主循环，复位时间标识
		T_flag = obj_S->T_1ms_flag_M;
		obj_S->T_1ms_flag_M = 0;
	}
	else
	{
		T_flag = *obj_T->BaseFlag;
	}
	
	if(set_value)
	{	//需延时
		if(0 == obj_T->Set)
		{	// 未设置延时时间
			obj_T->Set = set_value;
			obj_T->Counter = 0;
		}
	
		if(T_flag)
		{// 延时时基到
			if(obj_T->Counter < obj_T->Set)
			{// 延时未到
				obj_T->Counter++;
			}
			else
			{// 延时到
				obj_T->Set = obj_T->Counter = 0;	//	计时器复位
				
				return 1;
			}
		}
	}
	else
	{	//	不需延时
		return 1;
	}
	
	return 0;
}

// 功能：计时器类创建
// 参数：obj，TTimer类型，对类成员本身的引用
// 返回：无
// 备注：
void timer_Create(pobject(TTimer,obj))
{
	//常量初始化
	obj->ID = timer_ID;
	//变量初始化
	obj->Counter = 0;
	obj->Set = 0;
	//行为初始化
	obj->DELAY = timer_delay;
}

// 功能：返回系统类ID
// 参数：无
// 返回：系统类ID
// 备注：
u8 nakedsystem_ID(void)
{
	return ID_NakedSystem;
}

// 功能：主循环中系统处理函数，更新时间标记，统计系统使用率
// 参数：	obj_S，TNakedSystem类指针
//				obj_T，TTimer类指针
// 返回：无
// 备注：
void nakedsystem_run(pobject(TNakedSystem,obj_S),pobject(TTimer,obj_T))
{
	u32 one_time = 0;
	
	//更新时间标记
	obj_S->T_1ms_flag = obj_S->T_1ms_flag_M;
	obj_S->T_1ms_flag_M = 0;
	
	//统计
	if(obj_S->Statistics_Switch)
	{//统计已开始
		one_time = TIM_GetCounter(TIM6);
		TIM_SetCounter(TIM6,0);
		TIM_Cmd(TIM6, ENABLE);
		
		obj_S->Loop_Times_Counter++;
		
		if(obj_T->DELAY(obj_T,obj_S,1000))
		{//延时1秒执行平均统计
			obj_S->LoopTime_MEAN_1S = 1000000u/obj_S->Loop_Times_Counter;//更新平均值
			obj_S->Loop_Times_Counter = 0;		//复位开始下一次统计
#if UART_SWITCH
			printf("mean %dus---min %dus---max %dus\r\n",obj_S->LoopTime_MEAN_1S,obj_S->LoopTime_MIN,obj_S->LoopTime_MAX);
#endif
		}
		
		if(one_time > obj_S->LoopTime_MAX)
		{//更新最大值
			obj_S->LoopTime_MAX = one_time;
		}
		if(one_time < obj_S->LoopTime_MIN)
		{//更新最小值
			obj_S->LoopTime_MIN = one_time;
		}
	}
	else
	{//统计未开始
		obj_S->Loop_Times_Counter = 0;
		obj_S->LoopTime_MAX = 0;
		obj_S->LoopTime_MIN = 10000000;//10秒
		obj_S->LoopTime_MEAN_1S = 0;
		obj_S->Statistics_Switch = 1;
		TIM_SetCounter(TIM6,0);
		TIM_Cmd(TIM6, ENABLE);
	}
}

// 功能：置位时间标记
// 参数：	obj，TNakedSystem类指针
// 返回：无
// 备注：
void nakedsystem_set_t_flag(pobject(TNakedSystem,obj))
{
	obj->T_1ms_flag_M = 1;
}

#define SYS_LOOP_TIME_STATS_MAX 65000
// 功能：系统统计定时器初始化
// 参数：
// 返回：无
// 备注：
void nakedsystem_timer_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = SYS_LOOP_TIME_STATS_MAX; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到65536为65.536ms
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值  2Mhz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM6, //TIM6
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_Cmd(TIM6, DISABLE);  //使能TIMx外设RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 9; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到10为1ms
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
}

// 功能：TIM4定时器中断
// 参数：
// 返回：无
// 备注：

//声明系统类
extern struct TNakedSystem MySystem;
void TIM4_IRQHandler(void)   //TIM4中断
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			MySystem.SET_T_Flag(&MySystem);
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		}
}

// 功能：TIM6定时器中断
// 参数：
// 返回：无
// 备注：
void TIM6_IRQHandler(void)   //TIM4中断
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_Cmd(TIM6, DISABLE);  //使能TIMx外设
			TIM_SetCounter(TIM6,SYS_LOOP_TIME_STATS_MAX);
			
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		}
}

// 功能：系统类创建
// 参数：	obj，TNakedSystem类指针
// 返回：无
// 备注：
void nakedsystem_Create(pobject(TNakedSystem,obj))
{
	// 常量初始化
	obj->ID = nakedsystem_ID;
	// 变量初始化
	obj->Statistics_Switch = 0;
	obj->LoopTime_MAX = 0;
	obj->LoopTime_MEAN_1S = 0;
	obj->LoopTime_MIN = 10000000;
	obj->Loop_Times_Counter = 0;
	obj->T_1ms_flag = 0;
	obj->T_1ms_flag_M = 0;
	//	行为初始化
	obj->Timer_INIT = nakedsystem_timer_init;
	obj->RUN = nakedsystem_run;
	obj->SET_T_Flag = nakedsystem_set_t_flag;
}
