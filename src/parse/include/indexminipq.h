/**
 * 索引优先队列
 */
#ifndef _INDEXMINPQ_H
#define _INDEXMINPQ_H

#define T iminipq_t
typedef struct T *T;

/**
 * 创建索引优先队列
 * @param  size 队列的容量
 * @return      新建的优先队列
 */
extern T        iminipq_new(int size);
/**
 * 释放优先队列的资源
 * @param  iminipq 优先队列
 */
extern void     iminipq_free(T *iminipq);
/**
 * 清空优先队列中的数据
 * @param  iminipq 优先队列
 */
extern void     iminipq_clean(T iminipq);
/**
 * 获取有限队列的容量
 * @param  iminipq [description]
 * @return         优先队列的容量
 */
extern int      iminipq_size(T iminipq);
/**
 * 获取有限队列的元素个数
 * @param  iminipq 优先队列
 * @return         该队列元素个数
 */
extern int      iminipq_count(T iminipq);

/**
 * 更新索引i的key值(i值已经在优先队列的索引中)
 * @param  iminipq 优先队列
 * @param  i       元素索引
 * @param  key     新的key值
 */
extern void     iminipq_change_key(T iminipq,int i,double key);
/**
 * 减小索引优先队列中索引为i的key值
 * @param  iminipq 索引优先队列
 * @param  i       索引值
 * @param  key     减小后的key值
 */
extern void     iminipq_decrease_key(T iminipq,int i,double key);
/**
 * 增加索引优先队列中索引为i的key值
 * @param  iminipq 索引优先队列
 * @param  i       索引值
 * @param  key     增加后的key值
 */
extern void     iminipq_increase_key(T iminipq,int i,double key);
/**
 * 删除索引优先队列的最小值
 * @param  iminipq 索引优先队列
 */
extern int      iminipq_delete_min(T iminipq);
/**
 * 删除索引为i的值
 * @param  iminipq 索引优先队列
 * @param  i       索引值
 */
extern void     iminipq_delete(T iminipq,int i);
/**
 * 在优先队列中插入数据
 * @param  iminipq 优先队列
 * @param  i       插入值的索引
 * @param  key     插入值的key
 */
extern void     iminipq_insert(T iminipq,int i,double key);

/**
 * 优先队列中是否包含索引i
 * @param  iminipq 优先队列
 * @param  i       索引
 * @return         1 包含 0 不包含
 */
extern int      iminipq_contains(T iminipq,int i);
/**
 * 判断优先队列是否为空
 * @param  iminipq 优先队列
 * @return         1 为空 0 不为空
 */
extern int      iminipq_is_empty(T iminipq);
/**
 * 获取索引为i的key值
 * @param  iminipq 索引优先队列
 * @param  i       索引值
 * @return         索引值对应的key值
 */
extern double   iminipq_key_of(T iminipq,int i);
extern int      iminipq_min_index(T iminipq);
extern double   iminipq_min_key(T iminipq);

extern void     iminipq_print(T iminipq);
#undef T
#endif
