#ifndef ST_UNORDER_H
#define ST_UNORDER_H

#include "item.h"

#define T st_unorder_t
typedef struct T *T;


extern T 		st_new(int size);
extern int 		st_count(T st);
extern void 	st_insert(T st,Item item);
extern Item 	st_search(T st,Key key);
// extern void 	st_delete(T st,Key key);
// extern Item 	st_select(T st,Key key);
extern void 	st_free(T st);
// void 	st_sort()

#undef T
#endif
