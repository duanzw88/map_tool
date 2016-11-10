/**
 * 红黑树
 * date:2016-7-6
 *
 *
 * 红黑树的思想:
 * 		基本思想是用标准的二叉查找树(完全由2-结点构成)和一些额外的信息(替换3-结点)来表示2-3树
 * 		将树种的链接分为两种类型：
 * 						红链接将两个2-结点连接起来构成一个3-结点
 * 						黑链接是2-3树种普通的链接
 * 红黑树的性质:
 * 		1.每个节点或是红色或者是黑色的
 * 		2.根节点是黑色的
 * 		3.每个叶子节点都是黑色的
 * 		4.如果一个节点是红色的，则它的两个子结点都是黑色的
 * 		5.对每个结点，该节点到其所有后代叶子节点的简单路径上，均包含相同数目的黑色节点
 * 红黑树的定义:
 * 		1.红链接均为左链接
 * 		2.没有任何一个节点同时和两条红链接相连
 * 		3.该树是完美平衡的，即任意空链接到根节点路径上的黑链接数量相同
 * 红黑树的优点:
 * 		1.具有二叉查找树的高效简洁的查找方法
 * 		2.具有2-3树种高效的平衡插入算法
 * 红黑树表示：
 * 		每个结点都只会有一条指向自己的链接(从它的父节点指向它)，将该链接的颜色保存在表示结点的数据类型的布尔变量color中
 * 		如果指向它的链接的颜色是红色的，则color为true(1)
 * 		如果指向它的链接是黑色的，则color为false(0)
 * 		当我们提到一个结点的颜色时，指的是指向该节点的链接的颜色
 * 红链接在树种向上传递，
 * 		在沿着插入点到根节点的路径向上移动时在所经过的每个结点中顺序完成以下操作：
 * 		1.如果右子节点是红色而左子节点是黑色，进行左旋
 * 		2.如果左子节点是红色，且它的左子节点也是红色，进行右旋
 * 		3.如果左右子节点均为红色，进行颜色转换
 */

#include <stdlib.h>
#include <stdio.h>

#include "mem.h"
#include "assert.h"
#include "rbtree.h"

#define T rbtree_t
#define RED 	1
#define BLACK 	0

// struct T
// {
// 	int count;
// 	struct rbtree_node
// 	{
// 		item_t item;
// 		unsigned char color;
// 		struct rbtree_node *left;
// 		struct rbtree_node *right;
// 	}*head;
// };
typedef struct rbtree_node *RBTreeNode;

static char 		isRed(RBTreeNode node);
static RBTreeNode 	rotateLeft(RBTreeNode node);
static RBTreeNode	rotateRight(RBTreeNode node);
static void 		flipColors(RBTreeNode node);
static RBTreeNode	insert(RBTreeNode node,item_t item);
static item_t 		search(RBTreeNode node,Key key);
static item_t 		min(RBTreeNode node);
static item_t 		max(RBTreeNode node);
static int 			height(RBTreeNode node);
static RBTreeNode 	deleteMin(RBTreeNode node);
static RBTreeNode 	deleteMax(RBTreeNode node);
static RBTreeNode   delete(RBTreeNode,Key key);
static RBTreeNode   moveRedLeft(RBTreeNode node);
static RBTreeNode   moveRedRight(RBTreeNode node);

static RBTreeNode 	balance(RBTreeNode node);

static void 		inorder(RBTreeNode node);
static void 		freeNode(RBTreeNode node);

/**
 * 新建一颗红黑树
 */
T 	rbtree_new()
{
	T tree;
	NEW(tree);
	tree->count = 0;
	tree->head = NULL;

	return tree;
}
void rbtree_free(T tree)
{
	freeNode(tree->head);
	FREE(tree);
}
/**
 * 获取红黑树中节点的个数
 */
int rbtree_count(T tree)
{
	assert(tree);
	return tree->count;
}
/**
 * 判断该红黑树是否为空
 * @param  tree 目标红黑树
 * @return      1 不为空
 *              0 空
 */
