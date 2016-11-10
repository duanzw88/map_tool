/**
 * @file list.c
 * @brief 链表
 * @author duanzw
 * @date 2016-1-23
 */

#include <stdarg.h>
#include <stddef.h>
#include "assert.h"
#include "mem.h"
#include "list.h"


#define T list_t

/**
 * @brief  将一个链表附加到另一个链表上
 * @param  list 被附加的链表
 * @param  tail 要附加的链表
 * @return      赋值后的新链表
 */
T list_append(T list,T tail)
{
	T *p = &list;
	while(*p)
	{
		p = &(*p)->rest;
	}
	*p = tail;

	return list;
}
/**
 * @brief 			创建并返回一个链表
 * @param  x       [description]
 * @param  VARARGS [description]
 * @return         创建的链表
 */
T list_list(void *x,...)
{
	va_list ap;
	T list,*p = &list;

	va_start(ap,x);
	for(;x;x = va_arg(ap,void *))
	{
		NEW(*p);

		(*p)->first = x;
		p = &(*p)->rest;
	}
	*p = NULL;
	va_end(ap);

	return list;

}
/**
 * @brief 移除链表的第一个节点
 * @param  list 目标链表
 * @param  x    将第一个节点的first字段赋值给*x
 * @return      结果链表
 */
T list_pop(T list,void **x)
{
	if(list)
	{
		//链表非空
		T head = list->rest;
		if(x)
		{
			*x = list->first;
		}
		FREE(list);
		return head;
	}
	else
	{
		//链表为空
		return list;
	}
}
/**
 * @brief  在链表起始处添加一个包含x的新节点
 * @param  list 链表
 * @param  x    新节点的值
 * @return      新的链表
 */
T list_push(T list,void *x)
{
	T p;

	NEW(p);
	p->first = x;
	p->rest = list;

	return p;
}
/**
 * @brief 逆转链表
 * @param  list 链表
 * @return      逆转之后的链表
 */
T list_reverse(T list)
{
	T head = NULL;
	T next;
	for(;list;list = next)
	{
		next = list->rest;
		list->rest = head;
		head = list;
	}

	return head;
}
/**
 * @brief 链表节点的数目
 * @param  list 链表
 * @return      链表节点的数目
 */
int list_length(T list)
{
	int n;
	for (n = 0; list ; list = list->rest)
	{
		n++;
	}
	return n;
}

void list_free(T *list)
{
	T next;
	assert(list);
	for(; *list; *list = next)
	{
		next = (*list)->rest;
		FREE(*list);
	}
}
void list_map(T list,void apply(void **x,void *c1),void *c1)
{
	assert(apply);
	for(;list;list = list->rest)
	{
		apply(&list->first,c1);
	}
}
/**
 * @brief  把链表转换成数组
 * @param  list [description]
 * @param  end  [description]
 * @return      [description]
 */
void list_toArray(T list,void *end,void **array)
{
	int i;
	int n = list_length(list);
	// void **array = ALLOC((n + 1) * sizeof(*array));

	for(i = 0;i < n;i++)
	{
		array[i] = list->first;
		list = list->rest;
	}
	array[i] = end;
}
