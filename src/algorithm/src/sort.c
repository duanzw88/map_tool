/**
 * 排序算法集结
 * @author duanzw
 * 选择排序:
 * 		一种最简单的排序，
 * 		首先，找到数组中最小的那个元素，
 * 		其次，将它和数组的第一个元素交换位置
 * 		再次，在剩下的元素中找到最小的元素和数组的第二个元素交换
 * 		如此往复，直到将整个数组排序
 * 	  该算法的时间效率取决于比较的次数
 * 	  对于长度为N的数组，选择排序需要N*N/2次比较和N次交换
 * 	  特点：
 * 		 运行时间和输入无关
 * 		 数据移动是最少的：交换次数和数组的大小是线性关系
 * 插入排序:
 * 		与选择排序一样，当前索引左边的所有元素都是有序的，但他们的最终位置还不确定，
 * 		为了给更小的元素腾出空间，他们可能会被移动，
 * 		当索引数组达到右端是，数组排序就完成了
 * 	  插入排序所需的时间取决于元素中的初始顺序
 * 	  对于长度为N的数组，平均情况下排序需要约等于N*N/4次比较和约等于N*N/4次交换
 * 	  				  最坏的情况需要N*N/2次比较和N*N/2交换
 * 	  				  最好的情况需要N-1次比较和0次交换
 * 希尔排序:
 * 		希尔排序是插入排序的演化增强版，
 * 		希尔排序的思想是使数组中任意间隔为h的元素都是有序的，这样的数组称为h有序数组。
 * 		用插入法将h个子数组独立的排序，然后逐步减小h的值直到1 就完成了排序
 * 归并排序:
 * 		思想:归并排序是另一种利用分治法排序的算法，首先将数据集二等分为左右两个区间，分别在左右两个区间上递归使用归并排序算法，
 * 			然后将排序后的两个区间合并为一个有序区间。
 * 		将两个有序的数组归并成一个更大的有序数组
 * 		要将一个数组排序，可以先(递归的)将它分成两半分别排序，然后将结果归并起来
 * 		原地归并抽象方法：将数组的子数组a[lo .. mid]和数组a[mid+1 .. hi]归并成一个有序的数组并将结果存放在a[lo .. hi]中
 * 		自顶向下的归并排序:
 * 								a[0..15]
 * 									|
 * 					 -----------------------------------
 * 					 |                                 |
 * 			     a[0..7]                           a[8..15]
 * 					|								   |
 * 	        -----------------               ----------------------
 * 			|		        |               |                    |
 * 		a[0..3]	 		a[4..7] 		a[8..11]     		a[12..15]
 * 		   |			   |			   |         	        |
 * 	   --------- 	    --------       ----------	        ---------
 * 	   |       |	    |      |       |        |           |       |
 * a[0..1]  a[2..3]	a[4..5] a[6..7]	a[8..9] a[10..11]   a[12..13]  a[14..15]
 *
 * 快速排序：快速排序和归并排序是互补的，归并排序将数组分成两个子数组分别排序，并将有序的子数组归并以将整个数组排序
 * 			快速排序则是当两个数组都有序时，整个数组也就自然有序了
 * 三项切分的快速排序:
 * 			从左到右遍历数组一次，维护一个指针lt使得a[start..lt-1]中的元素都小于v
 * 								 一个指针gt使得a[gt+1..end]中的元素都大于v
 * 								 一个指针i使得a[lt..i-1]中的元素都等于v
 * 								 			a[i..gt]中的元素都还未确定
 * 		    一开始i和lo是相等的，用三向比较来直接处理一下情况:
 * 		    1.a[i]小于v 将a[lt]和a[i]交换，将lt和i加一
 * 		    2.a[i]大于v 将a[gt]和a[i]交换，将gt减一
 * 		    2.a[i]等于v 将i加一
 * 		    这些操作都会保证数组元素不变且缩小gt-i的值(这样循环才会结束)。另外，除非和切分元素相等，其他元素都会被交换
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mem.h"
#include "assert.h"
#include "sort.h"

static void sink(void *a,int key,int length,int itemsize,int (*cmp)(const void *a,const void *b));

void swap(void *a,void *b,int size)
{
	assert(a != NULL && b != NULL);
	void *buffer = (void *)malloc(size);
	memcpy(buffer,a,size);
	memcpy(a,b,size);
	memcpy(b,buffer,size);

	free(buffer);
}

void q_sort(void *base,int start,int end,int size,int (*cmp)(const void *a,const void *b))
{
	assert(base != NULL && size >= 1 && cmp != NULL);
	if(start >= end)
	{
		return;
	}

	char *pleft = (char *)base + start * size;
	char *pkey = (char *)base + (start + (end - start) / 2) * size;
	swap(pleft,pkey,size);
	int last = start;
	char *plast = (char *)base + last * size;
	for(int i = start + 1;i <= end; ++i)
	{
		char *pi = (char *)base + i * size;
		if(cmp(pi,pleft) < 0)
		{
			++last;
			plast = (char *)base + last * size;
			swap(pi,plast,size);
		}
	}
	q_sort(base,start,last - 1,size,cmp);
	q_sort(base,last + 1,end,size,cmp);
}
//选择排序
void select_sort(void *base,int start,int end,int size,int (*cmp)(const void *a,const void *b))
{
	int i;

	for(i = start; i <= end; i++)
	{
		char *min = (char *)base + i * size;
		for(int j = i + 1; j <= end; j++)
		{
			char *current = (char *)base + j * size;
			if(cmp(current,min) < 0)
			{
				min = current;
			}
		}
		char *data = (char *)base + i * size;
		swap(data,min,size);
	}

}
//插入排序
void insert_sort(void *base,int start,int end,int size,int (*cmp)(const void *a,const void *b))
{
	int i,j;
	for(i = start + 1; i <= end;i++)
	{
		for(j = i; j > 0; j--)
		{
			char *prev = (char *)base + (j-1) * size;
			char *after = (char *)base + j * size;
			if(cmp(prev,after) > 0)
			{
				swap(prev,after,size);
			}
		}
	}
}
//希尔排序
void shell_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b))
{
	int h = 1;
	int length = end - start + 1;
	while(h < length / 3)
	{
		h = 3 *h + 1;
	}
	while(h >= 1)
	{
		//将数组变为h有序
		for(int i = start;i <= end;i++)
		{
			//将a[i]插入到a[i-h],a[i-2*h] a[i-3*h]...之中
			for(int j = i;j >= h;j-=h)
			{
				char *prev = (char *)a + (j - h) * size;
				char *after = (char *)a + j * size;
				if(cmp(prev,after) > 0)
				{
					swap(prev,after,size);
				}
			}
		}
		h = h / 3;
	}
}

// void merge(void *a,int lo,int mid,int hi,int size,int (*cmp)(const void *a,const void *b))
// {
// 	printf("%s:%d merge(%d,%d,%d)\n",__FILE__,__LINE__,lo,mid,hi);
//
//
// 	// char  *pd = (char *)data;
// 	printf("---------0\n");
// 	char  *aux = (char *)malloc((hi - lo + 1) * size);
// 	printf("---------1\n");
// 	if(aux == NULL)
// 	{
// 		printf("malloc error!!\n");
// 		return;
// 	}
// 	// void *buffer = (void *)malloc(size);
// 	for(int k = lo; k <= hi; k++)
// 	{
// 		//将a[lo .. hi]复制到aux[lo .. hi]中
// 		memcpy(&aux[k * size],&a[k * size],size);
// 	}
// 	printf("---------2\n");
// 	//将a[lo .. mid] 和 a[mid+1 .. hi]归并
// 	int pleft = lo;
// 	int pright = mid + 1;
// 	int pm = 0;
// 	while (pleft <= mid || pright <= hi)
// 	{
// 		if(pleft > mid)
// 		{
// 			//左侧元素已经全部合并
// 			while(pright <= hi)
// 			{
// 				memcpy(&a[pm * size],&aux[pright * size],size);
// 				pm++;
// 				pright++;
// 			}
// 			continue;
// 		}
// 		else if(pright > hi)
// 		{
// 			while(pleft <= mid)
// 			{
// 				memcpy(&a[pm * size],&aux[pleft * size],size);
// 				pleft++;
// 				pm++;
// 			}
// 			continue;
// 		}
//
// 		if(cmp(&aux[pleft * size],&aux[pright * size]) < 0)
// 		{
// 			memcpy(&a[pm * size],&aux[pleft * size],size);
// 			pleft++;
// 			pm++;
// 		}
// 		else
// 		{
// 			memcpy(&a[pm * size],&aux[pright * size],size);
// 			pright++;
// 			pm++;
// 		}
//
// 	}
// 	free(aux);
//
// 	printf("%s:%d merge...end\n",__FILE__,__LINE__);
// }

void merge(void *data,int lpos,int dpos,int rpos,int esize,int (*cmp)(const void *a,const void *b))
{
	//printf("%s:%d merge(%d,%d,%d)\n",__FILE__,__LINE__,lpos,dpos,rpos);

	char*   pd = (char*)data;
    char*   m;
    int pl, pr, pm;

    /* 初始化指示器 */
    pl = lpos;          /* pl指向分割后的左分区 */
    pr = dpos + 1;      /* pr指向分割后的右分区 */
    pm = 0;             /* pm指向合并后的分区 */

    /* 分配归并后结果的存储空间 */
    if((m = (char*)malloc((rpos - lpos + 1) * esize)) == NULL)
        return ;

    /* 两个区间中仍然有需要合并的元素 */
    while(pl <= dpos || pr <= rpos)
    {
        if(pl > dpos)
        {
            /* 左分区元素已经全部合并 */
            while(pr <= rpos)               /* 此时将右分区剩下的元素全部接在m的后边 */
            {
                memcpy(&m[pm * esize], &pd[pr * esize], esize);
                pr++;
                pm++;
            }
            continue;
        }
        else if(pr > rpos)
        {
            /* 右分区元素已经全部合并 */
            while(pl <= dpos)               /* 此时将左分区剩下的元素全部接在m的后边 */
            {
                memcpy(&m[pm * esize], &pd[pl * esize], esize);
                pl++;
                pm++;
            }
            continue;
        }

        /* 将下一个有序集合添加到合并结果中 */
        if(cmp(&pd[pl * esize], &pd[pr * esize]) < 0)
        {
            memcpy(&m[pm * esize], &pd[pl * esize], esize);
            pl++;
            pm++;
        }
        else
        {
            memcpy(&m[pm * esize], &pd[pr * esize], esize);
            pr++;
            pm++;
        }
    }

    /* 复制合并结果到data中，然后释放m */
    memcpy(&pd[lpos * esize], m, (rpos - lpos + 1) * esize);

    /* 释放空间 */
    free(m);
}
//自顶向下的归并排序
void merge_sort(void *a,int lo,int hi,int size,int (*cmp)(const void *a,const void *b))
{
	//printf("%s:%d merge_sort(%d,%d)\n",__FILE__,__LINE__,lo,hi);
	int mid;

	if(hi <= lo)
	{
		return;
	}

	/*计算中间位置*/
	mid = lo + (hi - lo) / 2;
	/*递归进行对左分区进行归并排序*/
	merge_sort(a,lo,mid,size,cmp);
	merge_sort(a,mid+1,hi,size,cmp);
	merge(a,lo,mid,hi,size,cmp);
}
//自底向上的归并排序
void merge_sort2(void *a,int lo,int hi,int size,int (*cmp)(const void *a,const void *b))
{
	//int length = hi - lo + 1;
	for(int step=1; step <= hi-lo+1; step+=step)
	{
		//printf("size = %d\n",step);
		for(int j=lo; j<= hi-step; j+=step+step)
		{
			int min = j+step+step < hi ? j+step+step-1 : hi;
			//printf("merge(a,%d,%d,%d)\n",j,j+step-1,min);
			merge(a,j,j + step - 1,min,size,cmp);
		}
		printf("\n");
	}
}

