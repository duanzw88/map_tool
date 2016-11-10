
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "assert.h"
#include "seq.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"

#define T seq_t

struct T
{
	struct array_t array;
	int length;
	int head;
};


static void expand(T seq)
{
	int n = seq->array.length;

	array_resize(&seq->array,2 * n);
	if(seq->head > 0)
	{
		void **old = &((void **)seq->array.array)[seq->head];
		memcpy(old + n,old,(n - seq->head) * sizeof(void *));
		seq->head += n;
	}
}
/**
 * 创建序列
 * @param  hint 对新序列将包含值的最大数目的估计
 * @return      一个空序列
 */
T seq_new(int hint)
{
	T seq;

	assert(hint >= 0);

	NEW0(seq);
	if(hint == 0)
	{
		hint = 16;
	}

	arrayrep_init(&seq->array,hint,sizeof(void *),ALLOC(hint * sizeof(void *)));

	return seq;
}
/**
 * 创建并返回一个序列，函数用非NULL指针参数来创建初始化序列中的值
 * @param  x       [description]
 * @param  VARARGS [description]
 * @return         新的序列
 */
T seq_seq(void *x,...)
{
	va_list ap;
	T seq = seq_new(0);

	va_start(ap,x);
	for(; x; x = va_arg(ap,void *))
	{
		seq_addend(seq,x);
	}

	va_end(ap);

	return seq;
}

/**
 * 释放序列
 * @param seq 目标序列
 */
void seq_free(T *seq)
{
	assert(seq && *seq);
	assert((void *)*seq == (void *)&(*seq)->array);

	array_free((array_t *)seq);
}

/**
 * 获取序列长度
 * @param  seq 目标序列
 * @return     序列长度
 */
int seq_length(T seq)
{
	assert(seq);
	return seq->length;
}
/**
 * 获取序列中的值
 * @param seq 目标序列
 * @param i   索引
 */
void *seq_get(T seq,int i)
{
	assert(seq);
	assert(i >= 0 && i < seq->length);
	return ((void **)seq->array.array)[(seq->head + i) % seq->array.length];
}

/**
 * 按索引更改序列的值
 * @param  seq 目标序列
 * @param  i   索引
 * @param  x   新值
 * @return     返回索引对应的先前值
 */
void *seq_put(T seq,int i,void *x)
{
	void *prev;
	assert(seq);
	assert(i >= 0 && i < seq->length);
	prev = ((void **)seq->array.array)[(seq->head + i) % seq->array.length];

	((void **)seq->array.array)[(seq->head + i) % seq->array.length] = x;
	return prev;
}

/**
 * 将x添加到seq的低端并返回x，添加到低端其实是序列的开始
 * @param  seq 目标序列
 * @param  x   要添加到低端的值
 * @return     添加的值
 */
void *seq_addstart(T seq,void *x)
{
	int i = 0;
	assert(seq);
	if(seq->length == seq->array.length)
	{
		expand(seq);
	}
	//该判断是怕head指向内存的0地址
	if(--seq->head < 0)
	{
		//感觉这个地方有问题，如果seq->array.length - 1当前有值的话 岂不是被覆盖了？？
		seq->head = seq->array.length - 1;
	}
	seq->length++;
	((void **)seq->array.array)[(seq->head + i) % seq->array.length] = x;

	return ((void **)seq->array.array)[(seq->head + i) % seq->array.length];
}

/**
 * 将x添加到seq序列的高端并返回x
 * @param  seq 目标序列
 * @param  x   要添加到高端的值
 * @return     添加的值
 */
void *seq_addend(T seq,void *x)
{
	int i;
	assert(seq);

	if(seq->length == seq->array.length)
	{
		expand(seq);
	}
	i = seq->length;
	seq->length++;

	((void **)seq->array.array)[(seq->head + i) % seq->array.length] = x;

	return ((void **)seq->array.array)[(seq->head + i) % seq->array.length];
}

/**
 * 删除低端的值 即删除序列开始的值
 * @param  seq 目标序列
 * @return     删除的值
 */
void *seq_removestart(T seq)
{
	int i = 0;
	void *x;
	assert(seq);
	assert(seq->length > 0);

	x = ((void **)seq->array.array)[(seq->head + i) % seq->array.length];
	seq->head = (seq->head + 1) % seq->array.length;

	--seq->length;

	return x;
}

/**
 * 删除高端的值，即删除序列末端的值
 * @param  seq 目标序列
 * @return     删除的值
 */
void *seq_removeend(T seq)
{
	int i;
	assert(seq);
	assert(seq->length > 0);
	i = --seq->length;
	//
	return ((void **)seq->array.array)[(seq->head + i) % (seq->array.length)];
}
