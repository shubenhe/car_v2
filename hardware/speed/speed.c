#include "speed.h"

void speed_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;					//刷新频率为72MHZ/72
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV4;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV4;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC3|TIM_IT_CC4,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
}


u8  TIM2CH3_CAPTURE_STA = 0;	//输入捕获状态		    				
u16	TIM2CH3_CAPTURE_VAL =0;	//输入捕获值
u8  TIM2CH4_CAPTURE_STA = 0;	//输入捕获状态		    				
u16	TIM2CH4_CAPTURE_VAL = 0;	//输入捕获值
u16 s_temp_l = 0;						//当前左轮速度
u16 s_temp_r = 0;						//当前右轮速度
 

//定时器2中断服务程序	 
void TIM2_IRQHandler(void)
{ 
	u16 temp = 0;
	u32 s_temp = 0;

	if((TIM2CH3_CAPTURE_STA&0X80)==0)//CH3还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM2CH3_CAPTURE_STA&0X0A)==0X0A)//高电平太长了
				{
					TIM2CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM2CH3_CAPTURE_VAL=0XFFFF;
				}
				else 
					TIM2CH3_CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)//捕获1发生捕获事件
			{	
				if(TIM2CH3_CAPTURE_STA&0X40)		//捕获到第二个上降沿 		
				{	  			
					temp = TIM_GetCapture3(TIM2);
					if(temp >= TIM2CH3_CAPTURE_VAL)
						TIM2CH3_CAPTURE_VAL=temp - TIM2CH3_CAPTURE_VAL;
					else
					{
						TIM2CH3_CAPTURE_VAL = 65536 - TIM2CH3_CAPTURE_VAL + temp;
						TIM2CH3_CAPTURE_STA--;
					}
					TIM2CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
				}
				else  								//还未开始,第一次捕获上升沿
				{
					TIM2CH3_CAPTURE_STA=0;			//清空
					TIM2CH3_CAPTURE_VAL = TIM_GetCapture3(TIM2);
					TIM2CH3_CAPTURE_STA|=0X40;		//标记捕获到了第一个上升沿
				}		    
			}			     	    					   
 	}
	else
	{
		s_temp =TIM2CH3_CAPTURE_STA&0X3F;
		s_temp*=65536;					//溢出时间总和
		s_temp = 720000000 / (s_temp + TIM2CH3_CAPTURE_VAL);		//得到左轮速度单位为rad/h
		if(s_temp < 20000)
			s_temp_l = s_temp*0.8+s_temp_l*0.2;
		TIM2CH3_CAPTURE_STA=0;			//开启下一次捕获
	}		

	
 	
	
	if((TIM2CH4_CAPTURE_STA&0X80)==0)//CH4还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM2CH4_CAPTURE_STA&0X0A)==0X0A)//高电平太长了
				{
					TIM2CH4_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM2CH4_CAPTURE_VAL=0XFFFF;
				}
				else 
					TIM2CH4_CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)//捕获1发生捕获事件
			{	
				if(TIM2CH4_CAPTURE_STA&0X40)		//捕获到第二个上升沿 		
				{	  			
					temp = TIM_GetCapture4(TIM2);
					if(temp >= TIM2CH4_CAPTURE_VAL)
						TIM2CH4_CAPTURE_VAL=temp - TIM2CH4_CAPTURE_VAL;
					else
					{
						TIM2CH4_CAPTURE_VAL = 65536 - TIM2CH4_CAPTURE_VAL + temp;
						TIM2CH4_CAPTURE_STA--;
					}
					TIM2CH4_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
				}
				else  								//还未开始,第一次捕获上升沿
				{
					TIM2CH4_CAPTURE_STA=0;			//清空
					TIM2CH4_CAPTURE_VAL = TIM_GetCapture4(TIM2);
					TIM2CH4_CAPTURE_STA|=0X40;		//标记捕获到了第一个上升沿
				}		    
			}			     	    					   
 	}	
	else
	{
		s_temp =TIM2CH4_CAPTURE_STA&0X3F;
		s_temp*=65536;					//溢出时间总和
		s_temp = 720000000 / (s_temp + TIM2CH4_CAPTURE_VAL);		//得到右轮速度单位为rad/h v
		if(s_temp < 20000)
			s_temp_r = s_temp*0.8+s_temp_r*0.2;//数字滤波
		TIM2CH4_CAPTURE_STA=0;			//开启下一次捕获
	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); //清除中断标志位
 
}
