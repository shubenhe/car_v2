#include "direction.h"
#include "usart.h"

void direction_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void direction(u8 dir)
{
	
	switch(dir)
	{
		case stop:
		{
			PCout(6) = 0;
			PBout(0) = 0;
			PBout(1) = 0;
			PBout(2) = 0;
//			printf("\r\nstop\r\n");
			break;
		}
		case forward:
		{
			PCout(6) = 1;
			PBout(0) = 0;
			PBout(1) = 1;
			PBout(2) = 0;
//			printf("\r\nforward\r\n");
			break;
		}
		case back:
		{
			PCout(6) = 0;
			PBout(0) = 1;
			PBout(1) = 0;
			PBout(2) = 1;
//			printf("\r\nback\r\n");
			break;
		}
		case left:
		{
			PCout(6) = 0;
			PBout(0) = 1;
			PBout(1) = 1;
			PBout(2) = 0;
//			printf("\r\neft\r\n");
			break;
		}
		case right:
		{
			PCout(6) = 1;
			PBout(0) = 0;
			PBout(1) = 0;
			PBout(2) = 1;
//			printf("\r\nright\r\n");
			break;
		}
		default:
			break;
	}
}
