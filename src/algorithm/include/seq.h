#ifndef _SEQ_H
#define _SEQ_H

#define T seq_t
typedef struct T *T;

/**
 * 创建序列
 * @param  hint 对新序列将包含值的最大数目的估计
 * @return      一个空序列
 */
extern T seq_new(int hint);
/**
 * 创建并返回一个序列，函数用非NULL指针参数来创建初始化序列中的值
 * @param  x       [description]
 * @param  VARARGS [description]
 * @return         新的序列
 */
extern T seq_seq(void *x,...);

/**
 * 释放序列
 * @param seq 目标序列
 */
extern void seq_free(T *seq);

/**
 * 获取序列长度
 * @param  seq 目标序列
 * @return     序列长度
 */
extern int seq_length(T seq);
/**
 * 获取序列中的值
 * @param seq 目标序列
 * @param i   索引
 */
extern void *seq_get(T seq,int i);

/**
 * 按索引更改序列的值
 * @param  seq 目标序列
 * @param  i   索引
 * @param  x   新值
 * @return     返回索引对应的先前值
 */
extern void *seq_put(T seq,int i,void *x);

/**
 * 将x添加到seq的低端并返回x，添加到低端其实是序列的开始
 * @param  seq 目标序列
 * @param  x   要添加到低端的值
 * @return     添加的值
 */
extern void *seq_addstart(T seq,void *x);

/**
 * 将x添加到seq序列的高端并返回x
 * @param  seq 目标序列
 * @param  x   要添加到高端的值
 * @return     添加的值
 */
extern void *seq_addend(T seq,void *x);

/**
 * 删除低端的值 即删除序列开始的值
 * @param  seq 目标序列
 * @return     删除的值
 */
extern void *seq_removestart(T seq);

/**
 * 删除高端的值，即删除序列末端的值
 * @param  seq 目标序列
 * @return     删除的值
 */
extern void *seq_removeend(T seq);

#undef T
#endif
