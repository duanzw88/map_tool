
/**
*
* @file stack.c
* @brief 用一个抽象数据类型(ADT)来实现栈
* @author duanzw duanzhongwei@jiachongs.com
* @date 2016-1-13
*
*/

#include <assert.h>
#include <stddef.h>
#include "mem.h"
#include "stack.h"

#define T stack_tt

struct T
{
	int count;
	struct elem
	{
		void *x;
		struct elem *link;
	}*head;
};

/**
* @brief 创建新的栈
* @return 一个类型为T的值
*/
T stack_new(void)
{
	T stk;

	NEW(stk);
	stk->count = 0;
	stk->head = NULL;

	return stk;
}

/**
* @brief 判断栈是否为空
*
* @param stk 堆栈的指针
*
* @return
*		1 栈为空
*		0 栈不为空
*/
int  stack_empty(T stk)
{
	assert(stk);

	return stk->count == 0;
}
/**
* @brief 将一个指针推入栈顶
*
* @param stk 堆栈的指针
* @param x	 推入栈顶的数据
*
*/
void stack_push(T stk,void *x)
{
	struct elem *e;

	assert(stk);
	NEW(e);
	e->x = x;
	e->link = stk->head;

	stk->head = e;
	stk->count++;

}
// void stack_pop(T stk,void *x)
// {
// 	printf("pop---------------%p\n",x);
// 	struct elem *e;

// 	assert(stk);
// 	assert(stk->count > 0);

// 	e = stk->head;
// 	stk->head = e->link;
// 	stk->count--;

// 	memcpy(x,e->x,(unsigned long)sizeof(void *));
// }

/**
* @brief 弹出栈顶指针
*
* @param stk 堆栈的指针
*
* @return 栈顶指针
*/
void *stack_pop(T stk)
{
	void *x;
	struct elem *e;

	assert(stk);
	assert(stk->count > 0);

	e = stk->head;
	stk->head = e->link;
	stk->count--;

	x = e->x;

//	printf("pop---------------%p\n",x);
	FREE(e);

	return x;
}

/**
* @brief 释放指针所指向的栈
*
* @param stk 堆栈的指针
*
*/
void stack_free(T *stk)
{
	struct elem *e,*u;

	assert(stk && *stk);

	for(e = (*stk)->head;e;e = u)
	{
		u = e->link;

		FREE(e);
	}

	FREE(*stk);
}
