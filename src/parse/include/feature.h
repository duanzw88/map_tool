

#ifndef __FEATURE_H__
#define __FEATURE_H__

#include "item.h"
#include "color.h"

#define T feature_t
typedef struct T *T;

extern T feature_new();
extern void feature_set_type(T feature,const char *type);
extern void feature_set_geotype(T feature,const char *type);
extern void feature_set_fill_color(T feature,color_t color);
extern void feature_set_stroke_color(T feature,color_t color);
extern void feature_add_item(T feature,item_t item);
extern void feature_set_stroke_width(T feature,float width);

extern char     *feature_get_type(T feature);
extern char     *feature_get_geotype(T feature);
extern color_t  feature_get_fill_color(T feature);
extern color_t  feature_get_stroke_color(T feature);
extern int      feature_get_item_count(T feature);
extern item_t   feature_get_item(T feature,int index);
extern float    feature_get_stroke_width(T feature);

#undef T
#endif
