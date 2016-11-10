#ifndef __STACK_H
#define __STACK_H

#define T stack_tt
typedef struct T *T;//同名结构体指针


extern T 	stack_new(void);
extern int  stack_empty(T stk);
extern void stack_push(T stk,void *x);
extern void *stack_pop(T stk);
extern void stack_free(T *stk);

#undef T
#endif