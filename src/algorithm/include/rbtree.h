#ifndef RBTREE_H
#define RBTREE_H

#include "item.h"

#define T rbtree_t
typedef struct T *T;

struct T
{
	int count;
	struct rbtree_node
	{
		item_t item;
		unsigned char color;
		struct rbtree_node *left;
		struct rbtree_node *right;
	}*head;
};

extern T 	    rbtree_new();
extern void     rbtree_free(T tree);
extern int      rbtree_isEmpty(T tree);
extern int 	    rbtree_count(T tree);
extern void     rbtree_insert(T tree,item_t item);
extern item_t   rbtree_get(T tree,Key key);
extern item_t   rbtree_min(T tree);
extern item_t   rbtree_max(T tree);
extern int      rbtree_height(T tree);
extern void     rbtree_deleteMin(T tree);
extern void     rbtree_deleteMax(T tree);
// extern void     rbtree_delete(T tree,Key key);
extern void     rbtree_inorder(T tree);
#undef T
#endif
