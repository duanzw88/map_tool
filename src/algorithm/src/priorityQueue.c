/**
 * 优先队列：
 * 		普通的队列是一种先进先出的数据结构，元素在队列尾追加。
 * 		在优先队列中，元素被赋予优先级，当访问元素时，具有最高优先级的元素最先删除。
 * 		优先队列具有最高级先出的行为特征
 * 	优先队列支持两种基本操作：
 * 		1.向优先队列中插入一个新的数据项
 * 		2. 删除(最大)优先队列中的关键字最大的数据项
 * 	优先队列具有很好的灵活性，能支持的操作如下:
 * 		1.根据N各数据项构造一个优先队列
 * 		2.插入一个数据项
 * 		3.删除最大值的数据项
 * 		4.改变任意给定的数据项的优先级
 * 		5.删除任意给定的数据项
 * 		6.合并两个优先队列为一个优先队列
 */
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "mem.h"
#include "priorityQueue.h"

#define T pqueue_t
/**
 * 基于堆的完全二叉树
 * 数据存储于data[1..N]中，pq[0]没有使用，方便使用
 * 因为，位置k的节点的子节点位于2k和2k+1
 */
struct T
{
	void *data;		//优先队列的指针
	int itemsize;	//优先队列中元素的字节大小
	int count;		//优先队列已有元素的个数
	int length;		//优先队列的长度(容量)
};

static int cmp_int(const void *a,const void *b);
static int cmp_double(const void *a,const void *b);
static int cmp_char(const void *a,const void *b);
static void swap(void *a,void *b,int size);
static void swim(T pq,int key,int (*cmp)(const void *a,const void *b));
static void sink(T pq,int key,int (*cmp)(const void *a,const void *b));


T pq_new(int itemsize,int length)
{
	assert(itemsize > 0);
	T pq;

	NEW(pq);
	pq->itemsize = itemsize;
	pq->length = length;
	pq->count = 1;			//由于data[0]不用，所以默认有一个元素
	pq->data = ALLOC(itemsize * length + 1);
	memset(pq->data,0,itemsize*length+1);

	return pq;
}
T pq_new_by_array(void *data,int itemsize,int length)
{
	assert(data);
	assert(itemsize > 0);

	T pq = pq_new(itemsize,length);

	for(int i = 0;i < length;i++)
	{
		void *value = (void *)data + i * itemsize;
		pq_insert(pq,value);
	}

	return pq;
}
/**
 * 将新元素加到数组末尾，增加堆的大小并让这个新元素上浮到合适的位置
 * @param  pq [description]
 * @param  sv [description]
 * @return    [description]
 */
void 	pq_insert(T pq,void *value)
{
	assert(pq);

	//把元素添加到最后
	void *data = (void *)pq->data + pq->count * pq->itemsize;
	memcpy(data,value,pq->itemsize);
	if(pq->itemsize == sizeof(int))
	{
		swim(pq,pq->count,cmp_int);
	}
	else if(pq->itemsize == sizeof(double))
	{
		swim(pq,pq->count,cmp_double);
	}
	else if(pq->itemsize == sizeof(char))
	{
		swim(pq,pq->count,cmp_char);
	}

	pq->count++;
	if(pq->count >= pq->length)
	{
		pq->count = pq->length;
	}
}
/**
 * 从数组顶端删掉最大元素并将数组的最后一个元素放到顶端，减小堆的大小，并让这个元素下沉到合适的位置
 * @param  pq 堆
 * @return    无
 */
void    *pq_delMax(T pq)
{
	assert(pq);
	assert(pq->count >= 1);

	void *data = pq->data + 1 * pq->itemsize;
	void *last = pq->data + pq->count * pq->itemsize;
	swap(data,last,pq->itemsize);
	pq->count--;
	if(pq->itemsize == sizeof(int))
	{
		sink(pq,1,cmp_int);
	}
	else if(pq->itemsize == sizeof(double))
	{
		sink(pq,1,cmp_double);
	}
	else if(pq->itemsize == sizeof(char))
	{
		sink(pq,1,cmp_char);
	}

	return pq->data + (pq->count+1) * pq->itemsize;
}
int      	pq_count(T pq)
{
	assert(pq);
	return pq->count;
}
int 		pq_length(T pq)
{
	assert(pq);
	return pq->length;
}
int 		pq_isEmpty(T pq)
{
	assert(pq);
	return pq->count == 0;
}

