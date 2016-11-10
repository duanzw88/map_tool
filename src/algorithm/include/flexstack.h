/**
 * 灵活的栈
 * 由数组组成,将所有元素保存在数组中，并动态调整数组的大小
 */
#ifndef _FLEXSTACK_H
#define _FLEXSTACK_H

#define T flexstack_t
typedef struct T *T;

extern 	T 		flexstack_new(int length,int size);
extern  void 	flexstack_push(T stack,void *data);
extern 	void 	*flexstack_pop(T stack);
extern 	int 	flexstack_isEmpty(T stack);
extern  int 	flexstack_getSize(T stack);

#undef T
#endif
