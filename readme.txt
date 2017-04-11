速度调节引脚
PC7	左边电机转速
PC8	右边电机转速

速度采集引脚
PA2	左轮转速
PA3	右轮转速

方向控制引脚
PC6	左轮正转正信号
PB0	左轮正转负信号
PB1	右轮正转正信号
PB2	右轮正转正信号

PID调节记录
采样频率恰当是控制的前提，可以高，但参数要调整，高了有助于提高调整的速度


2015.6.13完成PID调节

170411 bug
/***************有问题，无线反应变慢************/
	//初始化
	delay_init();
	NVIC_Configuration();//***************************
	timer_init();//***************************
//	uart_init(9600);
	Wheel_Left.INIT();
	Light_Red.INIT();	//初始化与LED连接的硬件接口
	Light_Green.INIT();
	Wireless_Com.INIT();	//初始化NRF24L01

/**************无线变慢问题能解决**************/
	//初始化
	delay_init();
	NVIC_Configuration();//***************************
//	uart_init(9600);
	Wheel_Left.INIT();
	Light_Red.INIT();	//初始化与LED连接的硬件接口
	Light_Green.INIT();
	Wireless_Com.INIT();	//初始化NRF24L01
	timer_init();//***************************
