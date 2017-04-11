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
u8 timer_delay(pobject(TTimer,obj),u32 set_value)
{
	_Bool base_flag = *obj->BaseFlag;//����ʱ�����
	
	*obj->BaseFlag = 0;//��λʱ����ʶ
	
	if(set_value)
	{	//����ʱ
		if(0 == obj->Set)
		{	// δ������ʱʱ��
			obj->Set = set_value;
			obj->Counter = 0;
		}
	
		if(base_flag)
		{// ��ʱʱ����
			if(obj->Counter <= obj->Set)
			{// ��ʱδ��
				obj->Counter++;
			}
			else
			{// ��ʱ��
				obj->Set = obj->Counter = 0;	//	��ʱ����λ
				
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

_Bool TIMER1_1ms_flag;	// ʱ����ʶ

// ���ܣ���ʱ����ʶ��λ
// ��������
// ���أ���
// ��ע��
void timer_flag_set(void)
{
	TIMER1_1ms_flag = 1;
}

// ���ܣ���ʱ����ʼ��
// ������
// ���أ���
// ��ע��
void timer_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

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

// ���ܣ���ʱ���ж�
// ������
// ���أ���
// ��ע��
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			timer_flag_set();
			
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}
