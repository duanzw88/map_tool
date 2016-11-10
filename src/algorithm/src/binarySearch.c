#include <stdlib.h>

#include "binarySearch.h"

int binary_rank(int key,int a[],int len)
{
	//数组必须是有序的
	int lo = 0;
	int hi = len - 1;
	while(lo <= hi)
	{
		//被查找的值要么不存在，要么必然存在于a[lo..hi]之中
		int mid = lo + (hi - lo) / 2;
		if(key < a[mid])
		{
			hi = mid - 1;
		}
		else if(key > a[mid])
		{
			lo = mid + 1;
		}
		else
		{
			return mid;
		}
	}

	//执行到此处说明没有查找到
	return -1;
}
