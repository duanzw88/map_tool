#ifndef _QUADTREE_H
#define _QUADTREE_H

#define T quadtree_t
typedef struct T *T;

/**
 * 创建四叉树节点
 */
extern T quadtree_new(double left,double top,double right,double bottom);
/**
 * 对四叉树节点插入
 */
extern void quadtree_insert()
/**
 * 对四叉树节点删除
 */
/**
 * 四叉树节点检测节点相交性
 */
/**
 * 四叉树节点检测包含性
 */
#endif
