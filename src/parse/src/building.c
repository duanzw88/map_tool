

#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "seq.h"
#include "mem.h"
#include "wdigraph.h"
#include "routenode.h"
#include "building.h"

#define T building_t
struct T
{
    char *name;
    seq_t floor_seq;
    wdigraph_t graph_root;
    wdigraph_t graph_car;
    seq_t routenode_list;
};
// struct route_node_t
// {
//     int index;
//     double x;
//     double y;
// };

static double get_distance(double x1,double y1,double x2,double y2);
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
    build->routenode_list = seq_new(20);
    build->graph_root = NULL;
    build->graph_car = NULL;

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
    routenode_t routenode;
    int route_count = 0;
    for(i = 0; i < seq_length(building->floor_seq); i++)
    {
        floor = seq_get(building->floor_seq,i);
        route_count += floor_get_route_count(floor);
    }
    building->graph_root = wdigraph_new(route_count);
    building->graph_car = wdigraph_new(route_count * 2);

    printf("rounte count = %d\n",route_count);
    for(i = 0; i < seq_length(building->floor_seq); i++)
    {
        floor = seq_get(building->floor_seq,i);
        route_count += floor_get_route_count(floor);
        oneway_points = floor_add_oneway_road(floor);
        for(j = 0; j < seq_length(oneway_points); j+=2)
        {
            //一次读两个点 代表一条路
            itemPoint_t start_point = seq_get(oneway_points,j);
            itemPoint_t end_point = seq_get(oneway_points,j+1);
            double sx = itemPoint_get_x(start_point);
            double sy = itemPoint_get_y(start_point);
            double ex = itemPoint_get_x(end_point);
            double ey = itemPoint_get_y(end_point);
            double dist = get_distance(sx,sy,ex,ey);
            printf("(%.2f %.2f)-->(%.2f,%.2f)\n",sx,sy,ex,ey);

            //添加到车行图中
            wdigraph_add_edge(building->graph_car,node_index,node_index+1,dist);
            //添加到人行图中
            wdigraph_add_edge(building->graph_root,node_index,node_index+1,dist);
            wdigraph_add_edge(building->graph_root,node_index+1,node_index,dist);

            routenode = routenode_new(floor_get_index(floor),node_index++,sx,sy);
            seq_addend(building->routenode_list,routenode);
            routenode = routenode_new(floor_get_index(floor),node_index++,ex,ey);
            seq_addend(building->routenode_list,routenode);
        }
        twoway_points = floor_add_twoway_road(floor);
        for(j = 0; j < seq_length(twoway_points); j+=2)
        {
            //一次读两个点 代表一条路
            itemPoint_t start_point = seq_get(twoway_points,j);
            itemPoint_t end_point = seq_get(twoway_points,j+1);
            double sx = itemPoint_get_x(start_point);
            double sy = itemPoint_get_y(start_point);
            double ex = itemPoint_get_x(end_point);
            double ey = itemPoint_get_y(end_point);
            double dist = get_distance(sx,sy,ex,ey);
            printf("(%.2f %.2f)<-->(%.2f,%.2f)\n",sx,sy,ex,ey);

            //添加到车行图中
            wdigraph_add_edge(building->graph_car,node_index,node_index+1,dist);
            //添加到人行图中
            wdigraph_add_edge(building->graph_root,node_index,node_index+1,dist);
            wdigraph_add_edge(building->graph_root,node_index+1,node_index,dist);

            routenode = routenode_new(floor_get_index(floor),node_index++,sx,sy);
            seq_addend(building->routenode_list,routenode);
            routenode = routenode_new(floor_get_index(floor),node_index++,ex,ey);
            seq_addend(building->routenode_list,routenode);
        }
    }

    printf("node_index = %d\n",node_index);
    printf("Car:route = %d\n",wdigraph_edges(building->graph_car));
    printf("Root:route = %d\n",wdigraph_edges(building->graph_root));
    printf("route point = %d\n",seq_length(building->routenode_list));
}

void building_get_near_road_point(T building,double sx,double sy,double px,double py)
{
    assert(building);
    assert(building->graph_car);
    //到此为止 下一步计算投影 需要用到向量来计算
}
static double get_distance(double x1,double y1,double x2,double y2)
{
    double dist_sq = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);

    return sqrt(dist_sq);
}
