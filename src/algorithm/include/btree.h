/**
 * 二叉树的操作
 */
#ifndef _BTREE_H
#define _BTREE_H

#define T  btree_t
typedef struct T *T;

/**
 * 新建空的二叉树
 * @return 新建的二叉树
 */
extern 	T 		btree_new();
/**
 * 新建一个带数据的节点
 * @param  ch 新节点的数据
 * @return    返回新的节点
 */
extern  T 		btree_new_by_data(char ch);

//为节点插入数据
extern  void 	btree_add_data(T btree,char ch);
//插入左节点
extern	void 	btree_insertLeftNode(T btree,T leftChild);
//插入右节点
extern	void 	btree_insertRightNode(T parent,T rightChild);

//获取二叉树的深度
extern	int  	btree_getDepth(T btree);

//先序遍历二叉树
extern	void 	btree_preOrder(T btree);
//中序遍历二叉树
extern  void 	btree_inOrder(T btree);
//后续遍历二叉树
extern  void 	btree_postOrder(T btree);

//获取叶子节点的个数
extern 	int 	btree_getLeafCount(T btree);
//获取所有节点个数
extern 	int 	btree_getAllNode(T btree);
//输出叶子节点
extern  void 	btree_outputLeaf(T btree);
extern  int 	btree_getData(T btree);
//查找节点
extern	T 		btree_search(T btree,char ch);
//判断节点在不在二叉树中
extern  int 	btree_isNodeExist(T btree,T node);
//删除左子树
extern 	void 	btree_deleteLeftNode(T btree);
//删除右子树
extern	void 	btree_deleteRightNode(T btree);


//删除节点
extern  int 	btree_deleteNode(T btree,T node);
// extern  int 	btree_deleteNode_by_data(T btree,char ch);

// //复制二叉树
// extern  T 		btree_copy(T btree);
// 销毁二叉树
extern  void 	btree_free(T tree);



#undef T
#endif
