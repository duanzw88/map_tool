
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "assert.h"
#include "atom.h"
#include "mem.h"
#include "seq.h"
#include "floor.h"

#define T floor_t
struct T
{
    char  *build_name;       //设置所属build的名称
    int index;              //本层楼的索引
    char *name;             //本层楼的名称
    int  grid;              //网格
    float width;            //本楼层宽度
    float height;           //本楼层高度
    seq_t  layers;          //本楼层layers列表
};

/**
 * 新建楼层
 * @param  name 楼层名
 * @return      楼层对象
 */
T floor_new(const char *name)
{
    T floor;
    NEW(floor);

    floor->name = CALLOC(1,strlen(name)+1);
    strncpy(floor->name,name,strlen(name));
    floor->index = -1;
    floor->grid = 20;
    floor->width = 0.0;
    floor->height = 0.0;
    floor->build_name = NULL;
    floor->layers = seq_new(0);

    return floor;
}
/**
 * 根据楼层属性创建楼层
 * @param  name   楼层名
 * @param  index  楼层索引
 * @param  width  楼层宽度
 * @param  height 楼层高度
 * @return        楼层对象
 */
T floor_new_by_attribute(const char *name,float width,float height)
{
    T floor;
    NEW(floor);

    floor->name = CALLOC(1,strlen(name)+1);
    strncpy(floor->name,name,strlen(name));
    floor->index = -1;
    floor->grid = 20;
    floor->width = width;
    floor->height = height;
    floor->layers = seq_new(0);

    return floor;
}
/**
 * 设置floor所属建筑物的名称
 * @param floor      楼层对象
 * @param build_name 该建筑物的名称
 */
void floor_set_buildname(T floor,const char *build_name)
{
    assert(floor);
    floor->build_name = CALLOC(1,strlen(build_name) + 1);
    strncpy(floor->build_name,build_name,strlen(build_name));
}

/**
 * 设置楼层索引
 * @param floor 楼层对象
 * @param index 索引值
 */
void floor_set_index(T floor,int index)
{
    assert(floor);
    assert(index >= 0);
    floor->index = index;
}
void floor_set_grid(T floor,int grid)
{
    assert(floor);
    floor->grid = grid;
}
/**
 * 设置楼层宽度
 * @param floor 楼层对象
 * @param width 楼层宽度
 */
void floor_set_width(T floor,float width)
{
    assert(floor);
    assert(width >= 0.0);
    floor->width = width;
}
/**
 * 设置楼层长度
 * @param floor  楼层对象
 * @param height 楼层高度
 */
void floor_set_height(T floor,float height)
{
    assert(floor);
    assert(height >= 0);
    floor->height = height;
}

/**
 * 楼层中添加layer
 * @param floor 楼层对象
 * @param layer Layer对象
 */
void floor_add_layer(T floor,layer_t layer)
{
    assert(floor);
    assert(layer);

    seq_addend(floor->layers,layer);
}
/**
 * [floor_get_name description]
 * @param  floor [description]
 * @return       [description]
 */
const char *floor_get_name(T floor)
{
    assert(floor);
    return floor->name;
}
/**
 * 获取楼层所在建筑物的名称
 * @param  floor 楼层对象
 * @return       建筑物名称
 */
const char *floor_get_buildname(T floor)
{
    assert(floor);
    return floor->build_name;
}
/**
 * [floor_get_index description]
 * @param  floor [description]
 * @return       [description]
 */
int floor_get_index(T floor)
{
    assert(floor);
    return floor->index;
}
int floor_get_grid(T floor)
{
    assert(floor);
    return floor->grid;
}
/**
 * [floor_get_width description]
 * @param  floor [description]
 * @return       [description]
 */
float floor_get_width(T floor)
{
    assert(floor);
    return floor->width;
}
/**
 * [floor_get_height description]
 * @param  floor [description]
 * @return       [description]
 */
float floor_get_height(T floor)
{
    assert(floor);
    return floor->height;
}
/**
 * 获取layer个数
 * @param  floor 楼层对象
 * @return       layer个数
 */
int floor_get_layer_count(T floor)
{
    assert(floor);
    return seq_length(floor->layers);
}

/**
 * 从楼层根据索引中获取layer
 * @param  floor 楼层对象
 * @param  index 索引值
 * @return       layer对象
 */
