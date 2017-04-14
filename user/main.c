#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "pwm.h"
#include "pid.h"
#include "key.h"
#include "direction.h"
#include "speed.h"
#include "24l01.h"   
#include "led.h"   
#include "main.h"
#include "ycjobject.h"
#include "car.h"
#include "mysystem.h"

#define speed_min 2000
#define speed_max 12000
#define speed_range 100
#define pwm_min 3600
#define pwm_max 60000


extern	u8 hopping[5];
extern	u8 hopping_turn;

//�������ռ䡪��ϵͳ����
object(TNakedSystem,MySystem);

int main(void)
{
	
	//�������ռ䡪���̵�
	object(TLight,Light_Green);
	//���
	object(TLight,Light_Red);
	//����ͨ��
	object(TWireless_Com,Wireless_Com);
	//����
	object(TWheel,Wheel_Left);
	//�ҳ���
	object(TWheel,Wheel_Right);
	//��ʱ��1
	object(TTimer,TIMER1);
	//��ʱ��_ϵͳ����
	object(TTimer,TIMER_SYS);
	
	//�������󡪡�ϵͳ����
	create(nakedsystem_Create,&MySystem);
	//�̵�
	create(light_Create,&Light_Green);
	Light_Green.ID = light_green_ID;
	Light_Green.INIT = light_green_INIT;
	//���
	create(light_Create,&Light_Red);
	Light_Red.ID = light_red_ID;
	Light_Red.INIT = light_red_INIT;
	//����ͨ��
	create(wireless_com_Create,&Wireless_Com);
	//����
	create(wheel_Create,&Wheel_Left);
	Wheel_Left.ID = wheel_left_ID;
	//�ҳ���
	create(wheel_Create,&Wheel_Right);
	Wheel_Right.ID = wheel_right_ID;
	//��ʱ��1
	create(timer_Create,&TIMER1);
	TIMER1.BaseFlag = &(MySystem.T_1ms_flag);
	//��ʱ��_ϵͳ����
	create(timer_Create,&TIMER_SYS);
	TIMER_SYS.BaseFlag = &(MySystem.T_1ms_flag);
	
	//��ʼ��
	delay_init();
	NVIC_Configuration();
#if UART_SWITCH
	uart_init(115200);
#endif
	Wheel_Left.INIT();
	Light_Red.INIT();	//��ʼ����LED���ӵ�Ӳ���ӿ�
	Light_Green.INIT();
	Wireless_Com.INIT();	//��ʼ��NRF24L01
	MySystem.Timer_INIT();
	 
	while(NRF24L01_Check())	//���NRF24L01�Ƿ���λ.
	{
		while(1);
	}
	
	while(!(TIMER1.DELAY(&TIMER_SYS,&MySystem,1000)));
	
	NRF24L01_RX_Mode();
	
	while(1)
	{
		MySystem.RUN(&MySystem,&TIMER_SYS);
		
		if(Wireless_Com.Get_Message(&Wireless_Com) ==0)
		{	//һ�����յ���Ϣ
			Light_Red.ON(&Light_Red);	//�����
			com_to_wheel(&Wireless_Com,&Wheel_Left,&Wheel_Right);	//����ָ��ת��Ϊ����
			Wheel_Left.Run(&Wheel_Left);	//ִ������
			Wheel_Right.Run(&Wheel_Right);	//ִ������
		}
		else
		{	//δ�յ��ź�
			Light_Red.OFF(&Light_Red);	//�����
		}
		
		if(TIMER1.DELAY(&TIMER1,&MySystem,100))
		{
			if(Status_Light_OFF == Light_Green.Status)
				Light_Green.ON(&Light_Green);
			else
				Light_Green.OFF(&Light_Green);
		}
		
	 }
}


