
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "assert.h"
#include "seq.h"
#include "mem.h"
#include "wdigraph.h"
#include "routenode.h"
#include "vector2d.h"
#include "sparsearray.h"
#include "route.h"
#include "building.h"

#define T building_t
struct T
{
    char *name;
    seq_t floor_seq;
    wdigraph_t graph_root;
    wdigraph_t graph_car;
    seq_t route_list;
    sparsearray_t routenode_array;
};
// struct route_node_t
// {
//     int index;
//     double x;
//     double y;
// };

static double get_distance(double x1,double y1,double x2,double y2);
/**
 * [add_route description]
 * @param  building [description]
 * @param  nodes    坐标点列表
 * @param  flag     0 单向路 1 双向路
 */
void   add_route(T building,floor_t floor,seq_t nodes,int flag);
void   add_graph(T building);
routenode_t get_routenode_by_pointkey(T building,int point_key);
/**
 * 新建建筑物
 * @param  name 建筑物名称
 * @return      建筑物对象
 */
T building_new(const char *name)
{
    T build;
    NEW(build);
    build->name = CALLOC(1,strlen(name) + 1);
    build->floor_seq = seq_new(10);
    build->route_list = seq_new(20);
    build->routenode_array = sparsearray_new(1024);
    build->graph_root = NULL;//wdigraph_new(1024);
    build->graph_car = NULL;//wdigraph_new(1024);

    strncpy(build->name,name,strlen(name));
    return build;
}

/**
 * 释放建筑物对象
 * @param build 建筑物
 */
void building_free(T *build)
{

}

/**
 * 获取建筑物名称
 * @param  build 建筑物
 * @return       建筑物名称
 */
const char *building_get_name(T build)
{
    assert(build);
    return build->name;
}
/**
 * 在建筑物中添加楼层
 * @param  build 建筑物
 * @param  floor 楼层
 */
void building_add_floor(T build,floor_t floor)
{
    assert(build);
    assert(floor);
    floor_set_index(floor,seq_length(build->floor_seq));
    seq_addend(build->floor_seq,floor);
}
/**
 * 根据索引获取楼层
 * @param  build 建筑物
 * @param  index 索引值
 * @return       NULL 索引值不存在
 *               floor 楼层
 */
floor_t building_get_floor_by_index(T build,int index)
{
    assert(build);
    assert(index >= 0);
    if(index >= seq_length(build->floor_seq))
    {
        return NULL;
    }

    return seq_get(build->floor_seq,index);
}
/**
 * 根据索引获取楼层
 * @param  build 建筑物
 * @param  name  索引值
 * @return       NULL 索引值不存在
 *               floor 楼层
 */
floor_t building_get_floor_by_name(T build,const char *name)
{
    assert(build);
    assert(name);
    for(int i = 0;i < seq_length(build->floor_seq);i++)
    {
        floor_t f = seq_get(build->floor_seq,i);
        if(strcmp(name,floor_get_name(f)) == 0)
        {
            return f;
        }
    }

    return NULL;
}

/**
 * 获取楼层数
 * @param  build 建筑物
 * @return       楼层总数
 */
int building_get_floor_count(T build)
{
    assert(build);
    return seq_length(build->floor_seq);
}

void building_add_route(T building)
{
    int i,j;
    floor_t floor;
    int node_index = 0;
    seq_t oneway_points;
    seq_t twoway_points;
    route_t route;
    int route_count = 0;

    for(i = 0; i < seq_length(building->floor_seq); i++)
    {
        floor = seq_get(building->floor_seq,i);
        route_count += floor_get_route_count(floor);
        //单向路
        oneway_points = floor_add_oneway_road(floor);
        add_route(building,floor,oneway_points,0);

        twoway_points = floor_add_twoway_road(floor);
        add_route(building,floor,twoway_points,1);
    }
    add_graph(building);

    for(i = 0; i < seq_length(building->route_list); i++)
    {
        route = seq_get(building->route_list,i);
        routenode_t sn = route_get_start_node(route);
        routenode_t en = route_get_end_node(route);
        route_style style = route_get_style(route);
        printf("num = %d weight = %.2f   ",route_get_num(route),route_get_weight(route));
        printf("%d:(%.2f,%.2f)",routenode_get_key(sn),routenode_get_x(sn),routenode_get_y(sn));
        printf("%s",style == ONEWAY ? "---->" : "<---->");
        printf("%d:(%.2f,%.2f)\n",routenode_get_key(en),routenode_get_x(en),routenode_get_y(en));
    }
}

int building_get_near_road_point(T building,double sx,double sy,double *px,double *py)
{
    routenode_t start_node;
    routenode_t end_node;
    int i,j,road_num = -1;

    double min_dist = 2000000;
    assert(building);
    assert(building->route_list);
    point target_point = {sx,sy};

    for(i = 0; i < seq_length(building->route_list); i++)
    {
        route_t     route       = seq_get(building->route_list,i);
        routenode_t start_node  = route_get_start_node(route);
        routenode_t end_node    = route_get_end_node(route);

        point sp = {routenode_get_x(start_node),routenode_get_y(start_node)};
        point ep = {routenode_get_x(end_node),routenode_get_y(end_node)};
        point p_point;
        double d = vector2d_projection_by_point(target_point,sp,ep,&p_point);

        if(d < min_dist)
        {
            min_dist = d;
            *px = p_point.x;
            *py = p_point.y;
            road_num = i;
        }
    }
    printf("realpoint = (%.2f,%.2f)\n",sx,sy);
    //以下部分纯属为了测试
    route_t route   = seq_get(building->route_list,road_num);
    routenode_t sn  = route_get_start_node(route);
    routenode_t en  = route_get_end_node(route);
    route_style style = route_get_style(route);
    printf("num = %d weight = %.2f",route_get_num(route),min_dist);
    printf("(%.2f,%.2f)",routenode_get_x(sn),routenode_get_y(sn));
    printf("%s",style == ONEWAY ? "---->" : "<---->");
    printf("(%.2f,%.2f)\n",routenode_get_x(en),routenode_get_y(en));
    // printf("d = %.2f (%.2f,%.2f)->(%.2f,%.2f) proj(%.2f,%.2f)\n",min_dist,routenode_get_x(targetsp),routenode_get_y(targetsp),routenode_get_x(targetep),routenode_get_y(targetep),*px,*py);

    return road_num;
}

