#ifndef __COUNTER_H__
#define __COUNTER_H__

#define T counter_t
typedef struct T *T;

extern T 	counter_new(const char *name);
extern void counter_increment(T counter);
extern const char *counter_get_name(T counter);
extern int 	counter_get_tally(T counter);
// extern char *counter_tostring(T counter);

#undef T
#endif
