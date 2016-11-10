
#ifndef __ITEM_H__
#define __ITEM_H__

#include "item_types.h"

#define T item_t
typedef struct T *T;

typedef enum itemKind
{
    ITEM_KIND_POINT = 0,
    ITEM_KIND_LINE,
    ITEM_KIND_RECT,
    ITEM_KIND_CIRCLE,
    ITEM_KIND_ELLIPSE,
	ITEM_KIND_POLYLINE,
	ITEM_KIND_POLYGON,
    ITEM_KIND_PATH
}itemKind;
extern T item_new(itemKind kind);
extern itemKind item_get_kind(T item);
extern void item_set_point(T item,itemPoint_t point);
extern void item_set_line(T item,itemLine_t line);
extern void item_set_rect(T item,itemRect_t rect);
extern void item_set_circle(T item,itemCircle_t circle);
extern void item_set_ellipse(T item,itemEllipse_t ellipse);
extern void item_set_polyline(T item,itemPolyline_t polyline);
extern void item_set_polygon(T item,itemPolygon_t polygon);
extern void item_set_path(T item,itemPath_t path);

extern itemPoint_t item_get_point(T item);
extern itemLine_t item_get_line(T item);
extern itemRect_t item_get_rect(T item);
extern itemCircle_t item_get_circle(T item);
extern itemEllipse_t item_get_ellipse(T item);
extern itemPolyline_t item_get_polyline(T item);
extern itemPolygon_t item_get_polygon(T item);
extern itemPath_t item_get_path(T item);

#undef T
#endif
