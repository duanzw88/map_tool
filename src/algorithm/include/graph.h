#ifndef _GRAPH_H
#define _GRAPH_H

#include "seq.h"

#define T graph_tt
typedef struct T *T;

/**
 * 保存node节点的相邻节点信息
 */
struct adj
{
    int from;
    int to;
    int distance;
    struct adj *next;
};

struct T
{
    int vertexs;    // 顶点个数
    int edges;      // 边的个数
    seq_t adj_seq;  // 邻接表
};

//新建图
extern T    graph_create(int vertexSize);
//顶点个数
extern int graph_vertex(T graph);
//边的条数
extern int graph_edge(T graph);
//添加一条边
extern void graph_addEdge(T graph,int v,int w);

extern void graph_printAdj(T graph);

/**
 * 深度优先搜索
 * @param graph [description]
 */
// extern void dfs_init(graph_tt graph);
extern void graph_dfs(graph_tt graph,int s);
extern void graph_bfs(graph_tt graph,int s);
extern seq_t graph_pathTo(graph_tt graph,int start,int end);

#endif
