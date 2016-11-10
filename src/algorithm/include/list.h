#ifndef __LIST_H
#define __LIST_H

#define T list_t
typedef struct T *T;

struct T
{
	T rest;
	void *first;
};

extern 	T		list_append(T list,T tail);
extern 	T 		list_copy(T list);

//创建并返回一个链表,传递N+1个指针作为参数，前N个非空，最后一个为NULL
extern 	T		list_list(void *x,...);
extern 	T 		list_pop(T list,void **x);
//在链表list的起始处添加一个包含x的新节点，并返回新的链表
extern  T 		list_push(T list,void *x);
//逆转链表的顺序，返回结果链表
extern 	T 		list_reverse(T list);
//返回其参数链表的节点数目
extern  int 	list_length(T list);
//释放*list链表中的所有节点并将其设置为NULL指针
extern 	void 	list_free(T *list);
//对list的链表中的每个节点调用apply指向的函数
extern 	void 	list_map(T list,void apply(void **x,void *cl),void *cl);
extern  void 	list_toArray(T list,void *end,void **array);

#endif
