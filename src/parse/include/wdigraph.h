/**
 * 加权有向图
 * 在图中保存邻接表的时候，使用链表保存每个顶点的邻接表
 * 也可以使用数组表示
 *
 * 注意事项:
 * 顶点从0开始计算 是编号连续的顶点
 */
#ifndef _WDIGRAPH_H
#define _WDIGRAPH_H

#include "../../algorithm/include/seq.h"

#define T wdigraph_t
typedef struct T *T;

struct adj
{
    int     from;
    int     to;
    double  weight;
    struct  adj     *next;
};

struct T
{
    int     vertexs;    //顶点个数
    int     edges;      //边的个数
    int     *marked;    //每个顶点是否被遍历表示 1 遍历过 0 为遍历过
    seq_t   adj_seq;    //邻接表队列，存储每个顶点的邻接表，所以队列的长度<= 顶点个数
};
typedef struct adj *adj_t;

/**
 * 新建加权有向图
 * @param  vertexs 顶点个数
 * @return         新建的加权有向图
 */
extern T       wdigraph_new(int vertexs);
/**
 * 释放加权有向图资源
 * @param  wdigraph 加权有向图
 * @return          无
 */
extern void     wdigraph_free(T wdigraph);

/**
 * 获取加权有向图的顶点个数
 * @param  wdigraph 加权有向图
 * @return          图的顶点个数
 */
extern int      wdigraph_vertexs(T wdigraph);

/**
 * 获取加权有向图边的条数
 * @param  wdigraph 加权有向图
 * @return          图的边的条数
 */
extern int      wdigraph_edges(T wdigraph);

/**
 * 添加一条边
 * @param  wdigraph 加权有向图
 * @param  v        边的起点
 * @param  w        边的终点
 * @param  weight   边的权重
 * @return          无
 */
extern void     wdigraph_add_edge(T wdigraph,int v,int w,double weight);

/**
 * 在加权有向图中获取顶点v的邻接表
 * @param  wdigraph 加权有向图
 * @param  v        顶点
 * @return          顶点v的邻接表(链表组成)
 */
extern adj_t     wdigraph_get_adj(T wdigraph,int v);
extern void      wdigraph_print_adj(T wdigraph);
/**
 * 加权有向图中顶点v的出度
 * @param  wdigraph 加权有向图
 * @param  v        顶点
 * @return          v的出度
 */
//extern int      wdigraph_out_degress(T wdigraph,int v);

/**
 * 加权有向图中顶点v的入度
 * @param  wdigraph 加权有向图
 * @param  v        顶点
 * @return          v的入度
 */
//extern int      wdigraph_in_degress(T wdigraph,int v);

/**
 * 判断从顶点v到顶点w是否存在路径
 * @param  wdigraph 图
 * @param  v        起点
 * @param  w        终点
 * @return          1 存在路径
 *                  0 不可达
 */
extern int wdigraph_has_path(T wdigraph,int v,int w);
/**
 * 从顶点v到顶点w的路径
 * @param  wdigraph 图
 * @param  v        起点
 * @param  w        终点
 * @return          adj_t 路径的链表
 *                  NULL  不存在路径
 */
extern seq_t wdigraph_path(T wdigraph,int v,int w);
/**
 * 从顶点v到顶点w的路径权重之和
 * @param  wdigraph 图
 * @param  v        起点
 * @param  w        终点
 * @return          起点到终点的权重之和
 *                  DBL_MAX 不存在路径(相当于无穷大)
 */
extern double wdigraph_dist(T wdigraph,int v,int w);
#undef T
#endif
