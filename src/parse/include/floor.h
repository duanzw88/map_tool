#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "layer.h"
#include "wdigraph.h"

#define T floor_t
typedef struct T *T;

/**
 * 新建楼层
 * @param  name 楼层名
 * @return      楼层对象
 */
extern T floor_new(const char *name);
/**
 * 根据楼层属性创建楼层
 * @param  name   楼层名
 * @param  index  楼层索引
 * @param  width  楼层宽度
 * @param  height 楼层高度
 * @return        楼层对象
 */
extern T floor_new_by_attribute(const char *name,float width,float height);
/**
 * 设置floor所属建筑物的名称
 * @param floor      楼层对象
 * @param build_name 该建筑物的名称
 */
extern void floor_set_buildname(T floor,const char *build_name);
/**
 * 设置楼层索引
 * @param floor 楼层对象
 * @param index 索引值
 */
extern void floor_set_index(T floor,int index);
extern void floor_set_grid(T floor,int grid);

/**
 * 设置楼层宽度
 * @param floor 楼层对象
 * @param width 楼层宽度
 */
extern void floor_set_width(T floor,float width);
/**
 * 设置楼层长度
 * @param floor  楼层对象
 * @param height 楼层高度
 */
extern void floor_set_height(T floor,float height);

/**
 * 楼层中添加layer
 * @param floor 楼层对象
 * @param layer Layer对象
 */
extern void floor_add_layer(T floor,layer_t layer);
/**
 * 获取楼层所在建筑物的名称
 * @param  floor 楼层对象
 * @return       建筑物名称
 */
extern const char *floor_get_buildname(T floor);
/**
 * [floor_get_name description]
 * @param  floor [description]
 * @return       [description]
 */
extern const char *floor_get_name(T floor);
/**
 * [floor_get_index description]
 * @param  floor [description]
 * @return       [description]
 */
extern int floor_get_index(T floor);
extern int floor_get_grid(T floor);
/**
 * [floor_get_width description]
 * @param  floor [description]
 * @return       [description]
 */
extern float floor_get_width(T floor);
/**
 * [floor_get_height description]
 * @param  floor [description]
 * @return       [description]
 */
extern float floor_get_height(T floor);

/**
 * 获取layer个数
 * @param  floor 楼层对象
 * @return       layer个数
 */
extern int floor_get_layer_count(T floor);

/**
 * 从楼层根据索引中获取layer
 * @param  floor 楼层对象
 * @param  index 索引值
 * @return       layer对象
 */
extern layer_t floor_get_layer_by_index(T floor,int index);
extern layer_t floor_get_layer_by_name(T floor,const char *name);
extern int      floor_get_route_count(T floor);
//添加路点
extern void     floor_add_route_nodes(T floor);
extern seq_t floor_add_oneway_road(T floor);
extern seq_t floor_add_twoway_road(T floor);
//extern void floor_add_all_road_point(T floor,seq_t oneway_points,seq_t twoway_points);
extern int floor_get_key(T floor,double x,double y);

#undef T
#endif