layer_t floor_get_layer_by_index(T floor,int index)
{
    assert(floor);
    assert(index >= 0);
    if(index >= seq_length(floor->layers))
    {
        return NULL;
    }

    return seq_get(floor->layers,index);
}

layer_t floor_get_layer_by_name(T floor,const char *name)
{
    int len,i;
    layer_t layer;
    assert(floor);
    assert(name);

    name = atom_string(name);
    len = seq_length(floor->layers);
    for(i = 0; i < len; i++)
    {
        layer = seq_get(floor->layers,i);
        if(name == layer_get_name(layer))
        {
            return layer;
        }
    }

    return NULL;
}
int floor_get_route_count(T floor)
{
    int i,j;
    int ret = 0;
    element_t element;
    feature_t feature;
    seq_t    point_seq;
    assert(floor);
    layer_t navilayer;

    point_seq = seq_new(20);
    navilayer = floor_get_layer_by_name(floor,"road");
    if(!navilayer)
    {
        return 0;
    }

    element = layer_get_element_oneway(navilayer);
    ret += element_get_feature_count(element);

    element = layer_get_element_twoway(navilayer);
    ret += element_get_feature_count(element);

    return ret;
}


//添加路点
void floor_add_route_nodes(T floor)
{
    // assert(floor);
    // layer_t navilayer;
    // seq_t oneway_points;
    // seq_t twoway_points;
    //
    // oneway_points = floor_add_oneway_road(floor);
    // for(j = 0; j < seq_length(oneway_points); j++)
    // {
    //     //一次读两个点 代表一条路
    //     itemPoint_t start_point = seq_get(oneway_points,j);
    //
    //     double sx = itemPoint_get_x(start_point);
    //     double sy = itemPoint_get_y(start_point);
    //     double ex = itemPoint_get_x(end_point);
    //     double ey = itemPoint_get_y(end_point);
    //     double dist = get_distance(sx,sy,ex,ey);
    //
    //     itemPoint_t end_point = seq_get(oneway_points,j+1);
    //
    //     //添加到路途中
    //     routenode_t startnode = routenode_new(floor_get_index(floor),node_index++,sx,sy);
    //     routenode_t endnode = routenode_new(floor_get_index(floor),node_index++,ex,ey);
    //     int num = seq_length(building->route_list);
    //     route_style style = ONEWAY;
    //     route_t route = route_new(num,startnode,endnode,dist,style);
    //     seq_addend(building->route_list,route);
    // }
}
seq_t floor_add_oneway_road(T floor)
{
    int i,j;
    element_t element;
    feature_t feature;
    seq_t    point_seq;
    assert(floor);
    layer_t navilayer;

    point_seq = seq_new(20);
    navilayer = floor_get_layer_by_name(floor,"road");
    if(!navilayer)
    {
        return NULL;
    }
    element = layer_get_element_oneway(navilayer);
    for(i = 0; i < element_get_feature_count(element); i++)
    {
        feature = element_get_features_by_index(element,i);
        seq_t coords = feature_get_road_coords(feature);
        for(j = 0; j < seq_length(coords);j++)
        {
            itemPoint_t point = seq_get(coords,j);
            seq_addend(point_seq,point);
        }
    }

    return point_seq;
}
seq_t floor_add_twoway_road(T floor)
{
    int i,j;
    element_t element;
    feature_t feature;
    seq_t    point_seq;
    assert(floor);
    layer_t navilayer;

    point_seq = seq_new(20);
    navilayer = floor_get_layer_by_name(floor,"road");
    if(!navilayer)
    {
        return NULL;
    }
    element = layer_get_element_twoway(navilayer);
    for(i = 0; i < element_get_feature_count(element); i++)
    {
        feature = element_get_features_by_index(element,i);
        seq_t coords = feature_get_road_coords(feature);
        for(j = 0; j < seq_length(coords);j++)
        {
            itemPoint_t point = seq_get(coords,j);
            seq_addend(point_seq,point);
            // printf("two-way (%.2f,%.2f)\n",itemPoint_get_x(point),itemPoint_get_y(point));
        }
    }

    return point_seq;
}

int floor_get_key(T floor,double x,double y)
{
    assert(floor);
    assert(floor->grid != 0);

    int x_index = (int)(x / floor->grid);
    int y_index = (int)(y / floor->grid);

    // printf("x = %.2f y = %.2f grid = %d x_index = %d y_index = %d\n",x,y,floor->grid,x_index,y_index);
    return (floor->index << 21) + (y_index << 10) + (x_index);
}
