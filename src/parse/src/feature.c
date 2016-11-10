
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "seq.h"
#include "assert.h"
#include "feature.h"

#define T feature_t
struct T
{
    char *type;
    char *geo_type;
    seq_t  item_seq;
    color_t fill_color;
    color_t stroke_color;
    float stroke_width;
};

T feature_new()
{
    T feature;
    NEW(feature);

    feature->type = NULL;
    feature->geo_type = NULL;
    feature->item_seq = seq_new(10);
    feature->fill_color = NULL;
    feature->stroke_color = NULL;
    feature->stroke_width = 0.0;
    return feature;
}
void feature_set_type(T feature,const char *type)
{
    assert(feature);
    feature->type = CALLOC(1,strlen(type)+1);

    strncpy(feature->type,type,strlen(type));
}
void feature_set_geotype(T feature,const char *type)
{
    assert(feature);
    feature->geo_type = CALLOC(1,strlen(type)+1);

    strncpy(feature->geo_type,type,strlen(type));
}
void feature_set_fill_color(T feature,color_t color)
{
    assert(feature);
    feature->fill_color = color;
}
void feature_set_stroke_color(T feature,color_t color)
{
    assert(feature);
    feature->stroke_color = color;
}
void feature_add_item(T feature,item_t item)
{
    assert(feature);
    assert(item);

    seq_addend(feature->item_seq,item);
}
void feature_set_stroke_width(T feature,float width)
{
    assert(feature);
    feature->stroke_width = width;
}


char     *feature_get_type(T feature)
{
    assert(feature);
    return feature->type;
}
char     *feature_get_geotype(T feature)
{
    assert(feature);
    return feature->geo_type;
}
color_t  feature_get_fill_color(T feature)
{
    assert(feature);
    return feature->fill_color;
}
color_t  feature_get_stroke_color(T feature)
{
    assert(feature);
    return feature->stroke_color;
}
int feature_get_item_count(T feature)
{
    assert(feature);
    return seq_length(feature->item_seq);
}
item_t   feature_get_item(T feature,int index)
{
    assert(feature);
    assert(index >= 0 && index < seq_length(feature->item_seq));

    return seq_get(feature->item_seq,index);
}
float    feature_get_stroke_width(T feature)
{
    assert(feature);
    return feature->stroke_width;
}
