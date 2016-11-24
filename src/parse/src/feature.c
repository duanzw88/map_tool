
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "assert.h"
#include "item_types.h"
#include "feature.h"

#define T feature_t
struct T
{
    long id;
    char *type;        //
    char *geo_type;     //几何属性
    item_t  item;
    envelop_t extent;
    color_t fill_color;
    color_t stroke_color;
    float stroke_width;
};

T feature_new(long id)
{
    T feature;
    NEW(feature);

    feature->id = id;
    feature->type = NULL;
    feature->geo_type = NULL;
    feature->item = NULL;
    feature->extent = NULL;//envelop_new(0.0,0.0,0.0,0.0);
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
void feature_set_item(T feature,item_t item)
{
    assert(feature);
    assert(item);

    feature->item = item;
}
void     feature_set_extent(T feature,envelop_t extent)
{
    assert(feature);
    assert(extent);

    feature->extent = extent;
}
void feature_set_stroke_width(T feature,float width)
{
    assert(feature);
    feature->stroke_width = width;
}

long     feature_get_id(T feature)
{
    assert(feature);
    return feature->id;
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

item_t   feature_get_item(T feature)
{
    assert(feature);

    return feature->item;
}
envelop_t feature_get_extent(T feature)
{
    assert(feature);
    return feature->extent;
}
float    feature_get_stroke_width(T feature)
{
    assert(feature);
    return feature->stroke_width;
}

seq_t    feature_get_road_coords(T feature)
{
    int i;
    seq_t coords;
    itemPolyline_t polyline;
    assert(feature);
    int type_diff = strncmp("LineString",feature->geo_type,strlen("LineString"));
    // int name_diff = strncmp("road",feature->, size_t)
    if(type_diff != 0)
    {
        return NULL;
    }
    coords = seq_new(20);
    if(item_get_kind(feature->item) != ITEM_KIND_POLYLINE)
    {
        seq_free(&coords);
        return NULL;
    }

    polyline = item_get_polyline(feature->item);
    for(i = 0; i < itemPolyline_get_point_count(polyline); i++)
    {
        itemPoint_t point = itemPolyline_get_point_by_index(polyline,i);
        seq_addend(coords,point);
    }
    return coords;
}
