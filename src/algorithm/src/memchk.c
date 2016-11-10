#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "assert.h"
#include "except.h"
#include "mem.h"

const Except_T Mem_Failed = {"Allocation Failed"};

//checking types 58
#define HTAB_LEN 2048


union align
{
	int 		i;
	long 		l;
	long 		*lp;
	void 		*p;
	void 		(*fp)(void);
	float 		f;
	double 		d;
	long double ld;
};
//data 54

static struct descriptor
{
	struct descriptor *free;
	struct descriptor *link;	//该字段构建了一个块描述符的链表，这些块散列到htab的同一索引

	const void *ptr;	//块地址
		  long size;	//块长度
	const char *file;	//该块分配的坐标(文件和行号)
		  int line;		//
}*htab[HTAB_LEN];


//htab本身是一个描述符指针的数组
//htab中的描述符还形成了一个空闲块的链表，该链表通过描述符的free字段建立。该链表是环形的：freelist是链表中的最后一个描述符，
//其free字段指向链表中的第一个描述符
//在任一时刻，htab中包含了所有块的描述符，包括空闲块和已分配块，同时空闲块还出现在freelist链表上。
//如果描述符的free字段为NULL，则该块已经分配，如果free字段不是NULL，则该块是空闲的
//通过上述描述，htab实现了集合S
static struct descriptor freelist = {.free = &freelist};
#define NDESCRIPTORS 512

#define NALLOC ( (4096 + sizeof(union align) - 1) / (sizeof(union align))) * (sizeof(union align))
#define hash(p,t) (((unsigned long)(p) >> 3) & (sizeof((t)) / sizeof((t)[0]) - 1))
/**
 * 搜索ptr地址的描述符
 * @param  ptr 目标地址
 * @return     指向描述符的指针或者NULL值
 */
static struct descriptor *find(const void *ptr)
{
	struct descriptor *bp = htab[hash(ptr,htab)];

	while(bp && bp->ptr != ptr)
	{
		bp = bp->link;
	}

	return bp;
}

/*
*分配描述符
*/
static struct descriptor *dalloc(void *ptr,long size,const char *file,int line)
{
	static struct descriptor *avail;
	static int nleft;

	if(nleft <= 0)
	{
		avail = malloc(NDESCRIPTORS * sizeof(*avail));
		if(avail == NULL)
		{
			return NULL;
		}

		nleft = NDESCRIPTORS;
	}

	avail->ptr = ptr;
	avail->size = size;
	avail->file = file;
	avail->line = line;
	avail->free = avail->link = NULL;

	nleft--;

	return avail++;
}

void mem_free(void *ptr,const char *file,int line)
{
	if(ptr)
	{
		struct descriptor *bp;
		if( ((unsigned long)ptr) % (sizeof(union align)) != 0 || (bp = find(ptr)) == NULL || bp->free )
		{
			Except_raise(&Assert_Failed,file,line);
		}
		//执行到此说明ptr不为NULL 且是一个有效地址
		bp->free = freelist.free;
		freelist.free = bp;

	}
}

//重新分配内存
void *mem_resize(void *ptr,long nbytes,const char *file,int line)
{
	struct descriptor *bp;
	void *newPtr;

	assert(ptr);
	assert(nbytes > 0);

	if( ((unsigned long)ptr) % (sizeof(union align)) != 0 || (bp = find(ptr)) == NULL || bp->free )
	{
		Except_raise(&Assert_Failed,file,line);
	}

	newPtr = mem_alloc(nbytes,file,line);
	memcpy(newPtr,ptr,nbytes < bp->size ? nbytes : bp->size);
	mem_free(ptr,file,line);
	return newPtr;
}
/*
* mem_alloc使用最先适配算法分配内存
* 如果对mem_alloc分配一个足够大的内存块，使他能够容纳一个描述符以及调用mem_alloc所请求的内存空间
* 这种做法有两个缺点:
* 1.它使划分一块空闲内存以满足几个较小请求的工作复杂化，因为每个请求自身都需要描述符
* 2.它使描述符易受到破坏，当通过指针或索引写内存越界时，就会发生
* 目前采用的方法：
* 独立分配描述符，解除了描述符分配与mem_alloc进行内存分配之间的耦合，并减少了描述符被破坏的可能性
* dalloc会分配、初始化一个描述符，这来自 由malloc分配的包含512个描述符的内存块
*/
void *mem_alloc(long nbytes,const char *file,int line)
{
	struct descriptor *bp;
	void *ptr;

	assert(nbytes > 0);


	//对nbytes向上舍入,使得其返回的每个指针都对齐到联合union align大小的倍数
	nbytes = (
		(nbytes + sizeof(union align) - 1) /
		(sizeof(union align))) *
		(sizeof(union align));
	//
	for(bp = freelist.free;bp;bp = bp->free)
	{
		//printf("bp->size = %ld  nbytes = %ld\n",bp->size,nbytes);
		//首先从空闲内存块中查找是否有足够大的内存块
		if(bp->size > nbytes)
		{
			//从空闲内存块分配
			bp->size -= nbytes;
			ptr = (char *)bp->ptr + bp->size;
			if( (bp = dalloc(ptr,nbytes,file,line)) != NULL)
			{
				unsigned int h = hash(ptr,htab);
				bp->link = htab[h];
				htab[h] = bp;
				return ptr;
			}
			else
			{
				if(file == NULL)
				{
					RAISE(Mem_Failed);
				}
				else
				{
					Except_raise(&Mem_Failed,file,line);
				}
			}
		}
		if(bp == &freelist)
		{
			struct descriptor *newPtr;
			if( (ptr = malloc(nbytes + NALLOC)) == NULL
				 || (newPtr = dalloc(ptr,nbytes + NALLOC,__FILE__,__LINE__)) == NULL)
			{
				if(file == NULL)
				{
					RAISE(Mem_Failed);
				}
				else
				{
					Except_raise(&Mem_Failed,file,line);
				}
			}

			newPtr->free = freelist.free;
			freelist.free = newPtr;
		}
	}

	assert(0);
	return NULL;
}

void *mem_calloc(long count,long nbytes,const char *file,int line)
{
	void *ptr;

	assert(count > 0);
	assert(nbytes > 0);

	ptr = mem_alloc(count * nbytes,file,line);
	memset(ptr,'\0',count * nbytes);

	return ptr;
}


void mem_leak(void (*apply)(const void *ptr,
                            long size,
                            const char *file,
                            int line,
                            void *cl),
                void *cl)
{
    struct descriptor *bp;
    int i;


    for(i = 0; i < HTAB_LEN; i++){
        bp = htab[i];

        while(bp && !bp->free){

            (*apply)(bp->ptr,
                    bp->size,
                    bp->file,
                    bp->line,
                    cl);
            bp = bp->link;
        }
    }
}
