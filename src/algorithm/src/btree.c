
#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "assert.h"
#include "btree.h"

#define T  btree_t

struct T
{
	char data;
	struct T *leftChild;
	struct T *rightChild;
};


/**
 * 新建空的二叉树
 * @return 新建的二叉树
 */
T btree_new()
{
	T btree;
	NEW(btree);

	btree->data = 0;
	btree->leftChild = NULL;
	btree->rightChild = NULL;
	return btree;
}
/**
 * 新建一个带数据的节点
 * @param  ch 新节点的数据
 * @return    返回新的节点
 */
T  btree_new_by_data(char ch)
{
	T btree;
	NEW(btree);

	btree->data = ch;
	btree->leftChild = NULL;
	btree->rightChild = NULL;
	return btree;
}

//为节点插入数据
void 	btree_add_data(T btree,char ch)
{
	assert(btree);
	btree->data = ch;
}
/**
 * 插入左节点
 * @param  btree     目标节点
 * @param  leftChild 左孩子
 * @return           无
 */
void 	btree_insertLeftNode(T btree,T leftChild)
{
	assert(btree);
	assert(leftChild);

	btree->leftChild = leftChild;
}
//插入右节点
/**
 * 插入右节点
 * @param  btree       目标节点
 * @param  rightChild 左孩子
 * @return            无
 */
void 	btree_insertRightNode(T btree,T rightChild)
{
	assert(btree);
	assert(rightChild);

	btree->rightChild = rightChild;
}

//获取二叉树的深度
int 	btree_getDepth(T btree)
{
	int leftDepth;
	int rightDepth;

	if(btree == NULL)
	{
		return 0;
	}

	leftDepth = btree_getDepth(btree->leftChild);
	rightDepth = btree_getDepth(btree->rightChild);

	return (leftDepth > rightDepth) ? leftDepth + 1 : rightDepth + 1 ;
}

/**
 * 先序遍历二叉树
 * @param  btree 目标二叉树
 * @return       无
 */
void 	btree_preOrder(T btree)
{
	if(btree == NULL)
	{
		return;
	}
	printf("%d\t",btree->data);
	btree_preOrder(btree->leftChild);
	btree_preOrder(btree->rightChild);

}
/**
 * 中序遍历二叉树
 * @param  btree 目标二叉树
 * @return       无
 */
void 	btree_inOrder(T btree)
{
	if(btree == NULL)
	{
		return;
	}
	btree_inOrder(btree->leftChild);
	printf("%d\t",btree->data);
	btree_inOrder(btree->rightChild);
}
/**
 * 后续遍历二叉树
 * @param  btree 目标二叉树
 * @return       无
 */
void 	btree_postOrder(T btree)
{
	if(btree == NULL)
	{
		return;
	}
	btree_postOrder(btree->leftChild);
	btree_postOrder(btree->rightChild);
	printf("%d\t",btree->data);
}

/**
 * 获取叶子节点的个数
 * @param  btree 目标二叉树
 * @return       叶子节点个数
 */
int btree_getLeafCount(T btree)
{
	if(btree == NULL)
	{
		return 0;
	}
	if(btree->leftChild == NULL && btree->rightChild == NULL)
	{
		return 1;
	}
	else
	{
		return btree_getLeafCount(btree->leftChild) + btree_getLeafCount(btree->rightChild);
	}
}
/**
 * 获取所有节点个数
 * @param  btree 目标二叉树
 * @return       返回所有节点个数
 */
int 	btree_getAllNode(T btree)
{
	if(btree == NULL)
	{
		return 0;
	}
	return btree_getAllNode(btree->leftChild) + btree_getAllNode(btree->rightChild) + 1;
}

/**
 * 输出叶子节点
 * @param  btree 目标二叉树
 * @return       无
 */
void 	btree_outputLeaf(T btree)
{
	if(btree == NULL)
	{
		return;
	}
	if(btree->leftChild == NULL && btree->rightChild == NULL)
	{
		printf("%d\t",btree->data);
	}
	else
	{
		btree_outputLeaf(btree->leftChild);
		btree_outputLeaf(btree->rightChild);
	}
}
/**
 * 获取节点数据
 * @param  btree 目标节点
 * @return       节点数据
 */
int 	btree_getData(T btree)
{
	assert(btree);
	return btree->data;
}
//查找节点
T 	btree_search(T btree,char ch)
{
	T ret;
	if(btree == NULL)
	{
		return NULL;
	}
	if(btree->data == ch)
	{
		return btree;
	}
	else
	{
		ret = btree_search(btree->leftChild,ch);
		if(ret != NULL)
		{
			return ret;
		}
		else
		{
			btree_search(btree->rightChild,ch);
		}
	}

	return NULL;
}
int 	btree_isNodeExist(T btree,T node)
{
	if(btree== NULL || node == NULL)
	{
		return 0;
	}
	if(btree_search(btree,node->data) == NULL)
	{
		return 0;
	}

	return 1;
}

//删除左子树
void 	btree_deleteLeftNode(T btree)
{
	if(btree == NULL)
	{
		return;
	}
	btree_free(btree->leftChild);
	btree->leftChild = NULL;
}
//删除右子树
void 	btree_deleteRightNode(T btree)
{
	if(btree == NULL)
	{
		return;
	}
	btree_free(btree->rightChild);
	btree->rightChild = NULL;
}

//删除节点
/**
 * 删除节点，包括其所有子孙节点
 * @param  btree 目标二叉树
 * @param  node  要删除的节点
 * @return       1 删除成功
 *               0 删除失败
 */
int 	btree_deleteNode(T btree,T node)
{
	//先搜索该节点是否存在
	if(btree == NULL || node == NULL)
	{
		return 0;
	}

	if(!btree_isNodeExist(btree,node))
	{
		return 0;

	}

	printf("----------节点在二叉树中------------\n");
	btree_free(node);
	return 1;
}

// //复制二叉树
// extern  T 		btree_copy(T btree);
/**
 * 销毁二叉树
 * @param  tree 目标二叉树
 * @return      无
 */
void 	btree_free(T btree)
{
	printf("btree_free ....start\n");
	if(btree == NULL)
	{
		return;
	}
	if(btree->leftChild != NULL)
	{
		printf("btree_free ....left\n");
		btree_free(btree->leftChild);
	}
	if(btree->rightChild != NULL)
	{
		printf("btree_free ....right\n");
		btree_free(btree->rightChild);
	}

	FREE(btree);
	printf("btree_free ....end\n");

}



/**
 * 复制二叉树
 * @param  btree 目标二叉树
 * @return       新的二叉树
 *
 * ps:用递归的方法复制二叉树
 */
// T 		btree_copy(T btree)
// {
// 	T p,lchild,rchild;
// 	assert(btree);
//
// 	lchild = btree_copy(btree->leftChild);
// 	rchild = btree_copy(btree->rightChild);
//
// 	NEW(p);
// 	p->data = btree->data;
// 	p->leftChild = lchild;
// 	p->rightChild = rchild;
//
// 	return p;
// }

//