int rbtree_isEmpty(T tree)
{
	return tree->count > 0;
}
/**
 * 在红黑树中插入节点
 */
void rbtree_insert(T tree,item_t item)
{
	assert(tree);
	assert(item);

	tree->head = insert(tree->head,item);
	tree->head->color = BLACK;
	tree->count++;
}
item_t rbtree_get(T tree,Key key)
{
	if(tree == NULL)
	{
		return NULL;
	}
	return search(tree->head,key);

}
item_t   rbtree_min(T tree)
{
	assert(tree);
	return min(tree->head);
}
item_t   rbtree_max(T tree)
{
	assert(tree);
	return max(tree->head);
}
void rbtree_deleteMin(T tree)
{
	assert(tree);
	if(tree->count == 0)
	{
		return;
	}

	RBTreeNode root = tree->head;
	if(!isRed(root->left) && !isRed(root->right))
	{
		root->color = 1;
	}
	root = deleteMin(root);
	tree->count--;
	if(tree->count != 0)
	{
		root->color = 0;
	}
}
void rbtree_deleteMax(T tree)
{
	assert(tree);
	if(tree->count == 0)
	{
		return;
	}
	RBTreeNode root = tree->head;
	// printf("%s:%d------------item:%d\n",__FILE__,__LINE__,item_getKey(root->item));
	if(!isRed(root->left) && !isRed(root->right))
	{
		root->color = 1;
	}
	root = deleteMax(root);
	tree->count--;
	if(tree->count != 0)
	{
		root->color = 0;
	}
}

void rbtree_delete(T tree,Key key)
{
	assert(tree);
	if(tree->count == 0)
	{
		return;
	}

	if(rbtree_get(tree,key) == NULL)
	{
		return;
	}

	RBTreeNode root = tree->head;
	if(!isRed(root->left) && !isRed(root->right))
	{
		root->color = RED;
	}
	root = delete(root,key);
	tree->count--;
	if(tree->count != 0)
	{
		root->color = 0;
	}
}
int rbtree_height(T tree)
{
	assert(tree);
	return height(tree->head);
}
/**
 * 前序遍历
 * @param bst 对二叉查找树进行前序遍历(前序遍历就是排序后的结果)
 */
void rbtree_inorder(T tree)
{
	printf("inorder:\n");
	if(tree->count > 0)
	{
		printf("根节点为:%d\n",item_getKey(tree->head->item));
	}

	inorder(tree->head);
	// tree->head->color = 0;
}
/**
 * 前序遍历
 */
void inorder(RBTreeNode node)
{
	if(node == NULL)
	{
		return;
	}
	inorder(node->left);
	printf("%d(%d)   ",item_getValue(node->item),node->color);

	inorder(node->right);
}

static RBTreeNode insert(RBTreeNode node,item_t item)
{
	RBTreeNode newNode;
	if(node == NULL)
	{
		//根节点为空
		// printf("该节点为空，插入新值:%d\n",item_getKey(item));
		NEW(newNode);
		newNode->item = item;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->color = 1;
		return newNode;
	}

	item_t nodeItem = node->item;
	if(item_getKey(nodeItem) == item_getKey(item))
	{
		node->item = item;
	}
	else if(item_getKey(nodeItem) < item_getKey(item))
	{
		// printf("插入到%d 的右节点\n",item_getKey(node->item));
		node->right = insert(node->right,item);
	}
	else if(item_getKey(nodeItem) > item_getKey(item))
	{
		// printf("插入到%d 的左节点\n",item_getKey(node->item));
		node->left = insert(node->left,item);
	}

	if(isRed(node->right) && !isRed(node->left))
	{
			//该节点的右链接为红色 左链接为黑色，需要进行左旋
			node = rotateLeft(node);
	}
	if(isRed(node->left) && isRed(node->left->left))
	{
		//左链接为红色 左链接的孙子节点也为红色 需要右旋
		node = rotateRight(node);
	}
	if(isRed(node->left) && isRed(node->right))
	{
		flipColors(node);
	}

	return node;
}

