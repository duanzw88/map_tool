


#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "floor.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define T building_t
typedef struct T *T;

/**
 * 新建建筑物
 * @param  name 建筑物名称
 * @return      建筑物对象
 */
extern T building_new(const char *name);

/**
 * 释放建筑物对象
 * @param build 建筑物
 */
extern void building_free(T *build);

/**
 * 获取建筑物名称
 * @param  build 建筑物
 * @return       建筑物名称
 */
extern const char *building_get_name(T build);
/**
 * 在建筑物中添加楼层
 * @param  build 建筑物
 * @param  floor 楼层
 * @return       1 添加成功
 *               0 添加失败
 */
extern void building_add_floor(T build,floor_t floor);
/**
 * 根据索引获取楼层
 * @param  build 建筑物
 * @param  index 索引值
 * @return       NULL 索引值不存在
 *               floor 楼层
 */
extern floor_t building_get_floor_by_index(T build,int index);
/**
 * 根据索引获取楼层
 * @param  build 建筑物
 * @param  name  索引值
 * @return       NULL 索引值不存在
 *               floor 楼层
 */
extern floor_t building_get_floor_by_name(T build,const char *name);

/**
 * 获取楼层数
 * @param  build 建筑物
 * @return       楼层总数
 */
extern int building_get_floor_count(T build);

#ifdef __cplusplus
}
#endif

#undef T
#endif
