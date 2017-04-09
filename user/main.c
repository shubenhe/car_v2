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

#define speed_min 2000
#define speed_max 12000
#define speed_range 100
#define pwm_min 3600
#define pwm_max 60000


extern	u8 hopping[5];
extern	u8 hopping_turn;


 int main(void)
 {
	 u32 t=0;
	 
	 //申请对象空间——绿灯
	 object(TLight,Light_Green);
	 //红灯
	 object(TLight,Light_Red);
	 //无线通信
	 object(TWireless_Com,Wireless_Com);
	 
	 //创建对象——绿灯
	 create(light_Create,&Light_Green);
	 Light_Green.ID = light_green_ID;
	 //红灯
	 create(light_Create,&Light_Red);
	 Light_Red.ID = light_red_ID;
	 //无线通信
	 create(wireless_com_Create,&Wireless_Com);
	 
	 delay_init();
	 NVIC_Configuration();
	 uart_init(9600);
	 pwm_init();
	 direction_init();
	 
	key_init();		  		//初始化与LED连接的硬件接口
 	led_init();				//按键初始化
 	Wireless_Com.INIT();    	//初始化NRF24L01
	
 	while(NRF24L01_Check())	//检查NRF24L01是否在位.	
	{
		while(1);
	}
	delay_ms(1000);
	
		NRF24L01_RX_Mode();		
	 
	 while(1)
	 {  		    		    				 
			if(Wireless_Com.Get_Message(&Wireless_Com) ==0)//一旦接收到信息
			{
				Light_Red.ON(&Light_Red); //红灯灭
				
				pwm_l = pwm_r = Wireless_Com.Channel[2]*60000u/250;

				if(Wireless_Com.Channel[0]>135)
				{
					direction(forward);
					
					if(Wireless_Com.Channel[1]>135)//往左
					{
						pwm_r = ((Wireless_Com.Channel[0]-135u)+(Wireless_Com.Channel[1]-135u))*pwm_r/(250-135);
						pwm_l = (Wireless_Com.Channel[0]-135u)*pwm_l/(250-135);
					}
					else if(Wireless_Com.Channel[1]<115)//往右
					{
						pwm_l = ((Wireless_Com.Channel[0]-135u)+(115u-Wireless_Com.Channel[1]))*pwm_l/(250-135);
						pwm_r = (Wireless_Com.Channel[0]-135u)*pwm_r/(250-135);
					}
					else
					{
						pwm_l = (Wireless_Com.Channel[0]-135u)*pwm_l/(250-135);
						pwm_r = (Wireless_Com.Channel[0]-135u)*pwm_r/(250-135);
					}
				}
				else if(Wireless_Com.Channel[0]<115)
				{
					direction(back);
					
					if(Wireless_Com.Channel[1]>135)//往左
					{
						pwm_l = ((115u-Wireless_Com.Channel[0])+(Wireless_Com.Channel[1]-135u))*pwm_l/(250-135);
						pwm_r = (115u-Wireless_Com.Channel[0])*pwm_r/(250-135);
					}
					else if(Wireless_Com.Channel[1]<115)//往右
					{
						pwm_r = ((115u-Wireless_Com.Channel[0])+(115u-Wireless_Com.Channel[1]))*pwm_r/(250-135);
						pwm_l = (115u-Wireless_Com.Channel[0])*pwm_l/(250-135);
					}
					else
					{
						pwm_l = (115u-Wireless_Com.Channel[0])*pwm_l/115;
						pwm_r = (115u-Wireless_Com.Channel[0])*pwm_r/115;
					}
				}
				else
				{
					if(Wireless_Com.Channel[1]>135)
					{
						direction(left);
						pwm_r = (Wireless_Com.Channel[1]-135u)*pwm_r/(250-135);
						pwm_l = 0;
					}
					else if(Wireless_Com.Channel[1]<115)
					{
						direction(right);
						pwm_l = (115u-Wireless_Com.Channel[1])*pwm_l/115;
						pwm_r = 0;
					}
					else
					{
						direction(stop);
					}
				}
				
				TIM_SetCompare2(TIM8,pwm_l);
				TIM_SetCompare3(TIM8,pwm_r);
			}
			else
			{//未收到信号，红灯亮
				Light_Red.OFF(&Light_Red);
			}
			
			if(t==50000)
			{
				t=0;
				if(Status_Light_OFF == Light_Green.Status)
					Light_Green.ON(&Light_Green);
				else
					Light_Green.OFF(&Light_Green);
			} 
			t++;
	 
	 }
 }


