#include "car.h"


// ���ܣ������̵Ʊ�ʶ
// ������obj��TLight���ͣ������Ա���������
// ���أ��̵Ʊ�ʶ
// ��ע��
u8 light_green_ID(void)
{
	return ID_LightGreen;
}

// ���ܣ����غ�Ʊ�ʶ
// ������obj��TLight���ͣ������Ա���������
// ���أ���Ʊ�ʶ
// ��ע��
u8 light_red_ID(void)
{
	return ID_LightRed;
}

// ���ܣ���Ƴ�ʼ��
// ��������
// ���أ���
// ��ע��
void light_red_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

// ���ܣ��̵Ƴ�ʼ��
// ��������
// ���أ���
// ��ע��
void light_green_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}

// ���ܣ��򿪵�
// ������obj��TLight���ͣ������Ա���������
// ���أ���
// ��ע��
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

// ���ܣ��رյ�
// ������obj��TLight���ͣ������Ա���������
// ���أ���
// ��ע��
void light_OFF(pobject(TLight,obj))
{
	obj->Status = Status_Light_OFF;
	
	if(obj->ID() == ID_LightGreen)	LED1 = 1;
	else if(obj->ID() == ID_LightRed)	LED0 = 1;
}

// ���ܣ����ഴ��
// ������obj��TLight���ͣ������Ա���������
// ���أ���
// ��ע��
void light_Create(pobject(TLight,obj))
{
	//������ʼ��
	
	//������ʼ��
	obj->Status = Status_Light_OFF;
	
	//��Ϊ��ʼ��
	obj->ON = light_ON;
	obj->OFF = light_OFF;
};

// ���ܣ���������ͨ�ű�ʶ
// ������obj��TLight���ͣ������Ա���������
// ���أ��̵Ʊ�ʶ
// ��ע��
u8 wireless_com_ID(void)
{
	return ID_Wireless_Com;
}

// ���ܣ�����ͨ�ų�ʼ��
// ������obj��TLight���ͣ������Ա���������
// ���أ���
// ��ע��
void wireless_com_INIT(void)
{
	NRF24L01_Init();
}

u8 CH_message[RX_PLOAD_WIDTH];

// ���ܣ���ȡ����ͨ������
// ������obj��TLight���ͣ������Ա���������
// ���أ�	0���������
//				1��δ�������
// ��ע��
u8 get_message(pobject(TWireless_Com,obj))
{
	return NRF24L01_RxPacket(obj->Channel);
}

// ���ܣ�����ͨ���ഴ��
// ������obj��TLight���ͣ������Ա���������
// ���أ���
// ��ע��
void wireless_com_Create(pobject(TWireless_Com,obj))
{
	//������ʼ��
	obj->ID = wireless_com_ID;
	
	//������ʼ��
	obj->Channel = CH_message;
	
	//��Ϊ��ʼ��
	obj->INIT = wireless_com_INIT;
	obj->Get_Message = get_message;
}

// ���ܣ��������ֱ�ʶ
// ��������
// ���أ����ֱ�ʶ
// ��ע��
u8 wheel_left_ID(void)
{
	return ID_Wheel_Left;
}

// ���ܣ������ҳ��ֱ�ʶ
// ��������
// ���أ��ҳ��ֱ�ʶ
// ��ע��
u8 wheel_right_ID(void)
{
	return ID_Wheel_Right;
}

// ���ܣ����ֿ��Ƴ�ʼ��
// ��������
// ���أ���
// ��ע��
void wheel_INIT(void)
{
	 pwm_init();
	 direction_init();
}

