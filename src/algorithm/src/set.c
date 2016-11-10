#include <limits.h>
#include <stddef.h>
#include "mem.h"
#include "assert.h"
// #include "arith.h"
#include "set.h"

#define T set_t //set_t是一个哈希表，其中通过链表来保存集合的成员

#define MAX(x,y) ( ((x) > (y)) ? (x) : (y))
#define MIN(x,y) ( ((x) < (y)) ? (x) : (y))
/**
 * 结合结构体
 */
struct T
{
	int  length;								//集合中成员的数目
	unsigned timestamp;							//用于实现set_map中的已检查的运行时错误，即禁止apply修改集合
	int (*cmp)(const void *x,const void *y);	//比较函数
	unsigned (*hash)(const void *x);			//哈希函数
	int size;									//桶的长度
	struct member								//hash桶是靠链表实现的
	{
		struct member *link;					//链表指针
		const void *member_data;				//链表数据
	}**buckets;									//桶
};


static int cmpatom(const void *x,const void *y)
{
	return x != y;
}
static unsigned hashatom(const void *x)
{
	return (unsigned long)x>>2;
}
/**
 * 集合考本
 * @param  t    要拷贝的集合
 * @param  hint 新集合的预设值，之所以要设置该值主要是因为，在拷贝之后新集合肯定会在以后使用
 *              所以新集合的预设值可能会比t->size大，这样做可以提高效率
 * @return      t的副本
 */
static T copy(T t,int hint)
{
	int i;
	T set;

	assert(t);
	set = set_new(hint,t->cmp,t->hash);

	struct member *q;
	for(i = 0;i < t->size;i++)
	{
		for(q=t->buckets[i]; q; q = q->link)
		{
			struct  member *p;
			const void *member_data = q->member_data;

			int index = (*set->hash)(member_data) % set->size;
			NEW(p);
			p->member_data = member_data;
			p->link = set->buckets[index];
			set->buckets[index] = p;
			set->length++;
		}
	}

	return set;
}
/**
 * 分配、初始化一个新的set集合实例
 * @param  hint  对集合预期会包含的成员数目的一个估计，准确的hint值会提高性能，任何非负值都是可接受的
 * @param  cmp	 用来比较两个成员
 * @param  hash	 用来将成员映射到无符号整数
 * @retval  新的T实例
 *
 * @other cmp(x,y)针对x小于y x等于y x大于y的情形必须分别返回小于0 等于0 大于0的整数
 *        如果cmp(x,y)返回0，那么x和y只有一个会出现在集合中，而且hash(x)必定等于hash(y)
 *        如果cmp为NULL函数指针，那么假定集合成员为原子，采用默认比较函数
 *        如果hash为NULL函数指针，采用默认hash函数
 */
T set_new(int hint,
				 int cmp(const void *x,const void *y),
			 	 unsigned hash(const void *x))
{
	T set;
	int i;
	static int primes[] = {509,509,1021,2053,4093,8191,16381,32771,INT_MAX};

	assert(hint >= 0);
	for(i = 1;primes[i] < hint;i++);
	set = ALLOC(sizeof(*set) + primes[i-1] * sizeof(set->buckets[0]) );
	set->length = 0;
	set->timestamp = 0;
	set->cmp = cmp ? cmp : cmpatom;
	set->hash = hash ?  hash : hashatom;
	set->size = primes[i-1];
	set->buckets = (struct member **)(set + 1);

	for(i = 0;i < set->size;i++)
	{
		set->buckets[i] = NULL;
	}

	return set;
}

void set_free(T *set)
{
	int i;
	assert(set && *set);
	struct member *p,*q;

	if((*set)->length > 0)
	{
		for(i = 0;i < (*set)->size;i++)
		{
			for(p=(*set)->buckets[i]; p; p = q)
			{
				q = p->link;
				FREE(p);
			}
		}
	}

	FREE(*set);
}

/**
 * 获取集合的势
 * @param  set  集合对象
 * @retval  集合的势 或其所包含的成员数目
 */
int 	set_length(T set)
{
	assert(set);
	return set->length;
}
/**
 * 查询member是否在集合中
 * @param  set    集合对象
 * @param  member 要查询的成员
 * @return  1	  member在set中
 *          0	  member不在set中
 */
int 	set_member(T set,const void *member_data)
{
	int i;
	struct member *p;

	assert(set);
	assert(member_data);

	i = (*set->hash)(member_data) % set->size;
	for(p = set->buckets[i];p;p=p->link)
	{
		if( (*set->cmp)(member_data,p->member_data) == 0)
		{
			break;
		}
	}

	return p != NULL;
}

/**
 * 向set中添加成员
 * @param set    集合对象
 * @param member 要添加的成员
 */
void set_put(T set,const void *member_data)
{
	int i;
	struct member *p;

	assert(set);
	assert(member_data);

	//查看member是否已经存在
	i = (*set->hash)(member_data) % set->size;
	for(p = set->buckets[i];p;p = p->link)
	{
		if( (*set->cmp)(member_data,p->member_data) == 0)
		{
			break;
		}
	}
	if(p == NULL)
	{
		//member不在set中
		NEW(p);
		p->member_data = member_data;
		p->link = set->buckets[i];
		set->buckets[i] = p;
		set->length++;
	}
	else
	{
		//member已经存在
		p->member_data = member_data;
	}

	set->timestamp++;

}

/**
 * 删除成员
 * @param  set    集合对象
 * @param  member 要删除的成员
 * @return  member在set中    删除的成员
 *          member不在set中      NULL
 */
