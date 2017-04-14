#include "mysystem.h"


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

//����ϵͳ��
extern struct TNakedSystem MySystem;

// ���ܣ�TIM4��ʱ���ж�
// ������
// ���أ���
// ��ע��
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
