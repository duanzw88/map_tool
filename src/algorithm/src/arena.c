#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "arena.h"

#define T arena_t

//为避免占用太多内存,仅支持10个大内存块
#define THRESHOLD 10
const Except_T Arena_NewFailed = {"Arena Creation Failed"};
const Except_T Arena_Failed = {"Arena Allocation Failed"};

struct T
{
	T 	  prev; //指向内存块的起始
	char *avail;//指向大内存块中的第一个空闲位置
	char *limit;//指向大内存块的结束处
	//对于每个大内存块，在avail和limit之间的空间可用于分配
};

//对齐要求
union align
{
	int 	i;
	long 	l;
	long	*lp;
	void 	*p;
	void	(*fp)(void);
	float	f;
	double 	d;
	long double ld;
};
union header
{
	struct T b;
	union align a;
};

static T freechunks;//空闲链表
static int nfree;//链表中大内存块的数目

/*
*分配一个内存池并返回一个arena_t的结构实例，各个字段均设置为NULL表示一个空得内存池
*/
T 	arena_new(void)
{
	T arena = malloc(sizeof(* arena));
	if(arena == NULL)
	{
		RAISE(Arena_NewFailed);
	}

	arena->prev = NULL;
	arena->limit = arena->avail = NULL;

	printf("arena = %p\n",arena);
	printf("arena->prev = %p\n",arena->prev);
	printf("arena->avail = %p\n",arena->avail);
	printf("arena->limit = %p\n",arena->limit);
	return arena;
}

/*
*调用arena_free释放内存池中的各个大内存块。将释放arena_t结构本身并将指向内存池的指针清零
*/
void arena_dispose(T *ap)
{
	assert(ap && *ap);

	arena_free(*ap);
	free(*ap);
	*ap = NULL;

}


void *arena_alloc(T arena,long nbytes,const char *file,int line)
{

	assert(arena);
	assert(nbytes > 0);

	//将请求分配的内存的长度向上舍入到适当的对齐边界
	//对nbytes向上舍入,满足最严格的对齐要求
	nbytes = ((nbytes + sizeof(union align) - 1) / (sizeof(union align))) * (sizeof(union align));

	while(nbytes > (arena->limit - arena->avail))
	{
		//当前内存块无法满足分配请求，重新分配一个新的大内存块
		T ptr;
		char *limit;
		//分配一个新的大内存块
		if((ptr = freechunks) != NULL)
		{
			freechunks = freechunks->prev;
			nfree--;
			limit = ptr->limit;
		}
		else
		{
			//从内存中重新分配内存块
			long m = sizeof(union header) + nbytes + 10*1024;
			ptr = malloc(m);

			printf("malloc ptr = %p\n",ptr);
			if(ptr == NULL)
			{
				if(file == NULL)
				{
					RAISE(Arena_Failed);
				}
				else
				{
					Except_raise(&Arena_Failed,file,line);
				}
			}
			limit = (char *)ptr + m;
		}

		//执行到此处说明已经重新分配了大内存块
		//*arena的当前值保存到该内存块的起始处，并初始化arean的各个字段使之指向新块
		*ptr = *arena;
		arena->avail = (char *)((union header *)ptr + 1);
		arena->limit = limit;
		arena->prev = ptr;
		printf("malloc ptr = %p\n",ptr);
		printf("malloc ptr->prev = %p\n",ptr->prev);
		printf("malloc ptr->avail = %p\n",ptr->avail);
		printf("malloc ptr->limit = %p\n",ptr->limit);

	}
	arena->avail += nbytes;

	printf("arena = %p\n",arena);
	printf("arena->prev = %p\n",arena->prev);
	printf("arena->avail = %p\n",arena->avail);
	printf("arena->limit = %p\n",arena->limit);
	return arena->avail - nbytes;
}
void *arena_calloc(T arena,long count,long nbytes,const char *file,int line)
{
	void *ptr;

	assert(count > 0);
	ptr = arena_alloc(arena,count*nbytes,file,line);
	memset(ptr,'\0',count*nbytes);

	return ptr;
}
void arena_free(T arena)
{
	assert(arena);
	while(arena->prev)
	{
		struct T tmp = *arena->prev;
		if(nfree < THRESHOLD)
		{
			arena->prev->prev = freechunks;
			freechunks = arena->prev;
			nfree++;
			freechunks->limit = arena->limit;
		}
		else
		{
			free(arena->prev);
		}
		*arena = tmp;
	}

	assert(arena->limit == NULL);
	assert(arena->avail == NULL);
}