static item_t search(RBTreeNode node,Key key)
{
	if(node == NULL)
	{
		return NULL;
	}

	item_t item = node->item;
	if(item_getKey(item) == key)
	{
		return item;
	}
	else if(item_getKey(item) < key)
	{
		return search(node->right,key);
	}
	else
	{
		return search(node->left,key);
	}
}
/**
 * 以该节点为根的树种最小值
 * @param  node 节点
 * @return      最小元素
 *
 * 该函数用来查找以node节点为根节点的最小元素
 * 由于红黑树中节点的左子树都要与该节点，右子树都大于该节点
 * 所以以node为根节点的最小元素就是最左边的叶子节点
 * 所以递归遍历左子树，直到为最后一颗左子树即使最小元素
 */
static item_t min(RBTreeNode node)
{
	if(node == NULL)
	{
		return NULL;
	}
	if(node->left == NULL)
	{
		return node->item;
	}
	else
	{
		return min(node->left);
	}
}
/**
 * 以该节点为根的树种最大值
 * @param  node 节点
 * @return      最小元素
 *
 * 原理同min类似
 * 该函数用来查找以node节点为根节点的最小元素
 * 由于红黑树中节点的左子树都要与该节点，右子树都大于该节点
 * 所以以node为根节点的最小元素就是最右边的叶子节点
 * 所以递归遍历右子树，直到为最后一颗右子树即使最小元素
 */
static item_t max(RBTreeNode node)
{
	if(node == NULL)
	{
		return NULL;
	}
	if(node->right == NULL)
	{
		return node->item;
	}
	else
	{
		return max(node->right);
	}
}

/**
 * 以node为根节点的树的深度
 * @param  node 节点
 * @return      以node为根节点的树的深度
 *
 * 原理：
 * 递归遍历左子树和右子树的深度，然后取两者中的大者，然后+1即使以node为根的树的深度
 */
static int height(RBTreeNode node)
{
	if(node == NULL)
	{
		return 0;
	}

	int leftHeight = height(node->left);
	int rightHeight = height(node->right);
	// printf("node(%d) leftHeight = %d rightHeight = %d\n",item_getKey(node->item),leftHeight,rightHeight);
	return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}
static RBTreeNode 	deleteMin(RBTreeNode node)
{
	// printf("%s:%d---------------Key = %d\n",__FILE__,__LINE__,item_getKey(node->item));
	if(node->left == NULL)
	{
		freeNode(node);
		// printf("%s:%d------------min item:%d\n",__FILE__,__LINE__,item_getKey(node->item));
		return NULL;
	}
	if(!isRed(node->left) && !isRed(node->left->left))
	{
		node = moveRedLeft(node);
	}

	node->left = deleteMin(node->left);

	return balance(node);
}

static RBTreeNode 	deleteMax(RBTreeNode node)
{
	// printf("%s:%d------------item:%d(%d)\n",__FILE__,__LINE__,item_getKey(node->item),node->color);
	if(isRed(node->left))
	{
		node = rotateRight(node);
	}

	if(node->right == NULL)
	{
		// printf("%s:%d-----delete-------item:%d\n",__FILE__,__LINE__,item_getKey(node->item));
		freeNode(node);
		return NULL;
	}

	if(!isRed(node->right) && !isRed(node->right->left))
	{
		node = moveRedRight(node);
	}

	node->right = deleteMax(node->right);

	return balance(node);
}

static RBTreeNode   delete(RBTreeNode node,Key key)
{
	item_t nodeItem = node->item;
	if(item_getKey(nodeItem) > key)
	{
		if(!isRed(node->left) && !isRed(node->left->left))
		{
			node = moveRedLeft(node);
		}
		node->left = delete(node->left,key);
	}
	else
	{
		if(isRed(node->left))
		{
			node = rotateRight(node);
		}
		if(item_getKey(nodeItem) == key && node->right == NULL)
		{
			freeNode(node);
			return NULL;
		}
		if(!isRed(node->right) && !isRed(node->right->left))
		{
			node = moveRedLeft(node);
		}
		if(item_getKey(nodeItem) == key)
		{
			node->item = min(node->right);
			node->right = deleteMin(node->right);
		}
		else
		{
			node->right = delete(node->right,key);
		}
	}

	return balance(node);
}
/**
 * 判断指向该节点的连接是否为红连接
 */
