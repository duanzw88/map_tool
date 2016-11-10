#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "atom.h"
#include "mem.h"
#include "counter.h"

#define T counter_t

struct T
{
	const char *name;
	int count;
};

T 	counter_new(const char *name)
{
	T counter;
	assert(name);

	NEW(counter);
	counter->name = atom_string(name);
	counter->count = 0;
	return counter;
}
void counter_increment(T counter)
{
	assert(counter);
	counter->count++;
}
int 	counter_get_tally(T counter)
{
	assert(counter);
	return counter->count;
}
const char *counter_get_name(T counter)
{
	assert(counter);
	return counter->name;
}
// char *counter_tostring(T counter)
// {
// 	return NULL;
// }
