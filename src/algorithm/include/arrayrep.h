#ifndef _ARRAYREP_H
#define _ARRAYREP_H

#define T array_t

struct T
{
	int 	length;	//数组中元素的数目
	int 	size;	//元素的大小
	char 	*array;	//指向数组空间内存的指针
};

extern void  arrayrep_init(T array,int length,int size,void *ary);

#undef T
#endif
