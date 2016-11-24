#include <stdlib.h>
#include <float.h>

#include "assert.h"
#include "mem.h"
#include "indexminipq.h"

#define T iminipq_t

struct T
{
    int size;       //容量
    int count;      //已包含元素个数
    int *pq;        //从1开始的二叉堆
    int *qp;        //pq的对称映射 即qp[pq[i]] = i 用于映射key索引对应pq二叉堆中的位置
    double *keys;   //keys[i] 既是元素值
};

static void exch(T iminipq,int i,int j);
static int  greater(T iminipq,int i,int j);
static void sink(T iminipq,int k);
static void swim(T iminipq,int k);
/**
 * 创建索引优先队列
 * @param  size 队列的容量
 * @return      新建的优先队列
 */
T iminipq_new(int size)
{
    int i;
    T iminipq;

    assert(size >= 0);
    iminipq = ALLOC(sizeof(*iminipq));
    iminipq->size = size;
    iminipq->count = 0;
    iminipq->pq = ALLOC((size + 1) * sizeof(int));
    iminipq->qp = ALLOC((size + 1) * sizeof(int));
    iminipq->keys = ALLOC((size + 1) * sizeof(double));

    for(i = 0;i <= size;i++)
    {
        iminipq->qp[i] = -1;
    }

    return iminipq;
}

/**
 * 释放优先队列的资源
 * @param  iminipq 优先队列
 */
void     iminipq_free(T *iminipq)
{
    assert(iminipq);
    assert(*iminipq);
    FREE((*iminipq)->pq);
    FREE((*iminipq)->qp);
    FREE((*iminipq)->keys);
    FREE(*iminipq);
}
/**
 * 清空优先队列中的数据
 * @param  iminipq 优先队列
 */
void iminipq_clean(T iminipq)
{
    int size,i;
    assert(iminipq);
    size = iminipq->size;
    iminipq->count = 0;

    for(i = 0; i <= size; i++)
    {
        iminipq->qp[i] = -1;
        iminipq->keys[i] = DBL_MAX;
    }
}
int     iminipq_size(T iminipq)
{
    assert(iminipq);
    return iminipq->size;
}
int      iminipq_count(T iminipq)
{
    assert(iminipq);
    return iminipq->count;
}
/**
 * 更新索引i的key值(i值已经在优先队列的索引中)
 * @param  iminipq 优先队列
 * @param  i       元素索引
 * @param  key     新的key值
 */
void     iminipq_change_key(T iminipq,int i,double key)
{
    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    assert(iminipq_contains(iminipq,i));
    iminipq->keys[i] = key;

    swim(iminipq,iminipq->qp[i]);
    sink(iminipq,iminipq->qp[i]);

}
/**
 * 减小索引优先队列中索引为i的key值
 * @param  iminipq 索引优先队列
 * @param  i       索引值
 * @param  key     减小后的key值
 */
void     iminipq_decrease_key(T iminipq,int i,double key)
{
    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    assert(iminipq_contains(iminipq,i));
    assert(iminipq->keys[i] > key);
    iminipq->keys[i] = key;
    swim(iminipq,iminipq->qp[i]);
}
/**
 * 增加索引优先队列中索引为i的key值
 * @param  iminipq 索引优先队列
 * @param  i       索引值
 * @param  key     增加后的key值
 */
void     iminipq_increase_key(T iminipq,int i,double key)
{
    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    assert(iminipq_contains(iminipq,i));
    assert(iminipq->keys[i] < key);
    iminipq->keys[i] = key;
    sink(iminipq,iminipq->qp[i]);
}

/**
 * 删除索引优先队列的最小值
 * @param  iminipq 索引优先队列
 * @return         最小值的索引
 */
int iminipq_delete_min(T iminipq)
{
    assert(iminipq);
    assert(iminipq->count > 0);

    int min;
    min = iminipq->pq[1];
    exch(iminipq,1,iminipq->count);

    iminipq->count--;
    sink(iminipq,1);
    iminipq->qp[min] = -1;
    iminipq->keys[iminipq->pq[iminipq->count+1]] = DBL_MAX;
    iminipq->pq[iminipq->count+1] = -1;

    return min;
}
/**
 * 删除索引为i的值
 * @param  iminipq 索引优先队列
 * @param  i       索引值
 */
