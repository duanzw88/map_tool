#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "assert.h"
#include "st_unorder.h"

#define less(A,B) ((A) < (B))
#define eq(A,B)	  ((A) == (B))


#define T st_unorder_t
struct T
{
	int N;
	struct member
	{
		Item item;
		struct member *next;
	}*head;
};


T st_new()
{
	T st;
	NEW(st);
	st->N = 0;
	st->head = NULL;

	return st;
}
int 	st_count(T st)
{
	assert(st);
	return st->N;
}
void 	st_insert(T st,Item item)
{
	assert(st);

	struct member *p;
	Key key = item->key;
	Item checkItem = st_search(st,key);
	if(checkItem != NULL)
	{
		checkItem->value = item->value;
	}
	else
	{
		NEW(p);
		p->item = item;
		p->next = st->head;

		st->head = p;
		st->N++;
	}

}


Item 	st_search(T st,Key key)
{
	assert(st);
	struct member *p;
	Item item = NULL;
	p = st->head;

	while(p)
	{
		item = p->item;
		if(item->key == key)
		{

			return item;
		}
		p = p->next;
	}

	return NULL;
}
// void 	st_delete(T st,Key key)
// {
//
// }
// Item 	st_select(T st,Key key)
// {
// 	return NULL;
// }

void st_free(T st)
{
	assert(st);
	FREE(st);
}
