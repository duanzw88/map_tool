#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "assert.h"
#include "mem.h"
#include "indexminipq.h"
#include "wdigraph.h"

#define T wdigraph_t

static void wdigraph_dijkstra(wdigraph_t wdigraph,int v,double *distTo,struct adj **edgeTo,iminipq_t pq);
static void wdigraph_relax(struct adj *edge,double *distTo,struct adj **edgeTo,iminipq_t pq);
/**
 * 新建加权有向图
 * @param  vertexs 顶点个数
 * @return         新建的加权有向图
 */
T wdigraph_new(int vertexs)
{
    int i;
    assert(vertexs >= 0);
    T wdigraph;

    NEW(wdigraph);
    wdigraph->vertexs = vertexs;
    wdigraph->edges = 0;
    wdigraph->marked = ALLOC(sizeof(int) * vertexs);
    wdigraph->adj_seq = seq_new(vertexs);

    //每个顶点和自己都会组成自环
    for(i = 0;i  < vertexs; i++)
    {
        wdigraph->marked[i] = 0;

        struct adj *a;
        NEW(a);
        a->from = i;
        a->to = i;
        a->weight = 0.0;
        a->next = NULL;

        seq_addend(wdigraph->adj_seq,a);
    }
    return wdigraph;
}
/**
 * 释放加权有向图资源
 * @param  wdigraph 加权有向图
 * @return          无
 */
void wdigraph_free(T wdigraph)
{
    assert(wdigraph);
    FREE(wdigraph->marked);
    //先释放邻接表
    for(int i = 0;i < seq_length(wdigraph->adj_seq);i++)
    {
        struct adj *a = seq_get(wdigraph->adj_seq,i);
        while(a != NULL)
        {
            struct adj *b = a;
            a = a->next;
            FREE(b);
        }
    }

    seq_free(&(wdigraph->adj_seq));
    FREE(wdigraph);
}

/**
 * 获取加权有向图的顶点个数
 * @param  wdigraph 加权有向图
 * @return          图的顶点个数
 */
int wdigraph_vertexs(T wdigraph)
{
    assert(wdigraph);
    return wdigraph->vertexs;
}

/**
 * 获取加权有向图边的条数
 * @param  wdigraph 加权有向图
 * @return          图的边的条数
 */
int wdigraph_edges(T wdigraph)
{
    assert(wdigraph);
    return wdigraph->edges;
}

/**
 * 添加一条边
 * @param  wdigraph 加权有向图
 * @param  v        边的起点
 * @param  w        边的终点
 * @param  weight   边的权重
 * @return          无
 */
void wdigraph_add_edge(T wdigraph,int v,int w,double weight)
{
    assert(wdigraph);
    assert(v >= 0 && v < wdigraph->vertexs);
    assert(w >= 0 && w < wdigraph->vertexs);
    assert(weight >= 0.0);

    //获取v的邻接表
    struct adj *list = seq_get(wdigraph->adj_seq,v);
    struct adj *head;
    head = list;
    //先检查w顶点是否已经存在
    while(list != NULL && list->next != NULL)
    {
        if(list->to == w)
        {
            //如果节点已经存在,只需要更新该条边的权重即可
            list->weight = weight;
            return;
        }
        list = list->next;
    }

    //执行到此处说明w不存在list中
    //新建一个节点
    struct adj *newAdj;
    newAdj = ALLOC(sizeof(struct adj));
    newAdj->from = list->from;
    newAdj->to = w;
    newAdj->weight = weight;
    newAdj->next = head;
    seq_put(wdigraph->adj_seq,v,newAdj);
    wdigraph->edges++;

}

/**
 * 在加权有向图中获取顶点v的邻接表
 * @param  wdigraph 加权有向图
 * @param  v        顶点
 * @return          顶点v的邻接表(链表组成)
 */
adj_t wdigraph_get_adj(T wdigraph,int v)
{
    assert(wdigraph);
    assert(v >= 0 && v < wdigraph->vertexs);

    return seq_get(wdigraph->adj_seq,v);
}

/**
 * 判断从顶点v到顶点w是否存在路径
 * @param  wdigraph 图
 * @param  v        起点
 * @param  w        终点
 * @return          1 存在路径
 *                  0 不可达
 */
