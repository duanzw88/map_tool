/**
 * 空间节点
 */

#ifndef _QUADNODE_H
#define _QUADNODE_H

#define T quadnode_t
typedef struct T *T;

/**
 * 创建四叉树节点
 */
extern T quadnode_new(double left,double top,double right,double bottom);
/**
 * 对四叉树节点插入
 */
extern void quadnode_insert(T qn)
/**
 * 对四叉树节点删除
 */
/**
 * 四叉树节点检测节点相交性
 */
/**
 * 四叉树节点检测包含性
 */
#undef T
#endif
