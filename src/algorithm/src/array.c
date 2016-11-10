#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"

#define T array_t

void  arrayrep_init(T array,int length,int size,void *ary)
{
	assert(array);
	assert( (ary && length > 0) || (length == 0 && ary == NULL) );
	assert(size > 0);

	array->length = length;
	array->size = size;
	if(length > 0)
	{
		array->array = ary;
	}
	else
	{
		array->array = NULL;
	}
}
/**
 * 分配初始化数组
 */
T 	array_new(int length,int size)
{
	T array;

	NEW(array);
	if(length > 0)
	{
		arrayrep_init(array,length,size,CALLOC(length,size));
	}
	else
	{
		arrayrep_init(array,length,size,NULL);
	}

	return array;
}

/**
 * 释放数组并将其清零
 * @param array 要释放的数组
 */
void array_free(T *array)
{
	assert(array);
	assert(*array);
	FREE((*array)->array);
	FREE(*array);
}

/**
 * 获取数组长度
 * @param  array 目标数组
 * @return       元素的数目
 */
int array_length(T array)
{
	assert(array);
	return array->length;
}

/**
 * 元素大小
 * @param  array 目标数组
 * @return       数组中元素的大小
 */
int array_size(T array)
{
	assert(array);
	return array->size;
}

/**
 * 根据索引获取元素
 * @param  array 目标数组
 * @param  i     索引值
 * @return       编号为i的元素的指针
 */
void *array_get(T array,int i)
{
	assert(array);
	assert(i >= 0 && i < array->length);

	return array->array + i * array->size;
}

/**
 * 添加或者更新元素
 * @param  array 目标数组
 * @param  i     元素的索引值
 * @param  elem  要添加或者更新的元素
 * @return       elem
 */
void *array_put(T array,int i,void *elem)
{
	assert(array);
	assert(i >= 0 && i < array->length);
	assert(elem);

	memcpy(array->array + i * array->size,elem,array->size);

	return elem;
}

/**
 * 改变array的大小 使之能够容纳length个元素，根据需要扩展或收缩数组
 * @param array  目标数组
 * @param length 数组新的长度
 */
void array_resize(T array,int length)
{
	assert(array);
	assert(length >= 0);

	if(length == 0)
	{
		FREE(array->array);
	}
	else if(array->length == 0)
	{
		array->array = ALLOC(length * array->size);
	}
	else
	{
		RESIZE(array->array,length * array->size);
	}

	array->length = length;
}

/**
 * 数组拷贝
 * @param  array  目标数组
 * @param  length 要拷贝的长度
 * @return        目标数组的副本
 */
T array_copy(T array,int length)
{
	T copy;

	assert(array);
	assert(length >= 0);
	copy = array_new(length,array->size);
	if(copy->length >= array->length && array->length > 0)
	{
		memcpy(copy->array,array->array,array->length * array->size);
	}
	else if(array->length > copy->length && copy->length > 0)
	{
		memcpy(copy->array,array->array,copy->length * copy->size);
	}

	return copy;

}
