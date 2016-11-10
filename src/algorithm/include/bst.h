/**
 * 二叉查找树
 */

#ifndef BST_H
#define BST_H

#include "item.h"

#define T bst_t
typedef struct T *T;

//创建一个新的二叉查找树
extern T 	    bst_new();
//释放二叉查找树空间
extern void     bst_free(T bst);
//判断二叉查找树是否为空
extern int      bst_isEmpty(T bst);
//获取元素个数
extern int 	    bst_count(T bst);
//插入元素
extern void     bst_insert(T bst,item_t item);
//获取元素
extern item_t   bst_get(T bst,Key key);
//获取最小元素
extern item_t   bst_min(T bst);
//获取最大元素
extern item_t     bst_max(T bst);
//二叉查找树的高度
extern int     bst_height(T bst);
//删除最小元素
extern void     bst_deleteMin(T bst);
//删除最大元素
extern void     bst_deleteMax(T bst);
//删除元素
extern void     bst_delete(T bst,Key key);
//二叉查找树转换成数组(已经排序完成)
extern void   bst_toArray(T bst,item_t *array);
//中序遍历
extern void     bst_inorder(T bst);

#undef T
#endif