static int partition(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b))
{
	//printf("%s:%d a[%d .. %d]\n",__FILE__,__LINE__,start,end);

	//将数组切分为a[start..i-1] a[i] a[i+1..end]
	int i = start;
	int j = end;

	char *data = (char *)a + start * size;
	while(1)
	{
		while(i < end)
		{
			char *idata = (char *)a + (i) * size;

			if(cmp(idata,data) > 0)
			{
				//printf("%s:%d 左 %c\n",__FILE__,__LINE__,*idata);
				break;
			}
			i++;
		}
		while(j > start)
		{
			char *jdata = (char *)a + (j) * size;
			if(cmp(data,jdata) > 0)
			{
				//printf("%s:%d 右 %c\n",__FILE__,__LINE__,*jdata);
				break;
			}
			j--;
		}
		if(i >= j)
		{
			break;
		}
		char *idata = (char *)a + i * size;
		char *jdata = (char *)a + j * size;
		//printf("%s:%d %c---%c\n",__FILE__,__LINE__,*idata,*jdata);
		swap(idata,jdata,size);
	}
	char *jdata = (char *)a + j * size;
	swap(data,jdata,size);

	return j;
}
//快速排序
void quick_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b))
{
	if(end <= start)
	{
		return;
	}
	//切分
	int j = partition(a,start,end,size,cmp);
	// 将左半部分a[start..j-1]排序
	quick_sort(a,start,j-1,size,cmp);
	// 将有半部分a[j..end]排序
	quick_sort(a,j+1,end,size,cmp);
}
// void print_data(char *a,int start,int end,int lt,int i,int gt,int flag)
// {
// 	char *buffer = (char *)malloc((end - start + 1) * sizeof(char));
// 	memcpy(buffer,a,(end-start+1));
// 	printf("%s\n",buffer);
// 	if(flag)
// 	{
// 		for(int j = start;j <= end;j++)
// 		{
// 			if(j == lt)
// 			{
// 				printf("l");
// 			}
// 			else if(j == gt)
// 			{
// 				printf("g");
// 			}
// 			else if(j == i)
// 			{
// 				printf("i");
// 			}
// 			else
// 			{
// 				printf("*");
// 			}
// 		}
// 		printf("\n\n");
// 	}
//
//
// }
//三项切分的快速排序
void quick3way_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b))
{
	if(end <= start)
	{
		return;
	}
	int lt = start;
	int i = start + 1;
	int gt = end;


	while(i <= gt)
	{
		char *v = (char *)a + lt * size;
		char *idata = (char *)a + i * size;
		int cmpret = cmp(idata,v);
		//printf("cmp(%c %c) = %d\n",*idata,*v,cmpret);
		if(cmpret < 0)
		{
			//printf("lt....(%d %d %d)\n",lt,i,gt);
			// print_data(a,start,end,lt,i,gt,0);
			char *ltdata = (char *)a + lt * size;
			swap(ltdata,idata,size);
			lt++;
			i++;
			// print_data(a,start,end,lt,i,gt,1);

		}
		else if(cmpret > 0)
		{
			//printf("gt....(%d %d %d)\n",lt,i,gt);
			char *gtdata = (char *)a + gt * size;
			// print_data(a,start,end,lt,i,gt,0);
			swap(idata,gtdata,size);
			gt--;
			// print_data(a,start,end,lt,i,gt,1);
		}
		else
		{
			i++;
		}

	}


	quick3way_sort(a,start,lt-1,size,cmp);
	// 将有半部分a[j..end]排序
	quick3way_sort(a,gt+1,end,size,cmp);
}

