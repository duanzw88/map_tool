#ifndef _SET_H
#define _SET_H

/**
* @file         set.h
* @brief        集合是不同成员的“无序”汇集。对集合的操作包括检验成员资格、添加成员、删除成员。其他操作包括集合的并、交、差和对称差
* @details
* @author       duanzw
* @date     	2016-3-2
* @version  	0.0.1
*/

#define T set_t
typedef struct T *T;

/**
 * 分配、初始化一个新的T实例
 * @param  hint  对集合预期会包含的成员数目的一个估计，准确的hint值会提高性能，任何非负值都是可接受的
 * @param  cmp	 用来比较两个成员
 * @param  hash	 用来将成员映射到无符号整数
 * @retval  新的T实例
 *
 * @other cmp(x,y)针对x小于y x等于y x大于y的情形必须分别返回小于0 等于0 大于0的整数
 *        如果cmp(x,y)返回0，那么x和y只有一个会出现在集合中，而且hash(x)必定等于hash(y)
 *        如果cmp为NULL函数指针，那么假定集合成员为原子，采用默认比较函数
 *        如果hash为NULL函数指针，采用默认hash函数
 */
extern T set_new(int hint,
				 int cmp(const void *x,const void *y),
			 	 unsigned hash(const void *x));

extern void set_free(T *set);

/**
 * 获取集合的势
 * @param  set  集合对象
 * @retval  集合的势 或其所包含的成员数目
 */
extern int 	set_length(T set);
/**
 * 查询member是否在集合中
 * @param  set    集合对象
 * @param  member 要查询的成员
 * @return  1	  member在set中
 *          0	  member不在set中
 */
extern int 	set_member(T set,const void *member);

/**
 * 向set中添加成员
 * @param set    集合对象
 * @param member 要添加的成员
 */
extern void set_put(T set,const void *member);

/**
 * 删除成员
 * @param  set    集合对象
 * @param  member 要删除的成员
 * @return  member在set中    删除的成员
 *          member不在set中      NULL
 */
extern void *set_remove(T set,const void *member);

/**
 * 对集合每个成员都调用apply函数
 * @param set 集合对象
 * @param cl  [description]
 */
extern void set_map(T set,
					void apply(const void *member,void *cl),void *cl);

/**
 * 转换为数组
 * @param  set 集合对象
 * @param  end 赋值给数组的第N+1个元素
 * @return     指向N+1个元素的数组
 */
extern void **set_toArray(T set,void *end);

/**
 * 并集
 * @param  s 集合对象
 * @param  t 集合对象
 * @return  s+t
 */
extern T set_union(T s,T t);
/**
 * 交集
 * @param  s 集合对象
 * @param  t 集合对象
 * @return   s*t
 */
extern T set_inter(T s,T t);
/**
 * 差集
 * @param  s 集合对象
 * @param  t 集合对象
 * @return   s-t
 */
extern T set_minus(T s,T t);
/**
 * 对称差
 * @param  s 集合对象
 * @param  t 集合对象
 * @return   s/t
 */
extern T set_diff(T s,T t);

#undef T
#endif
