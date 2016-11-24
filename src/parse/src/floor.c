
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

static double get_distance(double x1,double y1,double x2,double y2);
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
        return NULL;
    }

    element = layer_get_element_oneway(navilayer);
    for(i = 0; i < element_get_feature_count(element); i++)
    {
        feature = element_get_features_by_index(element,i);
        seq_t coords = feature_get_road_coords(feature);
        ret += seq_length(coords);
    }
    element = layer_get_element_twoway(navilayer);
    for(i = 0; i < element_get_feature_count(element); i++)
    {
        feature = element_get_features_by_index(element,i);
        seq_t coords = feature_get_road_coords(feature);
        ret += seq_length(coords);
    }

    return ret;
}

//添加路点

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
            printf("one-way (%.2f,%.2f)\n",itemPoint_get_x(point),itemPoint_get_y(point));
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
// void floor_add_all_road_point(T floor,seq_t oneway_points,seq_t twoway_points);
// {
//     int i,j;
//     long key;
//     int point_index_car = 0;
//     int point_index_root = 0;
//     assert(floor);
//     oneway_points = floor_add_oneway_road(floor);
//     // if(oneway_points != NULL && seq_length(oneway_points) != 0)
//     // {
//     //     for(j = 0; j < seq_length(oneway_points);j+=2)
//     //     {
//     //         itemPoint_t start_point = seq_get(oneway_points,j);
//     //         itemPoint_t end_point = seq_get(oneway_points,j+1);
//     //         double sx = itemPoint_get_x(start_point);
//     //         double sy = itemPoint_get_y(start_point);
//     //         double ex = itemPoint_get_x(end_point);
//     //         double ey = itemPoint_get_y(end_point);
//     //
//     //         int startKey = floor_get_key(floor,sx,sy);
//     //         int endKey = floor_get_key(floor,ex,ey);
//     //         double dist = get_distance(sx,sy,ex,ey);
//     //         wdigraph_add_edge(graph_car,point_index_car,point_index_car+1,dist);
//     //         wdigraph_add_edge(graph_root,point_index_root,point_index_root+1,dist);
//     //         wdigraph_add_edge(graph_root,point_index_root+1,point_index_root,dist);
//     //         point_index_car++;
//     //         point_index_root+=2;
//     //         // printf("one-way %d:(%.2f,%.2f)\n",key,itemPoint_get_x(point),itemPoint_get_y(point));
//     //     }
//     // }
//     twoway_points = floor_add_twoway_road(floor);
//     // if(twoway_points != NULL && seq_length(twoway_points) != 0)
//     // {
//     //     for(j = 0; j < seq_length(twoway_points);j+=2)
//     //     {
//     //         itemPoint_t start_point = seq_get(twoway_points,j);
//     //         itemPoint_t end_point = seq_get(twoway_points,j+1);
//     //         double sx = itemPoint_get_x(start_point);
//     //         double sy = itemPoint_get_y(start_point);
//     //         double ex = itemPoint_get_x(end_point);
//     //         double ey = itemPoint_get_y(end_point);
//     //
//     //         int startKey = floor_get_key(floor,sx,sy);
//     //         int endKey = floor_get_key(floor,ex,ey);
//     //         double dist = get_distance(sx,sy,ex,ey);
//     //         wdigraph_add_edge(graph_car,point_index_car,point_index_car+1,dist);
//     //         wdigraph_add_edge(graph_car,point_index_car+1,point_index_car,dist);
//     //         wdigraph_add_edge(graph_root,point_index_root,point_index_root+1,dist);
//     //         wdigraph_add_edge(graph_root,point_index_root+1,point_index_root,dist);
//     //         point_index_car+=2;
//     //         point_index_root+=2;
//     //         // printf("one-way %d:(%.2f,%.2f)\n",key,itemPoint_get_x(point),itemPoint_get_y(point));
//     //     }
//     // }
//     // printf("Car:edges:%d\n",wdigraph_edges(graph_car));
//     // printf("Root:edges:%d\n",wdigraph_edges(graph_root));
//
// }
int floor_get_key(T floor,double x,double y)
{
    assert(floor);
    assert(floor->grid != 0);
    // printf("grid = %d\n",floor->grid);
    int x_index = (int)(x / floor->grid);
    int y_index = (int)(y / floor->grid);
    // printf("y_index = %d x_index = %d\n",y_index,x_index);

    //index      y        x
    //------  ------- -------
    return (floor->index << 21) + (y_index << 10) + (x_index);
}

static double get_distance(double x1,double y1,double x2,double y2)
{
    double dist_sq = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);

    return sqrt(dist_sq);
}
