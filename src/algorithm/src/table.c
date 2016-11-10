#include <limits.h>
#include <stddef.h>

#include "mem.h"
#include "assert.h"
#include "table.h"

#define T table_t

struct T
{
	int size;
	int (*cmp)(const void *x,const void *y);
	unsigned (*hash)(const void *key);
	int length;
	unsigned timestamp;

	//buckets指向一个数组，包含适当数目的元素
	struct binding
	{
		struct binding *link;
		const void *key;
		void *value;
	}**buckets;
};


static int cmpatom(const void *x,const void *y)
{
	return x != y;
}
static unsigned hashatom(const void *key)
{
	return (unsigned long)key >> 2;
}
/**
*
* 创建一个新表
* @param hint 新表中预期会容纳的表项目数,无论hint值如何,所有表都可以容纳任意数目的表项，但准确的hint值会提高性能
* @param cmp  表的比较函数
* @param hash 根据key值返回hash码(哈希函数)
*/
T    table_new(int hint,int cmp(const void *x,const void *y),unsigned hash(const void *key))
{
		T table;
		int i;
		static int primes[] = {509,1021,2053,4093,8191,16381,32771,65521,INT_MAX};

		assert(hint >= 0);
		//将i设置为primes中大于等于hint的第一个元素的索引值,注意：该循环索引从1开始
		for(i = 1;primes[i] < hint;i++);

		//分配table占用的空间
		table = ALLOC(sizeof(*table) + primes[i-1] * sizeof(table->buckets[0]));
		table->size = primes[i-1];
		table->cmp = cmp ? cmp : cmpatom;
		table->hash = hash ? hash : hashatom;
		table->buckets = (struct binding **)(table + 1);
		for(i = 0;i < table->size;i++)
		{
			table->buckets[i] = NULL;
		}
		table->length = 0;
		table->timestamp = 0;


		return table;
}

/**
 * 释放表
 * @param  table 表
 * @return       无
 *
 * 在释放表的过程中连其内容一起释放
 */
void    table_free(T *table)
{

	assert(table && *table);
	if((*table)->length > 0)
	{
		int i;
		struct binding *p,*q;
		//释放表中的元素
		for(i = 0;i < (*table)->size;i++)
		{
			for(p = (*table)->buckets[i];p;p = q)
			{
				q = p->link;
				FREE(p);
			}
		}
	}
	FREE(*table);
}

int  table_length(T table)
{
	assert(table);
	return table->length;
}
/**
 * 将key-value键值对添加到表中，如果table中已经包含key值，则更新，并且返回原来的value值
 * @param  table 表
 * @param  key   key
 * @param  value value
 * @return       key存在 返回原来的数据
 *               key不存在 返回NULL
 */
void *table_put(T table,const void *key,void *value)
{
	int i;
	struct binding *p;
	void *prev;

	assert(key);
	assert(value);

	//搜索对应的键是否已经存在，存在的话用新的value值覆盖
	i = ((*table->hash)(key)) % table->size;
	for(p = table->buckets[i];p;p = p->link)
	{
		if( ((*table->cmp)(key,p->key)) == 0 )
		{
			break;
		}
	}

	if(NULL == p)
	{
		//找不到键
		NEW(p);
		p->key = key;
		p->link = table->buckets[i];
		table->buckets[i] = p;
		table->length++;
		prev = NULL;
	}
	else
	{
		prev = p->value;
	}
	p->value = value;
	table->timestamp++;

	return prev;

}

/**
 * @brief 查找与键对应的值，首先得到键的哈希码，而后将哈希码对buckets桶中的元素取模，
 *        然后搜索对应的链表查找与key相同的键
 * @param  table [description]
 * @param  key   [description]
 * @return       [description]
 */
void *table_get(T table,const void *key)
{
	int i;
	struct binding *p;

	assert(table);
	assert(key);

	//获得键的hash码,然后对buckets桶中元素取模
	i = ((*table->hash)(key)) % table->size;
	//搜索
	for(p = table->buckets[i];p;p = p->link)
	{
		//判断是否与键key相等
		if( ((*table->cmp)(key,p->key)) == 0 )
		{
			break;
		}
	}

	return p ? p->value : NULL;

}

void *table_remove(T table,const void *key)
{
	int i;
	struct binding **pp;

	assert(table);
	assert(key);

	table->timestamp--;
	i = (*table->hash)(key) % table->size;
	for(pp = &table->buckets[i];*pp;pp = &(*pp)->link)
	{
		if( (*table->cmp)(key,(*pp)->key) == 0 )
		{
			struct binding *p = *pp;
			void *value = p->value;
			*pp = p->link;

			FREE(p);
			table->length--;
			return value;
		}
	}
	return NULL;
}
void table_map(T table,void apply(const void *key,void **value,void *c1),void *c1)
{
	int i;
	unsigned stamp;
	struct binding *p;

	assert(table);
	assert(apply);

	stamp = table->timestamp;
	for(i = 0;i < table->size;i++)
	{
		for(p = table->buckets[i];p;p = p->link)
		{
			apply(p->key,&p->value,c1);
			assert(table->timestamp == stamp);
		}
	}
}

void **table_toArray(T table,void *end)
{
	int i = 0;
	int j = 0;
	void **array;
	struct binding *p;

	assert(table);
	array = ALLOC( (2 * table->length + 1) * sizeof(*array) );
	for(i = 0;i < table->size;i++)
	{
		for(p = table->buckets[i];p;p = p->link)
		{
			array[j++] = (void *)p->key;
			array[j++] = p->value;
		}
	}

	array[j] = end;

	return array;
}
