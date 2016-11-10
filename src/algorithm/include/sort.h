
#ifndef _SORT_H
#define _SORT_H

extern void swap(void *a,void *b,int size);
extern void q_sort(void *base,int start,int end,int size,int (*cmp)(const void *a,const void *b));
/**
 * 选择排序
 * @param a     目标数组
 * @param start 开始排序的位置
 * @param end   排序结束的位置
 * @param size  数组中每一个值的大小
 * @param cmp   比较函数，排序会根据比较函数进行，所以比较函数不能为空
 */
extern void select_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b));
extern void insert_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b));
extern void shell_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b));
extern void merge(void *a,int lo,int mid,int hi,int size,int (*cmp)(const void *a,const void *b));
//自顶向下的归并排序
extern void merge_sort(void *a,int lo,int hi,int size,int (*cmp)(const void *a,const void *b));
//自底向上的归并排序
extern void merge_sort2(void *a,int lo,int hi,int size,int (*cmp)(const void *a,const void *b));
//快速排序
extern void quick_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b));
//三项切分的快速排序
extern void quick3way_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b));
//堆排序
extern void heap_sort(void *a,int start,int end,int size,int (*cmp)(const void *a,const void *b));
extern int 	cmp_int(const void *a,const void *b);
extern int 	cmp_double(const void *a,const void *b);
extern int 	cmp_char(const void *a,const void *b);
extern int  cmp_string(const void *a,const void *b);
#endif