static char isRed(RBTreeNode node)
{
	if(node == NULL)
	{
		return 0;
	}
	return node->color;
}
/**
 * 左旋
 * @param  node 围绕着node节点进行旋转(在旋转过程中可以看做是以node为跟的一颗小树)
 * @return      返回旋转后的根节点(旋转之后可以看成是一颗新树，只是元素还是原来的元素，根节点不同了)
 */
static RBTreeNode 	rotateLeft(RBTreeNode node)
{
	// printf("进行一次左旋\n");
	if(node == NULL)
	{
		return NULL;
	}

	RBTreeNode x = node->right;
	node->right = x->left;
	x->left = node;
	x->color = node->color;
	node->color = 1;

	return x;
}
/**
 * 右旋
 * @param  node 围绕着node节点进行旋转(在旋转过程中可以看做是以node为跟的一颗小树)
 * @return      返回旋转后的根节点(旋转之后可以看成是一颗新树，只是元素还是原来的元素，根节点不同了)
 */
static RBTreeNode	rotateRight(RBTreeNode node)
{
	// printf("进行一次右旋\n");
	if(node == NULL)
	{
		return NULL;
	}
	RBTreeNode x = node->left;
	node->left = x->right;
	x->right = node;
	x->color = node->color;
	node->color = 1;

	return x;
}

/**
 * 颜色转换
 * @param node 以node为根转换左右子节点的颜色和node节点的颜色
 *
 * 该函数在node节点左右子节点均为红色的情况下使用。
 * 该操作最重要的性质是它和旋转操作一样是局部变换，不会影响整棵树的黑色平衡性
 */
static void flipColors(RBTreeNode node)
{
	// printf("进行一次颜色调整\n");
	if(node == NULL || node->left == NULL || node->right == NULL)
	{
		return;
	}

	node->color = 1 - node->color;
	node->left->color = 1 - node->left->color;
	node->right->color = 1 - node->right->color;

}
/**
 * 释放该节点并释放该节点的元素
 * @param node [description]
 */
static void freeNode(RBTreeNode node)
{
	if(node == NULL)
	{
		return;
	}
	freeNode(node->left);
	freeNode(node->right);
	item_free(node->item);
	FREE(node);
}
/**
 * 该节点左子节点 和 左子节点的左子节点均为黑色
 * @param  node [description]
 * @return      [description]
 */
static RBTreeNode   moveRedLeft(RBTreeNode node)
{
	// printf("%s:%d---------------Key = %d\n",__FILE__,__LINE__,item_getKey(node->item));
	flipColors(node);
	if(isRed(node->right->left))
	{
		node->right = rotateRight(node->right);
		node = rotateLeft(node);
		flipColors(node);
	}
	//printf("%s:%d---------------Key = %d\n",__FILE__,__LINE__,item_getKey(node->item));
	return node;
}

static RBTreeNode   moveRedRight(RBTreeNode node)
{
	flipColors(node);
	if(isRed(node->left->left))
	{
		node = rotateRight(node);
		flipColors(node);
	}

	return node;
}
static RBTreeNode 	balance(RBTreeNode node)
{
	// printf("%s:%d ----- balance node = %d(%d)\n",__FILE__,__LINE__,item_getKey(node->item),node->color);
	if(isRed(node->right))
	{
		node = rotateLeft(node);
	}

	if(isRed(node->left) && isRed(node->left->left))
	{
		//左链接为红色 左链接的孙子节点也为红色 需要右旋
		node = rotateRight(node);
	}
	if(isRed(node->left) && isRed(node->right))
	{
		flipColors(node);
	}

	return node;
}
