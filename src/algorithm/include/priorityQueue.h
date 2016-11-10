#ifndef _PRIORITYQUEUE_H
#define _PRIORITYQUEUE_H

#define T pqueue_t
typedef struct T *T;


extern T 		pq_new(int itemsize,int length);
extern T        pq_new_by_array(void *data,int itemsize,int length);
/**
 * 将新元素加到数组末尾，增加堆的大小并让这个新元素上浮到合适的位置
 * @param  pq [description]
 * @param  sv [description]
 * @return    [description]
 */
extern void 	pq_insert(T pq,void *value);
/**
 * 从数组顶端删掉最大元素并将数组的最后一个元素放到顶端，减小堆的大小，并让这个元素下沉到合适的位置
 * @param  pq 堆
 * @return    无
 */
extern void    *pq_delMax(T pq);
extern int      pq_count(T pq);
extern int 		pq_length(T pq);
extern int 		pq_isEmpty(T pq);


extern void     pq_print(T pq);


#undef T
#endif