void *set_remove(T set,const void *member_data)
{
	int i;
	struct member **pp;

	assert(set);
	assert(member_data);

	i = (*set->hash)(member_data) % set->size;
	for(pp = &set->buckets[i]; *pp;pp = &(*pp)->link)
	{
		if( (*set->cmp)(member_data,(*pp)->member_data) == 0)
		{
			struct  member *p = *pp;
			*pp = p->link;
			member_data = p->member_data;
			FREE(p);
			set->length--;
			return (void *)member_data;
		}
	}

	return NULL;
}

/**
 * 对集合每个成员都调用apply函数
 * @param set 集合对象
 * @param cl  [description]
 */
void set_map(T set,
					void apply(const void *member,void *cl),void *cl)
{
	int i;
	unsigned stamp;
	struct member *p;

	assert(set);
	stamp = set->timestamp;
	for(i = 0;i < set->size;i++)
	{
		for(p = set->buckets[i];p;p = p->link)
		{
			apply(p->member_data,cl);
			assert(set->timestamp == stamp);
		}
	}
}

/**
 * 转换为数组
 * @param  set 集合对象
 * @param  end 赋值给数组的第N+1个元素
 * @return     指向N+1个元素的数组
 */
void **set_toArray(T set,void *end)
{
	int i;
	int j = 0;
	void **array;
	struct member *p;
	assert(set);
	array = ALLOC( (set->length + 1) * sizeof(*array) );
	for(i = 0;i < set->size;i++)
	{
		for(p = set->buckets[i]; p; p=p->link)
		{
			array[j++] = (void *)p->member_data;
		}
	}
	array[j] = end;

	return array;
}



/**
 * 并集
 * @param  s 集合对象
 * @param  t 集合对象
 * @return  s+t
 */
T set_union(T s,T t)
{
	if(s == NULL)
	{
		assert(t);
		return copy(t,t->size);
	}
	else if(t == NULL)
	{
		return copy(s,s->size);
	}
	else
	{
		T set = copy(s,MAX(s->size,t->size));
		int i;
		int j;
		struct member *q;
		for(i = 0;i < t->size;i++)
		{
			for(q=t->buckets[i];q;q = q->link)
			{
				set_put(set,q->member_data);
			}
		}

		return set;
	}
}
/**
 * 交集
 * @param  s 集合对象
 * @param  t 集合对象
 * @return   s*t
 */
T set_inter(T s,T t)
{

	//不能用下面的方法直接返回NULL 因为数学中集合的操作返回值也是一个集合，即使是一个空的集合
	// if(s == NULL || t == NULL)
	// {
	// 	return NULL;
	// }

	if(s == NULL)
	{
		assert(t);
		return set_new(t->size,t->cmp,t->hash);
	}
	else if(t == NULL)
	{
		return set_new(s->size,s->cmp,s->hash);
	}


	T set = set_new(MIN(s->size,t->size),s->cmp,s->hash);
	assert(s->cmp == t->cmp && s->hash == t->hash);
	int i;
	struct member *q;
	for(i = 0;i < t->size;i++)
	{
		for(q=t->buckets[i]; q; q=q->link)
		{
			if(set_member(s,q->member_data))
			{
				// set_put(set,p->member_data);
				struct member *p;
				const void *member_data = q->member_data;

				int index = (*set->hash)(member_data) % set->size;

				NEW(p);
				p->member_data = member_data;
				p->link = set->buckets[index];
				set->buckets[index] = p;
				set->length++;
			}
		}
	}

	return set;
}
/**
 * 差集
 * @param  s 集合对象
 * @param  t 集合对象
 * @return   s-t
 */
T set_minus(T s,T t)
{
	if(s == NULL)
	{
		assert(t);
		//返回空集
		return set_new(t->size,s->cmp,t->hash);
	}
	else if(t == NULL)
	{
		return copy(s,s->size);
	}
	else
	{
		T set = set_new(MIN(s->size,t->size),s->cmp,s->hash);

		assert(s->cmp == t->cmp && s->hash == t->hash);
		int i;
		struct member *q;
		for(i = 0; i < s->size; i++)
		{
			for(q=s->buckets[i]; q; q = q->link)
			{
				if(!set_member(t,q->member_data))
				{
					struct member *p;
					const void *member_data = q->member_data;
					int index = (*set->hash)(member_data) % set->size;
					NEW(p);
					p->member_data = member_data;
					p->link = set->buckets[index];
					set->buckets[index] = p;
				}
			}
		}

		return set;
	}
}
/**
 * 对称差
 * @param  s 集合对象
 * @param  t 集合对象
 * @return   s/t
 */
T set_diff(T s,T t)
{
	if(s == NULL)
	{
		assert(t);
		return copy(t,t->size);
	}
	else if(t == NULL)
	{
		return copy(s,s->size);
	}
	else
	{
		T set = set_new(MIN(s->size,t->size),s->cmp,s->hash);
		assert(s->cmp == t->cmp && s->hash == t->hash);
		int i;
		struct member *q;
		//先遍历s中的数据
		for(i = 0;i < s->size;i++)
		{
			for(q=s->buckets[i]; q; q=q->link)
			{

				if(!set_member(t,q->member_data))
				{
					//s中的数据不再t中
					int index = (*set->hash)(q->member_data) % set->size;
					struct member *p;
					NEW(p);
					p->member_data = q->member_data;
					p->link = set->buckets[i];
					set->buckets[i] = p;
					set->length++;
				}
			}
		}

		return set;
	}
}
