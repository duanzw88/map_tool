#ifndef _ARRAY_H
#define _ARRAY_H

#define T array_t
typedef struct T *T;

/**
 * 分配初始化数组
 * @param  length 数组包含length个元素 索引值0-(length-1)
 * @param  size   每个元素占size个字节
 * @return        新的数组
 *
 * ps：length为 数组不包含任何元素，每个元素的各个字节都初始化为0，size必须包含对齐所需的填充字节
 */
extern T 	array_new(int length,int size);

/**
 * 释放数组并将其清零
 * @param array 要释放的数组
 */
extern void array_free(T *array);

/**
 * 数组长度
 * @param  array 目标数组
 * @return       元素的数目
 */
extern int array_length(T array);

/**
 * 元素大小
 * @param  array 目标数组
 * @return       数组中元素的大小
 */
extern int array_size(T array);

/**
 * 根据索引获取元素
 * @param  array 目标数组
 * @param  i     索引值
 * @return       编号为i的元素的指针
 */
extern void *array_get(T array,int i);

/**
 * 添加或者更新元素
 * @param  array 目标数组
 * @param  i     元素的索引值
 * @param  elem  要添加或者更新的元素
 * @return       elem
 *
 * ps：该函数并不返回元素i先前的值，因为元素未必是指针而且元素也可以是任意字节长
 */
extern void *array_put(T array,int i,void *elem);

/**
 * 改变array的大小 使之能够容纳length个元素，根据需要扩展或收索数组
 * @param array  目标数组
 * @param length 数组新的长度
 */
extern void array_resize(T array,int length);

/**
 * 数组拷贝
 * @param  array  目标数组
 * @param  length 要拷贝的长度
 * @return        目标数组的副本
 */
extern T array_copy(T array,int length);

#undef T
#endif
