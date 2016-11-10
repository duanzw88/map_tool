

#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "seq.h"
#include "assert.h"
#include "element.h"

#define T element_t
struct T
{
    char *type;
    char *name;
    seq_t features_seq;
};


/**
 * 新建一个collections
 * @param  name 名称
 * @return      collections实例
 */
T element_new(const char *type,const char *name)
{
    T element;
    NEW(element);

    element->type = CALLOC(1,strlen(type)+1);
    strncpy(element->type,type,strlen(type));

    element->name = CALLOC(1,strlen(name)+1);
    strncpy(element->name,name,strlen(name));

    element->features_seq = seq_new(10);
    return element;
}
char *element_get_name(T element)
{
    assert(element);
    return element->name;
}
void element_add_features(T element,feature_t features)
{
    assert(element);
    seq_addend(element->features_seq,features);
}
int  element_get_feature_count(T element)
{
    assert(element);
    return seq_length(element->features_seq);
}
feature_t element_get_features_by_index(T element,int index)
{
    assert(element);
    assert(index >= 0 && index < seq_length(element->features_seq));
    return seq_get(element->features_seq,index);
}