int wdigraph_has_path(T wdigraph,int v,int w)
{
    assert(wdigraph);
    assert(v >= 0 && v < wdigraph->vertexs);
    assert(w >= 0 && w < wdigraph->vertexs);
    double *distTo = ALLOC(sizeof(double) * wdigraph->vertexs);
    struct adj **edgeTo = ALLOC(sizeof(struct adj *) * wdigraph->vertexs);
    iminipq_t pq = iminipq_new(wdigraph->vertexs);

    wdigraph_dijkstra(wdigraph,v,distTo,edgeTo,pq);
    iminipq_free(&pq);
    FREE(edgeTo);

    double dist = distTo[w];
    FREE(distTo);
    return dist < DBL_MAX;
}
/**
 * 从顶点v到顶点w的路径
 * @param  wdigraph 图
 * @param  v        起点
 * @param  w        终点
 * @return          adj_t 路径的链表
 *                  NULL  不存在路径
 */
seq_t wdigraph_path(T wdigraph,int v,int w)
{
    assert(wdigraph);
    assert(v >= 0 && v < wdigraph->vertexs);
    assert(w >= 0 && w < wdigraph->vertexs);
    double *distTo = ALLOC(sizeof(double) * wdigraph->vertexs);
    struct adj **edgeTo = ALLOC(sizeof(struct adj *) * wdigraph->vertexs);
    iminipq_t pq = iminipq_new(wdigraph->vertexs);

    wdigraph_dijkstra(wdigraph,v,distTo,edgeTo,pq);
    seq_t seq = seq_new(wdigraph->vertexs);
    struct adj *p;
    for(p = edgeTo[w]; p != NULL; p = edgeTo[p->from])
    {
        seq_addstart(seq,p->to);
    }
    seq_addstart(seq,v);
    // printf("\n");

    FREE(distTo);
    FREE(edgeTo);
    iminipq_free(&pq);
    return seq;
}
/**
 * 从顶点v到顶点w的路径权重之和
 * @param  wdigraph 图
 * @param  v        起点
 * @param  w        终点
 * @return          起点到终点的权重之和
 */
double wdigraph_dist(T wdigraph,int v,int w)
{
    assert(wdigraph);
    assert(v >= 0 && v < wdigraph->vertexs);
    assert(w >= 0 && w < wdigraph->vertexs);
    double *distTo = ALLOC(sizeof(double) * wdigraph->vertexs);
    struct adj **edgeTo = ALLOC(sizeof(struct adj *) * wdigraph->vertexs);
    iminipq_t pq = iminipq_new(wdigraph->vertexs);

    wdigraph_dijkstra(wdigraph,v,distTo,edgeTo,pq);
    double dist =  distTo[w];

    iminipq_free(&pq);
    FREE(edgeTo);
    FREE(distTo);

    return dist;
}

static void wdigraph_relax(struct adj *edge,double *distTo,struct adj **edgeTo,iminipq_t pq)
{
        int v = edge->from;
        int w = edge->to;
        if(distTo[w] > distTo[v] + edge->weight)
        {
            distTo[w] = distTo[v] + edge->weight;
            edgeTo[w] = edge;
            // printf("update w = %d v = %d weight = %.2lf\n",w,v,edge->weight);
            if(iminipq_contains(pq,w))
            {
                iminipq_change_key(pq,w,distTo[w]);
            }
            else
            {
                iminipq_insert(pq,w,distTo[w]);
            }
        }
}
static void wdigraph_dijkstra(wdigraph_t wdigraph,int v,double *distTo,struct adj **edgeTo,iminipq_t pq)
{
    int i;
    assert(wdigraph);
    for(i = 0; i < wdigraph->vertexs; i++)
    {
        distTo[i] = DBL_MAX;
    }

    distTo[v] = 0.0;
    iminipq_insert(pq,v,0.0);
    while(!iminipq_is_empty(pq))
    {
        int v = iminipq_delete_min(pq);
        struct adj *list = seq_get(wdigraph->adj_seq,v);
        while(list != NULL && list->next != NULL)
        {
            // printf("relax %d\n",list->to);
            wdigraph_relax(list,distTo,edgeTo,pq);
            list = list->next;
        }
    }
}

void wdigraph_print_adj(T wdigraph)
{
    for(int i = 0;i < seq_length(wdigraph->adj_seq);i++)
    {
        struct adj *a = seq_get(wdigraph->adj_seq,i);
        // printf("%d->",a->from);
        while(a != NULL && a->next != NULL)
        {
            //由于最后一个节点是存的自己到自己的自环所以不需要遍历
            printf("%d->%d   %.2f\n",a->from,a->to,a->weight);
            a = a->next;
        }
    }

}
