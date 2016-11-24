

#ifndef __FEATURE_H__
#define __FEATURE_H__

#include "../../algorithm/include/seq.h"
#include "item.h"
#include "color.h"
#include "envelop.h"

#define T feature_t
typedef struct T *T;

extern T        feature_new(long id);
extern void     feature_set_type(T feature,const char *type);
extern void     feature_set_geotype(T feature,const char *type);
extern void     feature_set_fill_color(T feature,color_t color);
extern void     feature_set_stroke_color(T feature,color_t color);
extern void     feature_set_item(T feature,item_t item);
extern void     feature_set_extent(T feature,envelop_t extent);
extern void     feature_set_stroke_width(T feature,float width);

extern long     feature_get_id(T feature);
extern char     *feature_get_type(T feature);
extern char     *feature_get_geotype(T feature);
extern color_t  feature_get_fill_color(T feature);
extern color_t  feature_get_stroke_color(T feature);
extern item_t   feature_get_item(T feature);
extern envelop_t feature_get_extent(T feature);
extern float    feature_get_stroke_width(T feature);

//获取路点坐标
extern seq_t    feature_get_road_coords(T feature);
#undef T
#endif
