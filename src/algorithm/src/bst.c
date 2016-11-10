/**
 * 二叉查找树
 * 二叉查找树是继承了链表插入的灵活性和有序数组查找的高效性
 * 二叉查找树的实现：
 * 一颗二叉查找树，其中每个节点都含有一个键和一个值，
 * 				且每个节点的键都大于其左子树种的任意节点
 * 				而小于右子树的任意节点的键
 * 难点：
 * 插入：
 * 插入是利用递归的方式实现的，
 * 如果树是空的，就插入到二叉查找树的根节点
 * 如果被插入的节点的键小于根节点的键，会递归在左子树中插入该键
 * 如果被插入的节点的键大于根节点的键,会递归在右子树中插入该键
 * 递归的实现：
 *   把新节点想象成沿着树往下走，它会新节点的键和每个节点的键值相比较，
 *   然后根据比较结果决定向左或者向右移动到下一个节点
 * date:2016-6-21
 */

#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "mem.h"

#include "bst.h"

#define T bst_t
struct T
{
	int count;
	struct bst_node
	{
		item_t item;
		struct bst_node *leftChild;
		struct bst_node *rightChild;
	}*head;
};
typedef struct bst_node* BstNode;
static void 	freeNode(BstNode node);
static BstNode	insert(BstNode node,item_t item);
static item_t 	search(BstNode node,Key key);
static BstNode 	minNode(BstNode node);
static BstNode 	maxNode(BstNode node);
static int 	  	heightNode(BstNode node);
static BstNode	delete(BstNode node,Key key);
static void 	inorder(BstNode node);

/**
 * 新建一颗二叉查找树
 * @return 新建的二叉查找树，不含任何元素
 */
T 	bst_new()
{
	T bst;
	NEW(bst);
	bst->count = 0;
	bst->head = 0;
	return bst;
}
/**
 * 释放二叉查找树空间
 * @param bst 由bst_new创建的二叉树
 */
void bst_free(T bst)
{
	freeNode(bst->head);
	FREE(bst);
}
/**
 * 判断二叉查找树是否为空
 * @param  bst 目标二叉查找树
 * @return     1 不为空
 *             0 空
 */
int bst_isEmpty(T bst)
{
	assert(bst);

	return bst->count >= 1;
}
/**
 * 获取二叉树元素的个数
 * @param  bst 目标二叉查找树
 * @return     目标二叉查找树的元素个数
 */
int bst_count(T bst)
{
	assert(bst);
	return bst->count;
}
/**
 * 在二叉查找树中插入元素
 * @param bst  目标二叉查找树
 * @param item 要插入的元素(如果已经存在该元素的Key，则只进行更新)
 */
void bst_insert(T bst,item_t item)
{
	assert(bst);
	assert(item);

	// if(bst->head == NULL)
	// {
	// 	BstNode node;
	// 	NEW(node);
	// 	node->item = item;
	// 	node->leftChild = NULL;
	// 	node->rightChild = NULL;
	//
	// 	bst->head = node;
	// 	bst->count++;
	// }
	// else
	// {
	// 	insert(bst->head,item);
	// 	bst->count++;
	// }
	bst->head = insert(bst->head,item);
	bst->count++;

}
/**
 * 二叉查找树的查找某个Key对应的元素
 * @param  bst 目标二叉查找树
 * @param  key 要查找元素的key值
 * @return     如果查找到 返回元素
 *             如果查找不到 返回NULL
 */
item_t bst_get(T bst,Key key)
{
	if(bst == NULL)
	{
		return NULL;
	}
	return search(bst->head,key);
}
/**
 * 获取二叉查找树中的最小值
 * @return  最小元素
 */
item_t bst_min(T bst)
{
	assert(bst);
	BstNode min = minNode(bst->head);
	return min->item;
}
/**
 * 获取二叉查找树中最大元素
 * @return 最大元素
 */
item_t bst_max(T bst)
{
	assert(bst);
	BstNode max = maxNode(bst->head);
	return max->item;
}
/**
 * 二叉查找树的高度
 * @return 树的高度
 */
int bst_height(T bst)
{
	assert(bst);
	return heightNode(bst->head);
}
/**
 * 删除二叉查找树的最小元素
 */
void bst_deleteMin(T bst)
{
	assert(bst);
	if(bst->count == 0)
	{
		return;
	}
	BstNode min = minNode(bst->head);
	bst->head = delete(bst->head,item_getKey(min->item));
	bst->count--;
}
/**
 * 删除最大元素
 */
void bst_deleteMax(T bst)
{
	assert(bst);
	if(bst->count == 0)
	{
		return;
	}

	BstNode max = maxNode(bst->head);
	bst->head = delete(bst->head,item_getKey(max->item));
	bst->count--;
}

/**
 * 删除元素
 */
void bst_delete(T bst,Key key)
{
	assert(bst);
	if(bst->count == 0)
	{
		return;
	}
	item_t item = search(bst->head,key);
	if(item == NULL)
	{
		return;
	}
	delete(bst->head,key);
	bst->count--;
}

