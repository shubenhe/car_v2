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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 99; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������100Ϊ10ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	int pwm_l_temp,pwm_r_temp;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			
			pwm_l_temp = pid(speed_l-s_temp_l ,&el_1 ,&el_2 ,Kp ,Ti ,Td);
			if((pwm_l_temp > -1*pwm_l) && (pwm_l_temp < 10000))				//���Ƶ������ȣ��ֿ��ⲿ����
				pwm_l += pwm_l_temp;
			if(pwm_l < 5000)
				pwm_l = 5000;
			else if(pwm_l >60000)
				pwm_l = 60000;
			TIM_SetCompare2(TIM8,pwm_l);
			
			
			pwm_r_temp = pid(speed_r-s_temp_r ,&er_1 ,&er_2 ,Kp ,Ti ,Td);
			if((pwm_r_temp > -1*pwm_r) && (pwm_r_temp < 10000))				//���Ƶ������ȣ��ֿ��ⲿ����
				pwm_r += pwm_r_temp;
			if(pwm_r < 5000)
				pwm_r = 5000;
			else if(pwm_r >60000)
				pwm_r = 60000;
			TIM_SetCompare3(TIM8,pwm_r);
			
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		
		}
}


 int pid(int e,int* e1,int* e2,float Kp,float Ti,float Td)
 {
	 int pwm;												//��������ı仯ֵ
	 
	 pwm = Kp * (e-(*e1) + e*(T/Ti) + (Td/T)*(e-(*e1)*2+(*e2)));
	 
	 (*e2) = (*e1);
	 (*e1) = e;
	 
	 return pwm;
 }
