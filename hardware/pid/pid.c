#include "pid.h"
#include "speed.h"
#include "pwm.h"

#define T 0.01

u16 speed_r = 2000;
u16 speed_l = 2000;
int el_1 = 0,el_2 = 0,er_1 = 0,er_2 = 0;
float Kp = 0.7f,Ti = 0.5f,Td = 0.0f;

void pid_init(void)
{
	
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 99; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到100为10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	int pwm_l_temp,pwm_r_temp;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			
			pwm_l_temp = pid(speed_l-s_temp_l ,&el_1 ,&el_2 ,Kp ,Ti ,Td);
			if((pwm_l_temp > -1*pwm_l) && (pwm_l_temp < 10000))				//限制调整幅度，抵抗外部干扰
				pwm_l += pwm_l_temp;
			if(pwm_l < 5000)
				pwm_l = 5000;
			else if(pwm_l >60000)
				pwm_l = 60000;
			TIM_SetCompare2(TIM8,pwm_l);
			
			
			pwm_r_temp = pid(speed_r-s_temp_r ,&er_1 ,&er_2 ,Kp ,Ti ,Td);
			if((pwm_r_temp > -1*pwm_r) && (pwm_r_temp < 10000))				//限制调整幅度，抵抗外部干扰
				pwm_r += pwm_r_temp;
			if(pwm_r < 5000)
				pwm_r = 5000;
			else if(pwm_r >60000)
				pwm_r = 60000;
			TIM_SetCompare3(TIM8,pwm_r);
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		
		}
}


 int pid(int e,int* e1,int* e2,float Kp,float Ti,float Td)
 {
	 int pwm;												//本次输出的变化值
	 
	 pwm = Kp * (e-(*e1) + e*(T/Ti) + (Td/T)*(e-(*e1)*2+(*e2)));
	 
	 (*e2) = (*e1);
	 (*e1) = e;
	 
	 return pwm;
 }
