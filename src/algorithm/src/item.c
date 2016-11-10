#include<stdlib.h>

#include "mem.h"
#include "assert.h"
#include "item.h"

#define T item_t
struct T
{
	Key key;
	int value;
};

T 	item_new()
{
	T item;

	NEW(item);
	item->key = 0;
	item->value = 0;

	return item;
}

void item_free(T item)
{
	assert(item);
	FREE(item);
}
void item_setKey(T item,Key key)
{
	assert(item);
	item->key = key;
}
Key  item_getKey(T item)
{
	assert(item);
	return item->key;
}

void item_setValue(T item,int value)
{
	assert(item);
	item->value = value;
}
int 	item_getValue(T item)
{
	assert(item);
	return item->value;
}
