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