/**
 * 二叉查找树转换成数组
 */
void   bst_toArray(T bst,item_t *array)
{

}
/**
 * 前序遍历
 * @param bst 对二叉查找树进行前序遍历(前序遍历就是排序后的结果)
 */
void bst_inorder(T bst)
{
	printf("inorder:\n");
	inorder(bst->head);
}
//----------------------------------------------------------------
/**
 * 释放节点
 * @param node 目标节点
 *
 * 释放节点包括释放其后代节点和该节点的元素
 */
static void freeNode(BstNode node)
{
	if(node == NULL)
	{
		return;
	}
	// printf("free node key = %d\n",item_getKey(node->item));
	freeNode(node->leftChild);
	freeNode(node->rightChild);
	item_free(node->item);
	FREE(node);
}

/**
 * 以node为根节点查找key值对应的元素
 * @param  node 节点
 * @param  key  要查找元素的key值
 * @return      NULL或者查找到的元素
 */
static item_t search(BstNode node,Key key)
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
		return search(node->rightChild,key);
	}
	else
	{
		return search(node->leftChild,key);
	}
}
/**
 * 以node为根节点的二叉查找树的最小元素
 */
static BstNode minNode(BstNode node)
{
	if(node == NULL)
	{
		return NULL;
	}
	if(node->leftChild == NULL)
	{
		return node;
	}
	return minNode(node->leftChild);
}
static BstNode maxNode(BstNode node)
{
	if(node == NULL)
	{
		return NULL;
	}
	if(node->rightChild == NULL)
	{
		return node;
	}
	return maxNode(node->rightChild);
}
static int 	  heightNode(BstNode node)
{
	if(node == NULL)
	{
		return 0;
	}
	int leftHeight = heightNode(node->leftChild);
	int rightHeight = heightNode(node->rightChild);
	return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

/**
 * 删除以node为根的二叉树的节点
 * 分三种情况：
 * 1.被删除的节点没有子树，直接删除，并修改对应父节点的指针为空
 * 2.被删除的节点只有一个子树，用其子树替代该节点即可
 * 3.两个子树均存在，用被删除的节点的左子树的最右节点或者用被删除节点的右子树的最左节点来替代即可
 */
static BstNode delete(BstNode node,Key key)
{
	BstNode retNode;
	item_t nodeItem = node->item;
	// printf("nodeKey  = %d key = %d\n",item_getKey(nodeItem),key);
	int cmp = item_getKey(nodeItem) - key;
	if(cmp == 0)
	{
		//node节点就是要删除的点
		if(node->leftChild == NULL && node->rightChild == NULL)
		{
			// printf("不存在孩子节点\n");
			//不存在子树
			freeNode(node);

			retNode = NULL;
			// return NULL;
		}
		else if(node->leftChild != NULL && node->rightChild == NULL)
		{
			// printf("不存在孩子节点\n");
			retNode = node->leftChild;
			//存在左子树，不存在右子树
			node->leftChild = NULL;
			freeNode(node);
			// return retNode;
		}
		else if(node->rightChild != NULL && node->leftChild == NULL)
		{
			//存在右子树 不存在左子树
			retNode = node->rightChild;
			node->rightChild = NULL;
			freeNode(node);
			// return retNode;
		}
		else if(node->rightChild != NULL && node->leftChild != NULL)
		{
			BstNode min = minNode(node->rightChild);
			node->item = retNode->item;
			retNode = node;
			freeNode(min);
			min = NULL;
			// return node;
		}
	}
	else if(cmp < 0)
	{
		node->rightChild =  delete(node->rightChild,key);
		retNode = node;
		// return node;
	}
	else if(cmp > 0)
	{
		node->leftChild =  delete(node->leftChild,key);
		retNode = node;
		// return node;
	}
	return retNode;
}
static void inorder(BstNode node)
{
	if(node == NULL)
	{
		return;
	}
	item_t item = node->item;

	inorder(node->leftChild);
	printf("%d  ",item_getValue(item));
	inorder(node->rightChild);
}


/**
 * 在以node为根节点的二叉查找树中插入元素
 * @param node 根节点
 * @param item 要插入的元素
 *
 * 该函数是以node为根节点的局部二叉查找树
 * 如果item的Key值已经存在，则进行更新，
 * 如果二叉查找树中不存在，插入
 */
static BstNode insert(BstNode node,item_t item)
{
	BstNode newNode;

	if(node == NULL)
	{
		NEW(newNode);
		newNode->item = item;
		newNode->leftChild = NULL;
		newNode->rightChild = NULL;

		return newNode;
	}

	item_t nodeItem = node->item;
	int cmp = item_getKey(nodeItem) - item_getKey(item);
	if(cmp == 0)
	{
		node->item = item;
	}
	else if(cmp < 0)
	{
		//插入到右节点
		node->rightChild = insert(node->rightChild,item);

	}
	else if(cmp > 0)
	{
		node->leftChild = insert(node->leftChild,item);
	}

	return node;
}