/**
 * 堆的有序化(下沉)
 * 堆的有序状态因为某个节点变得比它的两个子节点或其中一个节点小，该堆的有序状态被打破
 * 通过将它和它的两个子节点中比较大者交换来恢复堆
 * 交换可能会在子节点处继续打破堆的有序状态，因此需要不断地用相同的方式将其修复，将节点向下移动知道它
 * 的子节点都比它小或者达到了堆的底部
 * @param  pq  [description]
 * @param  key [description]
 * @return     [description]
 */
static void sink(T pq,int key,int (*cmp)(const void *a,const void *b))
{
	assert(key <= pq->length);

	while(2 * key <= pq->count)
	{

		void *father = pq->data + key * pq->itemsize;
		int j = 2 * key;
		if(j < pq->length)
		{
			void *lchild = pq->data + j * pq->itemsize;
			void *rchild = pq->data + (j + 1) * pq->itemsize;
			//判断左子树和右子树的大小
			//如果是右子树大，则下沉到右子树
			if(cmp(lchild,rchild) < 0)
			{
				j++;
			}
		}

		//j子树对应的数据最大
		void *child = pq->data + j * pq->itemsize;
		if(cmp(father,child) > 0)
		{
			break;
		}
		// printf("key = %d j = %d\n",key,j);
		swap(father,child,pq->itemsize);
		// pq_print(pq);
		key = j;
	}
}
void pq_print(T pq)
{
	assert(pq);
	for(int i = 1;i <= pq->count;i++)
	{
		double *data = (double *)(pq->data + i * pq->itemsize);
		// printf("%d = %f\n",i,*data);
	}
}

/**
 * 堆的有序化(上浮)
 * 堆的有序状态因为某个节点变得比它父节点更大，需要通过交换它和它的父节点来修复堆，该方法称作节点上浮
 * 但是该节点仍然可能比它现在的父节点更大，需要将该节点不断的向上移动知道遇到一个更大的父节点
 * @param  pq   堆
 * @param  k	该位置上的节点大于它的父节点
 * @return      无
 */
void  swim(T pq,int key,int (*cmp)(const void *a,const void *b))
{
	assert(pq);
	//printf("原始K = %d\n",key);
	while(key > 1)
	{
		void *child   = pq->data + key * pq->itemsize;
		void *father  = pq->data + (key/2) * pq->itemsize;
		if(cmp(child,father) > 0)
		{
			swap(child,father,pq->itemsize);
			key = key / 2;
	//		printf("key = %d\n",key);
		}
		else
		{
	//		printf("OK = %d\n",key);
			break;
		}

	}
}

void swap(void *a,void *b,int size)
{
	assert(a != NULL && b != NULL);
	void *buffer = (void *)malloc(size);
	memcpy(buffer,a,size);
	memcpy(a,b,size);
	memcpy(b,buffer,size);

	free(buffer);
}
int cmp_int(const void *a,const void *b)
{
	assert(a != NULL && b != NULL);
	const int * lhs = (const int *)a;
	const int * rhs = (const int *)b;

	return (*lhs) - (*rhs);
	// if(*lhs < *rhs)
	// {
	// 	return -1;
	// }
	// else if(*lhs == *rhs)
	// {
	// 	return 0;
	// }
	// else
	// {
	// 	return 1;
	// }
	//return (*(int *)a - *(int *)b);

}
int cmp_double(const void *a,const void *b)
{
	assert(a != NULL && b != NULL);
	const double * lhs = (const double *)a;
	const double * rhs = (const double *)b;

	if(*lhs < *rhs)
	{
		return -1;
	}
	else if(*lhs == *rhs)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int cmp_char(const void *a,const void *b)
{
	assert(a != NULL && b != NULL);
	const char * lhs = (const char *)a;
	const char * rhs = (const char *)b;

	if(*lhs < *rhs)
	{
		return -1;
	}
	else if(*lhs == *rhs)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
