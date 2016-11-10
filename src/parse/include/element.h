
#ifndef __FEATURE_COLLECTION_H__
#define __FEATURE_COLLECTION_H__

#include "feature.h"

#define T element_t
typedef struct T *T;

/**
 * 新建一个collections
 * @param  name 名称
 * @return      collections实例
 */
extern T element_new(const char *type,const char *name);
extern char *element_get_name(T element);
extern void element_add_features(T element,feature_t features);
extern int  element_get_feature_count(T element);
extern feature_t element_get_features_by_index(T element,int index);

#undef T
#endif
