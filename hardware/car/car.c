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
