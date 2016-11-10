#ifndef __TIMER_H
#define __TIMER_H

#define T timer_t
typedef struct T *T;

extern T 	timer_new();
extern void timer_start(T timer);
extern double timer_elapsedTime(T timer);

#undef T
#endif
