
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "assert.h"
#include "mem.h"
#include "seq.h"
#include "floor.h"

#define T floor_t
struct T
{
    char  *build_name;       //设置所属build的名称
    int index;              //本层楼的索引
    char *name;             //本层楼的名称
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
