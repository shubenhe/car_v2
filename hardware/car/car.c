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
// 参数：obj，TLight类型，对类成员本身的引用
// 返回：左车轮标识
// 备注：
u8 wheel_left_ID(void)
{
	return ID_Wheel_Left;
}

// 功能：返回右车轮标识
// 参数：obj，TLight类型，对类成员本身的引用
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
	{//原地
		if(obj_Wl->Channel[1]>135)
		{//左转
			obj_L->Direction = Direction_Stop;
			obj_R->Direction = Direction_Forward;
			s_r_temp = (obj_Wl->Channel[1]-135u)*s_r_temp/(250-135);
			s_l_temp = 0;
		}
		else if(obj_Wl->Channel[1]<115)
		{//右转
			obj_L->Direction = Direction_Forward;
			obj_R->Direction = Direction_Stop;
			s_l_temp = (115u-obj_Wl->Channel[1])*s_l_temp/115;
			s_r_temp = 0;
		}
		else
		{//停
			obj_L->Direction =  obj_R->Direction = Direction_Stop;
			s_l_temp = s_r_temp = 0;
		}
	}
	obj_L->Speed = s_l_temp;
	obj_R->Speed = s_r_temp;
}
