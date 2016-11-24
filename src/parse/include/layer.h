
#ifndef __LAYER_H__
#define __LAYER_H__

#include "element.h"

#define T layer_t
typedef struct T *T;

/**
 * 新建layer
 * @return layer对象
 */
extern T    layer_new(const char *name);

/**
 * 设置Layer对象的名称
 * @param layer layer对象
 * @param name  名称
 */
//extern void layer_set_name(T layer,char *name);
/**
 * 添加元素
 * @param layer   layer对象
 * @param element 元素对象
 */
extern void layer_add_element(T layer,element_t element);
/**
 * 获取layer名称
 * @param  layer layer对象
 * @return       名称
 */
extern char *layer_get_name(T layer);
/**
 * 获取layer中元素的个数
 * @param  layer layer对象
 * @return       元素的总个数
 */
extern int  layer_get_element_count(T layer);
/**
 * 根据索引获取element
 * @param  layer layer对象
 * @param  index 索引值
 * @return       element对象
 */
extern element_t layer_get_element_by_index(T layer,int index);
//获取单项路的element
extern element_t layer_get_element_oneway(T layer);
//获取双向路的elememnt
extern element_t layer_get_element_twoway(T layer);

#undef T
#endif