/**
 * 堆排序
 * @param a     排序的目标数组
 * @param start 起始位置
 * @param end   结束位置
 * @param size  单个数据元素的大小
 * @param cmp   排序中数据元素的比较方法
 */
void heap_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b))
{
	int length = end - start + 1;
	for(int k = length / 2 - 1; k >= start;k--)
	{
		sink(a,k,length,size,cmp);
	}
	for(int i = length-1; i > 0; --i)
	{
		//最大元素存在堆的顶部，即数组中的0下标
		void *max = (void *)a + 0 * size;
		void *last = (void *)a + i * size;
		//printf("max = %c last = %c\n",*(char *)max,*(char *)last);
		swap(max,last,size);
		sink(a,0,i,size,cmp);
		//printf("\n\n");
	}

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
static void sink(void *a,int key,int length,int itemsize,int (*cmp)(const void *a,const void *b))
{
	assert(a);
	// printf("length = %d key = %d\n",length,key);
	// printf("**************************************\n");
	// for(int i = 0;i < 11;i++)
	// {
	// 	char *data = (char *)a + i * itemsize;
	// 	printf("%c ",*data);
	// }
	// printf("\n---------------------------------------\n");
	//由于数组书从0开始的下标
	//所以 左子节点的位置 = 父节点的位置 * 2 + 1；
	//右子节点的位置 = 左子节点的位置 + 1 =  父节点的位置 * 2  + 2；
	while(2 * key + 1 < length)
	{
		void *father = (void *)a + key * itemsize;
		int j = 2 * key + 1;
		if(j < length-1)
		{
			void *lchild = (void *)a + j * itemsize;
			void *rchild = (void *)a + (j + 1) * itemsize;
			//判断左子树和右子树的大小
			//如果是右子树大，则下沉到右子树
			if(cmp(lchild,rchild) < 0)
			{
				j++;
			}
		}
		// printf("key = %d j = %d\n",key,j);
		//j子树对应的数据最大
		void *child = (void *)a + j * itemsize;
		if(cmp(father,child) > 0)
		{
			break;
		}

		swap(father,child,itemsize);
		// pq_print(pq);
		key = j;
	}

	// for(int i = 0;i < 11;i++)
	// {
	// 	char *data = (char *)a + i * itemsize;
	// 	printf("%c ",*data);
	// }
	// printf("\n**************************************\n");
}
int cmp_int(const void *a,const void *b)
{
	assert(a != NULL && b != NULL);
	const int * lhs = (const int *)a;
	const int * rhs = (const int *)b;

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
	return *lhs - *rhs;

}
int  cmp_string(const void *a,const void *b)
{
	assert(a != NULL && b != NULL);
	const char ** lhs = (const char **)a;
	const char ** rhs = (const char **)b;

	return strcmp(*lhs,*rhs);
}