// ���ܣ���������
// ������obj��TLight���ͣ������Ա���������
// ���أ���
// ��ע��
void wheel_Run(pobject(TWheel,obj))
{
	if(obj->ID() == ID_Wheel_Left)
	{		//����
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
	{		//����
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

// ���ܣ������ഴ��
// ������obj��TLight���ͣ������Ա���������
// ���أ���
// ��ע��
void wheel_Create(pobject(TWheel,obj))
{
	//������ʼ��
	
	//������ʼ��
	obj->Direction = Direction_Stop;
	obj->Speed = 0;
	//��Ϊ��ʼ��
	obj->Run = wheel_Run;
	obj->INIT = wheel_INIT;
}

// ���ܣ�ת������Ϊ���Ӷ���
// ������	obj_L,TWheel��ָ�룬����
//			obj_R,TWheel��ָ�룬����
//			obj_Wl��TWireless_Com��ָ��
//			obj_Wl->Channel[0],���ţ�0-250
//			obj_Wl->Channel[1]��ǰ����0-250
//			obj_Wl->Channel[2]�����ҷ���0-250
// ���أ���
// ��ע��
void com_to_wheel(pobject(TWireless_Com,obj_Wl),pobject(TWheel,obj_L),pobject(TWheel,obj_R))
{
	u16 s_l_temp = 0;
	u16 s_r_temp = 0;
	
	s_l_temp = s_r_temp = obj_Wl->Channel[2]*100u/250;

	if(obj_Wl->Channel[0]>135)
	{//��ǰ
		obj_L->Direction =  obj_R->Direction = Direction_Forward;
		
		if(obj_Wl->Channel[1]>135)//����
		{
			s_r_temp = ((obj_Wl->Channel[0]-135u)+(obj_Wl->Channel[1]-135u))*s_r_temp/(250-135);
			s_l_temp = (obj_Wl->Channel[0]-135u)*s_l_temp/(250-135);
		}
		else if(obj_Wl->Channel[1]<115)//����
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
	{//����
		obj_L->Direction =  obj_R->Direction = Direction_Backward;

		if(obj_Wl->Channel[1]>135)//����
		{
			s_l_temp = ((115u-obj_Wl->Channel[0])+(obj_Wl->Channel[1]-135u))*s_l_temp/(250-135);
			s_r_temp = (115u-obj_Wl->Channel[0])*s_r_temp/(250-135);
		}
		else if(obj_Wl->Channel[1]<115)//����
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
	{// ԭ��
		if(obj_Wl->Channel[1]>135)
		{// ��ת
			obj_L->Direction = Direction_Stop;
			obj_R->Direction = Direction_Forward;
			s_r_temp = (obj_Wl->Channel[1]-135u)*s_r_temp/(250-135);
			s_l_temp = 0;
		}
		else if(obj_Wl->Channel[1]<115)
		{// ��ת
			obj_L->Direction = Direction_Forward;
			obj_R->Direction = Direction_Stop;
			s_l_temp = (115u-obj_Wl->Channel[1])*s_l_temp/115;
			s_r_temp = 0;
		}
		else
		{// ͣ
			obj_L->Direction =  obj_R->Direction = Direction_Stop;
			s_l_temp = s_r_temp = 0;
		}
	}
	obj_L->Speed = s_l_temp;
	obj_R->Speed = s_r_temp;
}

// ���ܣ����ؼ�ʱ����ʶ
// ��������
// ���أ����ֱ�ʶ
// ��ע��
u8 timer_ID(void)
{
	return ID_Timer;
}

// ���ܣ���ʱ����ʱ����
// ������	obj��TTimer���ͣ������Ա���������
//			set_value��u32���ͣ�����ֵ
// ���أ�	0����ʱδ��
//			1����ʱ��
// ��ע��
u8 timer_delay(pobject(TTimer,obj_T),pobject(TNakedSystem,obj_S),u32 set_value)
{
	_Bool T_flag = 0;
	
	if(0 == obj_S->Statistics_Switch)
	{	//δ������ѭ������λʱ���ʶ
		T_flag = obj_S->T_1ms_flag_M;
		obj_S->T_1ms_flag_M = 0;
	}
	else
	{
		T_flag = *obj_T->BaseFlag;
	}
	
	if(set_value)
	{	//����ʱ
		if(0 == obj_T->Set)
		{	// δ������ʱʱ��
			obj_T->Set = set_value;
			obj_T->Counter = 0;
		}
	
		if(T_flag)
		{// ��ʱʱ����
			if(obj_T->Counter < obj_T->Set)
			{// ��ʱδ��
				obj_T->Counter++;
			}
			else
			{// ��ʱ��
				obj_T->Set = obj_T->Counter = 0;	//	��ʱ����λ
				
				return 1;
			}
		}
	}
	else
	{	//	������ʱ
		return 1;
	}
	
	return 0;
}

// ���ܣ���ʱ���ഴ��
// ������obj��TTimer���ͣ������Ա���������
// ���أ���
// ��ע��
void timer_Create(pobject(TTimer,obj))
{
	//������ʼ��
	obj->ID = timer_ID;
	//������ʼ��
	obj->Counter = 0;
	obj->Set = 0;
	//��Ϊ��ʼ��
	obj->DELAY = timer_delay;
}

// ���ܣ�����ϵͳ��ID
// ��������
// ���أ�ϵͳ��ID
// ��ע��
u8 nakedsystem_ID(void)
{
	return ID_NakedSystem;
}

// ���ܣ���ѭ����ϵͳ������������ʱ���ǣ�ͳ��ϵͳʹ����
// ������	obj_S��TNakedSystem��ָ��
//				obj_T��TTimer��ָ��
// ���أ���
// ��ע��
void nakedsystem_run(pobject(TNakedSystem,obj_S),pobject(TTimer,obj_T))
{
	u32 one_time = 0;
	
	//����ʱ����
	obj_S->T_1ms_flag = obj_S->T_1ms_flag_M;
	obj_S->T_1ms_flag_M = 0;
	
	//ͳ��
	if(obj_S->Statistics_Switch)
	{//ͳ���ѿ�ʼ
		one_time = TIM_GetCounter(TIM6);
		TIM_SetCounter(TIM6,0);
		TIM_Cmd(TIM6, ENABLE);
		
		obj_S->Loop_Times_Counter++;
		
		if(obj_T->DELAY(obj_T,obj_S,1000))
		{//��ʱ1��ִ��ƽ��ͳ��
			obj_S->LoopTime_MEAN_1S = 1000000u/obj_S->Loop_Times_Counter;//����ƽ��ֵ
			obj_S->Loop_Times_Counter = 0;		//��λ��ʼ��һ��ͳ��
#if UART_SWITCH
			printf("mean %dus---min %dus---max %dus\r\n",obj_S->LoopTime_MEAN_1S,obj_S->LoopTime_MIN,obj_S->LoopTime_MAX);
#endif
		}
		
		if(one_time > obj_S->LoopTime_MAX)
		{//�������ֵ
			obj_S->LoopTime_MAX = one_time;
		}
		if(one_time < obj_S->LoopTime_MIN)
		{//������Сֵ
			obj_S->LoopTime_MIN = one_time;
		}
	}
	else
	{//ͳ��δ��ʼ
		obj_S->Loop_Times_Counter = 0;
		obj_S->LoopTime_MAX = 0;
		obj_S->LoopTime_MIN = 10000000;//10��
		obj_S->LoopTime_MEAN_1S = 0;
		obj_S->Statistics_Switch = 1;
		TIM_SetCounter(TIM6,0);
		TIM_Cmd(TIM6, ENABLE);
	}
}

// ���ܣ���λʱ����
// ������	obj��TNakedSystem��ָ��
// ���أ���
// ��ע��
void nakedsystem_set_t_flag(pobject(TNakedSystem,obj))
{
	obj->T_1ms_flag_M = 1;
}

#define SYS_LOOP_TIME_STATS_MAX 65000
// ���ܣ�ϵͳͳ�ƶ�ʱ����ʼ��
// ������
// ���أ���
// ��ע��
void nakedsystem_timer_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = SYS_LOOP_TIME_STATS_MAX; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������65536Ϊ65.536ms
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  2Mhz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM6, //TIM6
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_Cmd(TIM6, DISABLE);  //ʹ��TIMx����RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 9; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������10Ϊ1ms
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM4
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
}

// ���ܣ�TIM4��ʱ���ж�
// ������
// ���أ���
// ��ע��

//����ϵͳ��
extern struct TNakedSystem MySystem;
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			MySystem.SET_T_Flag(&MySystem);
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}

// ���ܣ�TIM6��ʱ���ж�
// ������
// ���أ���
// ��ע��
void TIM6_IRQHandler(void)   //TIM4�ж�
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_Cmd(TIM6, DISABLE);  //ʹ��TIMx����
			TIM_SetCounter(TIM6,SYS_LOOP_TIME_STATS_MAX);
			
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}

// ���ܣ�ϵͳ�ഴ��
// ������	obj��TNakedSystem��ָ��
// ���أ���
// ��ע��
void nakedsystem_Create(pobject(TNakedSystem,obj))
{
	// ������ʼ��
	obj->ID = nakedsystem_ID;
	// ������ʼ��
	obj->Statistics_Switch = 0;
	obj->LoopTime_MAX = 0;
	obj->LoopTime_MEAN_1S = 0;
	obj->LoopTime_MIN = 10000000;
	obj->Loop_Times_Counter = 0;
	obj->T_1ms_flag = 0;
	obj->T_1ms_flag_M = 0;
	//	��Ϊ��ʼ��
	obj->Timer_INIT = nakedsystem_timer_init;
	obj->RUN = nakedsystem_run;
	obj->SET_T_Flag = nakedsystem_set_t_flag;
}
