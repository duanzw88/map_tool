#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "mem.h"
#include "assert.h"
#include "graph.h"

#define T graph_tt

// /**
//  * 保存node节点的相邻节点信息
//  */
// struct adj
// {
//     int from;
//     int to;
//     int distance;
//     struct adj *next;
// };
//
// struct T
// {
//     int vertexs;    // 顶点个数
//     int edges;      // 边的个数
//     seq_t adj_seq;  // 邻接表
// };

int *marked;
int *edgeTo;

static int  isExist(struct adj *listhead,int to);
static void search_init(graph_tt graph);
static void dfs(graph_tt graph,int s);
/**
 * 新建图
 * @param  vertexHint 默认顶点个数
 * @return            图
 */
T graph_create(int vertexs)
{
    T graph;

    NEW(graph);
    graph->vertexs = vertexs;
    graph->edges = 0;
    graph->adj_seq = seq_new(vertexs);
    //创建好所有顶点的邻接表
    for(int i = 0; i < vertexs; i++)
    {
        struct adj *a;
        NEW(a);
        a->from = i;
        a->to = i;
        a->distance = 0;
        a->next = NULL;

        seq_addend(graph->adj_seq,a);
    }

    return graph;
}
/**
 * 获取顶点个数
 * @param  graph 图
 * @return       顶点个数
 */
int graph_vertex(T graph)
{
    assert(graph);
    return graph->vertexs;
}
/**
 * 获取边的条数
 * @param  graph 图
 * @return       边的条数
 */
int graph_edge(T graph)
{
    assert(graph);
    return graph->edges;
}
/**
 * 在图中v和w点添加一条边
 * @param graph 图
 * @param v     顶点
 * @param w     顶点
 *
 * 每条边都会出现两次
 * 当存在一条连接v与w的边时
 * w会出现在v的链表中
 * v也会出现在w的链表中
 */
void graph_addEdge(T graph,int v,int w)
{
    assert(graph);
    struct adj *adj_v; //顶点v的邻接表
    struct adj *adj_w; //顶点w的邻接表
    int addFlag  = 0;

    adj_v = seq_get(graph->adj_seq,v);
    if(adj_v == NULL)
    {
        printf("%d的邻接表表不存在\n",v);
    }
    else
    {
        struct adj *newAdj;

        NEW(newAdj);
        newAdj->from = v;
        newAdj->to = w;
        newAdj->distance = 1;
        newAdj->next = adj_v;
        if(!isExist(adj_v,w))
        {
            seq_put(graph->adj_seq,v,newAdj);
            addFlag = 1;
            graph->edges++;
        }

        // printf("%d的邻接表已经存在\n",v);
    }

    adj_w = seq_get(graph->adj_seq,w);
    if(adj_w == NULL)
    {
        printf("%d的邻接表表不存在\n",w);
    }
    else
    {
        struct adj *newAdj;

        NEW(newAdj);
        newAdj->from = w;
        newAdj->to = v;
        newAdj->distance = 1;
        newAdj->next = adj_w;
        if(!isExist(adj_w,v))
        {
            seq_put(graph->adj_seq,w,newAdj);
            if(addFlag == 0)
            {
                graph->edges++;
            }
        }

    }

}

void graph_printAdj(T graph)
{
    int i = 0;
    for(i = 0;i < seq_length(graph->adj_seq); i++)
    {
        // printf("%d->",);
        struct adj *a = seq_get(graph->adj_seq,i);
        printf("%d->",a->from);
        while(a->next != NULL)
        {
            printf("%d->",a->to);
            a = a->next;
        }
        printf("NULL\n");
    }
}

static int isExist(struct adj *listhead,int to)
{
    while(listhead != NULL)
    {
        if(listhead->to == to)
        {
            return 1;
        }
        listhead = listhead->next;
    }

    return 0;
}

void search_init(graph_tt graph)
{
    assert(graph);

    marked = ALLOC(sizeof(int) * graph->vertexs);
    edgeTo = ALLOC(sizeof(int) * graph->vertexs);
    memset(edgeTo,-1,sizeof(int) * graph->vertexs);
}
/**
 * 深度优先搜索
 * @param graph [description]
 * @param s     [description]
 */
void graph_dfs(graph_tt graph,int s)
{
    if(graph == NULL)
    {
        return;
    }
    search_init(graph);
    dfs(graph,s);
}
/**
 * 广度优先搜索
 */
void graph_bfs(graph_tt graph,int s)
{
    if(graph == NULL)
    {
        return;
    }
    search_init(graph);

    seq_t queue = seq_new(10);
    marked[s] = 1;
    seq_addstart(queue,(void *)s);

    while(seq_length(queue) != 0)
    {
        for(int i = 0;i < seq_length(queue);i++)
        {
            int point = seq_get(queue,i);
            printf("%d ",point);
        }
        printf("\n");

        int v = seq_removestart(queue);
        printf("remove %d\n",v);
        struct adj *list = seq_get(graph->adj_seq,v);

        while(list)
        {
            printf("(%d,%d)\n",list->from,list->to);
            if(marked[list->to] == 0)
            {
                edgeTo[list->to] = v;
                marked[list->to] = 1;
                seq_addend(queue,(void *)list->to);
            }
            list = list->next;
        }
        printf("--------\n");

    }
    seq_free(&queue);
}

seq_t graph_pathTo(graph_tt graph,int start,int end)
{
    assert(graph);
    // dfs(graph,start);
    if(marked[end] == 0)
    {
        return NULL;
    }
    seq_t pathSeq = seq_new(10);
    for(int i = end; i != start; i = edgeTo[i])
    {
        seq_addstart(pathSeq,(void *)i);
    }
    seq_addstart(pathSeq,(void *)start);
    return pathSeq;
}
static void dfs(graph_tt graph,int s)
{
    // printf("\n\n\n---------------------------\n");
    // for(int i = 0;i < graph->vertexs;i++)
    // {
    //     printf("edge[%d] = %d\n",i,edgeTo[i]);
    // }
    // printf("\n---------------------------\n");

    marked[s] = 1;
    struct adj *list = seq_get(graph->adj_seq,s);
    while(list)
    {
        // printf("list(%d,%d)\n",list->from,list->to);
        if(marked[list->to] == 0)
        {
            edgeTo[list->to] = s;
            dfs(graph,list->to);
        }
        list = list->next;
    }
}
