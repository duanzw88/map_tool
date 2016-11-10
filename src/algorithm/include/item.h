#ifndef ITEM_H
#define ITEM_H

#define T item_t
typedef struct T *T;
typedef int Key;

extern T 	item_new();
extern void item_free(T item);
extern void item_setKey(T item,Key key);
extern Key  item_getKey(T item);

extern void item_setValue(T item,int value);
extern int 	item_getValue(T item);

#undef T
#endif
