/**
 * 表
 * ADT类型
 * 导出的函数：
 * 		分配、释放表的实例
 * 		向表中添加、删除键值对
 * 		访问表中的键值对
 * 	向表中任何函数传递table的实例为NULL，或键为NULL都是已检查的运行时错误
 */

#ifndef __TABLE_H
#define __TABLE_H

//抽象数据类型
#define T table_t
typedef struct T *T;

/**
*
* 创建一个新表
* @param hint 新表中预期会容纳的表项目数,无论hint值如何,所有表都可以容纳任意数目的表项，但准确的hint值会提高性能
*             主要是因为hash桶中的数据采用的是链表存储，所以可以容纳任意数目的表项
* @param cmp  表的比较函数
* @param hash 根据key值返回hash码(哈希函数)
*
* 传递hint为负责是已检查的运行时错误
*/
extern T    table_new(int hint,int cmp(const void *x,const void *y),unsigned hash(const void *key));

/**
* 释放table
*/
extern void    table_free(T *table);
/**
* 返回table中的键-值对的数目
*/
extern int  table_length(T table);
/**
* 将有key和value组成的键-值对添加到table,如果key已经存在,则用value覆盖key此前对应的值
*/
extern void *table_put(T table,const void *key,void *value);
/**
*搜索查找key键，找到则返回key键相关联的值，如果table不包含key键，则返回NULL
*/
extern void *table_get(T table,const void *key);
/**
*搜索table查找key值,找到则删除并返回该键值对，找不到返回NULL
*/
extern void *table_remove(T table,const void *key);
/**
*以未指定的顺序对table中的每一个键值对调用apply指向的函数
*/
extern void table_map(T table,void apply(const void *key,void **value,void *cl),void *cl);
/**
*访问表中的每一个键值对，并将其收集到一个数组中
*/
extern void **table_toArray(T table,void *end);

#undef T
#endif