seq_t building_route_plan_by_root(T building,double sx,double sy,double ex,double ey)
{
    int road_num = -1;
    double projx = 0.0;
    double projy = 0.0;
    seq_t nodes = seq_new(2);
    // //先把终点投影到路上并添加到途中
    // road_num = building_get_near_road_point(building,ex,ey,&projx,&projy);
    // //对终点所在路的两个端点进行路径规划
    // route_t route = seq_get(building->route_list,road_num);
    // //routenode_t sn = route_get_start_node(route);
    // routenode_t en = route_get_end_node(route);
    // printf("终点 = %d\n",road_num);
    // // printf("sn = %d en = %d\n",routenode_get_key(sn),routenode_get_key(en));
    //
    // //把起点投影到路上并添加到图中
    // road_num = building_get_near_road_point(building,sx,sy,&projx,&projy);
    // route_t route_start = seq_get(building->route_list,road_num);
    // routenode_t sn = route_get_start_node(route);
    // printf("起点 = %d\n",road_num);


    seq_t point_seq = wdigraph_path(building->graph_root,9,6);
    for(int i = 0; i < seq_length(point_seq); i++)
    {
        routenode_t node = get_routenode_by_pointkey(building,seq_get(point_seq,i));
        if(node != NULL)
        {
            seq_addend(nodes,node);
        }
    }

    // seq_t result = wdigraph_path(building->graph_root,routenode_get_key(sn),;
    //printf("result = %d\n",result == NULL ? 0 : seq_length(point_seq));

    //规划路径
    return nodes;
}

static double get_distance(double x1,double y1,double x2,double y2)
{
    double dist_sq = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);

    return sqrt(dist_sq);
}
void   add_route(T building,floor_t floor,seq_t nodes,int flag)
{
    int j;
    for(j = 0; j < seq_length(nodes); j+=2)
    {
        //一次读两个点 代表一条路
        itemPoint_t start_point = seq_get(nodes,j);
        double sx   = itemPoint_get_x(start_point);
        double sy   = itemPoint_get_y(start_point);
        int    skey  = floor_get_key(floor,sx,sy);
        //查询该点是否已经存储
        routenode_t startnode = sparsearray_get(building->routenode_array,skey);
        if(startnode == NULL)
        {
            startnode = routenode_new(floor_get_index(floor),sparsearray_length(building->routenode_array)+1,sx,sy);
            sparsearray_put(building->routenode_array,skey,startnode);
        }

        itemPoint_t end_point   = seq_get(nodes,j+1);
        double      ex          = itemPoint_get_x(end_point);
        double      ey          = itemPoint_get_y(end_point);
        int         ekey        = floor_get_key(floor,ex,ey);
        routenode_t endnode = sparsearray_get(building->routenode_array,ekey);
        if(endnode == NULL)
        {
            endnode = routenode_new(floor_get_index(floor),sparsearray_length(building->routenode_array)+1,ex,ey);
            sparsearray_put(building->routenode_array,ekey,endnode);
        }

        double dist = get_distance(sx,sy,ex,ey);
        //添加到路途中
        startnode   = sparsearray_get(building->routenode_array,skey);
        endnode     = sparsearray_get(building->routenode_array,ekey);
        int num     = seq_length(building->route_list)+1;
        route_style style = flag == 0 ? ONEWAY : TWOWAY;
        route_t route = route_new(num,startnode,endnode,dist,style);
        seq_addend(building->route_list,route);
    }
}
void   add_graph(T building)
{
    int i = 0;
    building->graph_root = wdigraph_new(seq_length(building->route_list)+1);
    building->graph_car = wdigraph_new(seq_length(building->route_list)+1);
    for(i = 0; i < seq_length(building->route_list); i++)
    {
        route_t route = seq_get(building->route_list,i);
        routenode_t sn      = route_get_start_node(route);
        routenode_t en      = route_get_end_node(route);
        route_style style   = route_get_style(route);
        double      dist    = route_get_weight(route);
        int         skey    = routenode_get_key(sn);
        int         ekey    = routenode_get_key(en);

        if(style == ONEWAY)
        {
            //添加到步行图中
            wdigraph_add_edge(building->graph_root,skey,ekey,dist);
            wdigraph_add_edge(building->graph_root,ekey,skey,dist);
            //添加到车行图中
            wdigraph_add_edge(building->graph_car,skey,ekey,dist);
        }
        else if(style == TWOWAY)
        {
            //添加到步行图中
            wdigraph_add_edge(building->graph_root,skey,ekey,dist);
            wdigraph_add_edge(building->graph_root,ekey,skey,dist);
            //添加到车行图中
            wdigraph_add_edge(building->graph_car,skey,ekey,dist);
            wdigraph_add_edge(building->graph_car,ekey,skey,dist);
        }
    }
}
routenode_t get_routenode_by_pointkey(T building,int point_key)
{
    int i;
    routenode_t node;
    for(i = 0; i < sparsearray_length(building->routenode_array); i++)
    {
        node = sparsearray_get_at(building->routenode_array,i);
        if(routenode_get_key(node) == point_key)
        {
            return node;
        }
    }

    return NULL;
}
