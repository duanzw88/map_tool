#include <stdlib.h>
#include <time.h>

#include "assert.h"
#include "mem.h"
#include "timer.h"
#define T timer_t
struct T
{
	clock_t start;
};

T 	timer_new()
{
	T timer;

	NEW(timer);
	timer->start = 0L;

	return timer;
}
void timer_start(T timer)
{
	assert(timer);
	timer->start = clock();
}
double timer_elapsedTime(T timer)
{
	assert(timer);
	clock_t end = clock();

	return (double)(end - timer->start) / CLOCKS_PER_SEC;
}
