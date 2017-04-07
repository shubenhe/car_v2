#include "pwm.h"

u16 pwm_l = 5000,pwm_r = 5000;

void pwm_init(void)					//用TIM8产生PWM波形
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 60;
	TIM_TimeBaseInitStructure.TIM_Period = 60000;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM8,&TIM_OCInitStructure);
	TIM_OC3Init(TIM8,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM8,ENABLE);
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能	
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_SetCompare2(TIM8,pwm_l);
	TIM_SetCompare3(TIM8,pwm_r);
}