void     iminipq_delete(T iminipq,int i)
{
    int index;

    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    assert(iminipq_contains(iminipq,i));

    index = iminipq->qp[i];
    exch(iminipq,index,iminipq->count);
    iminipq->count--;
    swim(iminipq,index);
    sink(iminipq,index);

    iminipq->keys[i] = DBL_MAX;
    iminipq->qp[i] = -1;
}
/**
 * 在优先队列中插入数据
 * @param  iminipq 优先队列
 * @param  i       插入值的索引
 * @param  key     插入值的key
 *
 * 把新值插入到最后，然后上浮进行有序化优先队列
 */
void     iminipq_insert(T iminipq,int i,double key)
{
    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    assert(!iminipq_contains(iminipq,i)); //索引值已经在优先队列中进行告警
    iminipq->count++;
    iminipq->pq[iminipq->count] = i;
    iminipq->qp[i] = iminipq->count;
    iminipq->keys[i] = key;

    swim(iminipq,iminipq->count);
}

/**
 * 优先队列中是否包含索引i
 * @param  iminipq 优先队列
 * @param  i       索引
 * @return         1 包含 0 不包含
 */
int      iminipq_contains(T iminipq,int i)
{
    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    return iminipq->qp[i] != -1;
}
/**
 * 判断优先队列是否为空
 * @param  iminipq 优先队列
 * @return         1 为空 0 不为空
 */
int      iminipq_is_empty(T iminipq)
{
    assert(iminipq);
    return iminipq->count == 0;
}
/**
 * 获取索引为i的key值
 * @param  iminipq 索引优先队列
 * @param  i       索引值
 * @return         索引值对应的key值
 */
double   iminipq_key_of(T iminipq,int i)
{
    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    assert(iminipq_contains(iminipq,i));

    return iminipq->keys[i];
}
/**
 * 获取优先队列中最小值的索引
 * @param  iminipq 优先队列
 * @return         最小值的索引
 */
int      iminipq_min_index(T iminipq)
{
    assert(iminipq);
    assert(iminipq->count > 0);
    return iminipq->pq[1];
}
/**
 * 获取优先队列中最小的key值
 * @param  iminipq 优先队列
 * @return         最小的key值
 */
double   iminipq_min_key(T iminipq)
{
    assert(iminipq);
    assert(iminipq->count > 0);
    return iminipq->keys[iminipq->pq[1]];
}

static void exch(T iminipq,int i,int j)
{
    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    assert(j >= 0 && j <= iminipq->size);

    int swap = iminipq->pq[i];
    iminipq->pq[i] = iminipq->pq[j];
    iminipq->pq[j] = swap;

    iminipq->qp[iminipq->pq[i]] = i;
    iminipq->qp[iminipq->pq[j]] = j;
}
static int greater(T iminipq,int i,int j)
{
    assert(iminipq);
    assert(i >= 0 && i <= iminipq->size);
    assert(j >= 0 && j <= iminipq->size);

    return iminipq->keys[iminipq->pq[i]] > iminipq->keys[iminipq->pq[j]];
}
static void sink(T iminipq,int k)
{
    assert(iminipq);
    assert(k >= 0 && k < iminipq->size);

    while(2 * k <= iminipq->count)
    {
        int j = 2 * k;
        if(j < iminipq->count && greater(iminipq,j,j+1))
        {
            j++;
        }
        if(!greater(iminipq,k,j))
        {
            break;
        }

        exch(iminipq,k,j);
        k = j;
    }
}
static void swim(T iminipq,int k)
{
    assert(iminipq);
    assert(k >= 0 && k <= iminipq->size);

    while(k > 1 && greater(iminipq,k/2,k))
    {
        exch(iminipq,k,k/2);
        k = k / 2;
    }
}

void iminipq_print(T iminipq)
{
    for(int i = 0;i < iminipq->count;i++)
    {
        if(iminipq->keys[i] == DBL_MAX)
        {
            // // printf("MAX   ");
        }
        else
        {
            // // printf("(%d)%.2lf   ",iminipq->pq[i+1],iminipq->keys[i]);
        }

    }
    // printf("\n");
    // printf("  pq:");
    for(int i = 0;i <= iminipq->size;i++)
    {
        // printf("%d ",iminipq->pq[i]);
    }
    // printf("\n");

    // printf("  qp:");
    for(int i = 0;i <= iminipq->size;i++)
    {
        // printf("%d ",iminipq->qp[i]);
    }
    // printf("\n");

    // printf("keys:");
    for(int i = 0;i <= iminipq->size;i++)
    {
        if(iminipq->keys[i] == DBL_MAX)
        {
            // printf("MAX   ");
        }
        else
        {
            // printf("%.2lf   ",iminipq->keys[i]);
        }

    }
    // printf("\n");
}
