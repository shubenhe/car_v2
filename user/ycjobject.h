#ifndef __YCJOBJECT_H__
#define __YCJOBJECT_H__

//�����ʶ��
#define Const		//����
#define Variable	//����
#define Method		//����

//�������ʶ��
#define class(c)	struct c

//��������ʶ��
#define object(c,o)		struct c o

//��������ʶ����������չ�ռ����
#define objectx(c,o)	struct c  xdata o

//�������ָ���ʶ��
#define pobject(c,o)	struct c *o

//���ܣ�ͨ�õĶ��󴴽���
//������m��ĳ��Ĵ���������
//		o���������Ķ�����
//���أ���
//��ע��
#define create(m,o)		m(o)

#endif