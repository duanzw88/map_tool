#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "assert.h"
#include "mem.h"
#include "flexstack.h"

#define T flexstack_t

struct T
{
	int N;			//栈顶位置
	int length;		//栈的总长度
	int dataSize;
	char *data;		//数据
};
static void resize(T stack,int newSize);

void flexstack_init(T stack,int length,int datasize,void *data)
{
	assert(stack);
	stack->length = length;
	stack->dataSize = datasize;
	stack->data = data;
	stack->N = 0;
}
T flexstack_new(int length,int datasize)
{
	assert(length > 0);

	T stack;
	NEW(stack);

	flexstack_init(stack,length,datasize,CALLOC(length,datasize));

	return stack;
}
void 	flexstack_push(T stack,void *data)
{
	assert(stack);
	if(stack->N == stack->length)
	{
		printf("%s:%d stack is full\n",__FILE__,__LINE__);
	}
	//printf("%s:%d push:n = %d\n",__FILE__,__LINE__,stack->N);
	memcpy(stack->data + stack->N * stack->dataSize,data,stack->dataSize);
	stack->N++;
}
void 	*flexstack_pop(T stack)
{
	assert(stack);
	stack->N--;
	void *data = stack->data + stack->N * stack->dataSize;
	//printf("%s:%d pop:n = %d\n",__FILE__,__LINE__,stack->N);

	return data;
}
int 	flexstack_isEmpty(T stack)
{
	return 0;
}
int 	flexstack_getSize(T stack)
{
	return 0;
}


static void resize(T stack,int newSize)
{
	assert(stack);
	assert(newSize > 0);
	RESIZE(stack,newSize);
}
