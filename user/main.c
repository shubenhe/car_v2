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

#define speed_min 2000
#define speed_max 12000
#define speed_range 100
#define pwm_min 3600
#define pwm_max 60000


extern	u8 hopping[5];
extern	u8 hopping_turn;


 int main(void)
 {
	 
	u16 t=0;			 
	u8 tmp_buf[33]; 
	 
//	 delay_init();
	 NVIC_Configuration();
	 uart_init(9600);
	 pwm_init();
	 direction_init();
	 
	 
	key_init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	led_init();				//������ʼ��
 	NRF24L01_Init();    	//��ʼ��NRF24L01   
 	while(NRF24L01_Check())	//���NRF24L01�Ƿ���λ.	
	{
		while(1);
	}
	delay_ms(1000);
	
		NRF24L01_RX_Mode();		
	 
	 while(1)
	 {  		    		    				 
			if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ
			{
				LED0=1; //�����
				
				pwm_l = pwm_r = tmp_buf[2]*60000u/250;

				if(tmp_buf[0]>135)
				{
					direction(forward);
					
					if(tmp_buf[1]>135)//����
					{
						pwm_r = ((tmp_buf[0]-135u)+(tmp_buf[1]-135u))*pwm_r/(250-135);
						pwm_l = (tmp_buf[0]-135u)*pwm_l/(250-135);
					}
					else if(tmp_buf[1]<115)//����
					{
						pwm_l = ((tmp_buf[0]-135u)+(115u-tmp_buf[1]))*pwm_l/(250-135);
						pwm_r = (tmp_buf[0]-135u)*pwm_r/(250-135);
					}
					else
					{
						pwm_l = (tmp_buf[0]-135u)*pwm_l/(250-135);
						pwm_r = (tmp_buf[0]-135u)*pwm_r/(250-135);
					}
				}
				else if(tmp_buf[0]<115)
				{
					direction(back);
					
					if(tmp_buf[1]>135)//����
					{
						pwm_l = ((115u-tmp_buf[0])+(tmp_buf[1]-135u))*pwm_l/(250-135);
						pwm_r = (115u-tmp_buf[0])*pwm_r/(250-135);
					}
					else if(tmp_buf[1]<115)//����
					{
						pwm_r = ((115u-tmp_buf[0])+(115u-tmp_buf[1]))*pwm_r/(250-135);
						pwm_l = (115u-tmp_buf[0])*pwm_l/(250-135);
					}
					else
					{
						pwm_l = (115u-tmp_buf[0])*pwm_l/115;
						pwm_r = (115u-tmp_buf[0])*pwm_r/115;
					}
				}
				else
				{
					if(tmp_buf[1]>135)
					{
						direction(left);
						pwm_r = (tmp_buf[1]-135u)*pwm_r/(250-135);
						pwm_l = 0;
					}
					else if(tmp_buf[1]<115)
					{
						direction(right);
						pwm_l = (115u-tmp_buf[1])*pwm_l/115;
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
			{//δ�յ��źţ������
				LED0=0;
			}
			
			if(t==50000)
			{
				t=0;
				LED1=!LED1;
			} 
			t++;
	 
	 }
 }


