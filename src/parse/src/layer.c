
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "atom.h"
#include "mem.h"
#include "seq.h"
#include "layer.h"

#define T layer_t

struct T
{
    char *name;
    seq_t element_seq;
};

/**
 * 新建layer
 * @return layer对象
 */
T  layer_new(const char *name)
{
    T layer;
    NEW(layer);

    // layer->name = CALLOC(1,strlen(name) + 1);
    // strncpy(layer->name,name,strlen(name));
    //
    layer->name = (char *)atom_string(name);
    layer->element_seq = seq_new(0);

    return layer;
}

/**
 * 设置Layer对象的名称
 * @param layer layer对象
 * @param name  名称
 */
// void layer_set_name(T layer,char *name)
// {
//     assert(layer);
//
//     layer->name = CALLOC
// }
/**
 * 添加元素
 * @param layer   layer对象
 * @param element 元素对象
 */
void layer_add_element(T layer,element_t element)
{
     assert(layer);
     assert(element);

     seq_addend(layer->element_seq,element);
}
/**
 * 获取layer名称
 * @param  layer layer对象
 * @return       名称
 */
char *layer_get_name(T layer)
{
    assert(layer);
    return layer->name;
}
/**
 * 获取layer中元素的个数
 * @param  layer layer对象
 * @return       元素的总个数
 */
int  layer_get_element_count(T layer)
{
    assert(layer);
    return seq_length(layer->element_seq);
}
/**
 * 根据索引获取element
 * @param  layer layer对象
 * @param  index 索引值
 * @return       element对象
 */
element_t layer_get_element_by_index(T layer,int index)
{
    assert(layer);
    assert(index >= 0 && index < seq_length(layer->element_seq));

    return seq_get(layer->element_seq,index);
}
//获取单项路的element
element_t layer_get_element_oneway(T layer)
{
    element_t element;
    int i;
    for(i = 0; i < seq_length(layer->element_seq); i++)
    {
        element = seq_get(layer->element_seq,i);
        if(strcmp(element_get_name(element),"one-way") == 0)
        {
            return element;
        }
    }

    return NULL;
}
//获取双向路的elememnt
element_t layer_get_element_twoway(T layer)
{
    element_t element;
    int i;
    for(i = 0; i < seq_length(layer->element_seq); i++)
    {
        element = seq_get(layer->element_seq,i);
        if(strcmp(element_get_name(element),"two-way") == 0)
        {
            return element;
        }
    }

    return NULL;
}
