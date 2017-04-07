#ifndef __YCJOBJECT_H__
#define __YCJOBJECT_H__

//分类标识符
#define Const		//常量
#define Variable	//变量
#define Method		//方法

//定义类标识符
#define class(c)	struct c

//定义对象标识符
#define object(c,o)		struct c o

//定义对象标识符：申请扩展空间对象
#define objectx(c,o)	struct c  xdata o

//申请对象指针标识符
#define pobject(c,o)	struct c *o

//功能：通用的对象创建宏
//参数：m，某类的创建函数名
//		o，待创建的对象名
//返回：无
//备注：
#define create(m,o)		m(o)

#endif