

#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "seq.h"
#include "mem.h"
#include "building.h"

#define T building_t
struct T
{
    char *name;
    seq_t floor_seq;
};


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
