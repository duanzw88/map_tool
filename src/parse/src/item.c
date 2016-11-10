
#include <stdlib.h>
#include "mem.h"
#include "assert.h"
#include "item.h"

#define T item_t
struct T
{
    itemKind tKind;
    union
    {
        itemPoint_t       tPoint;     /*tKind == ITEM_KIND_POINT*/
        itemLine_t        tLine;      /*tKind == ITEM_KIND_LINE*/
        itemRect_t        tRect;      /*tKind == ITEM_KIND_RECT*/
        itemCircle_t      tCircle;    /*tKind == ITEM_KIND_CIRCLE*/
        itemEllipse_t     tEllipse;   /*tKind == ITEM_KIND_ELLIPSE*/
        itemPolyline_t    tPolyline;  /*tKind == ITEM_KIND_POLYLINE*/
        itemPolygon_t     tPolygon;   /*tKind == ITEM_KIND_POLYGON*/
        itemPath_t        tPath;      /*tKind == ITEM_KIND_PATH*/
    }tItemStyle;
};

extern T item_new(itemKind kind)
{
    T item;
    NEW(item);

    item->tKind = kind;
    // itemPoint_t point;
    // NEW(point);
    // switch (kind)
    // {
    //     case ITEM_KIND_POINT:
    //         itemPoint_t point;
    //         NEW0(point);
    //         NEW0(item->tItemStyle.tPoint);break;
    //     case ITEM_KIND_LINE:NEW0(item->tItemStyle.tLine);break;
    //     case ITEM_KIND_RECT:NEW0(item->tItemStyle.tRect);break;
    //     case ITEM_KIND_CIRCLE:NEW0(item->tItemStyle.tCircle);break;
    //     case ITEM_KIND_ELLIPSE:NEW0(item->tItemStyle.tEllipse);break;
    // 	case ITEM_KIND_POLYLINE:NEW0(item->tItemStyle.tPolyline);break;
    // 	case ITEM_KIND_POLYGON:NEW0(item->tItemStyle.tPolygon);break;
    //     case ITEM_KIND_PATH:NEW0(item->tItemStyle.tPath);break;
    // }
    return item;
}
itemKind item_get_kind(T item)
{
    assert(item);
    return item->tKind;
}

void item_set_point(T item,itemPoint_t point)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_POINT);
    item->tItemStyle.tPoint = point;
}
void item_set_line(T item,itemLine_t line)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_LINE);

    item->tItemStyle.tLine = line;
}
void item_set_rect(T item,itemRect_t rect)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_RECT);

    item->tItemStyle.tRect = rect;
}
void item_set_circle(T item,itemCircle_t circle)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_CIRCLE);

    item->tItemStyle.tCircle = circle;
}
void item_set_ellipse(T item,itemEllipse_t ellipse)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_ELLIPSE);

    item->tItemStyle.tEllipse = ellipse;
}
void item_set_polyline(T item,itemPolyline_t polyline)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_POLYLINE);

    item->tItemStyle.tPolyline = polyline;
}
void item_set_polygon(T item,itemPolygon_t polygon)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_POLYGON);

    item->tItemStyle.tPolygon = polygon;
}
void item_set_path(T item,itemPath_t path)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_PATH);

    item->tItemStyle.tPath = path;
}

itemPoint_t item_get_point(T item)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_POINT);

    return item->tItemStyle.tPoint;
}
itemLine_t item_get_line(T item)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_LINE);

    return item->tItemStyle.tLine;
}
itemRect_t item_get_rect(T item)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_RECT);

    return item->tItemStyle.tRect;
}
itemCircle_t item_get_circle(T item)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_CIRCLE);

    return item->tItemStyle.tCircle;
}
itemEllipse_t item_get_ellipse(T item)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_ELLIPSE);
    return item->tItemStyle.tEllipse;
}
itemPolyline_t item_get_polyline(T item)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_POLYLINE);

    return item->tItemStyle.tPolyline;
}
itemPolygon_t item_get_polygon(T item)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_POLYGON);

    return item->tItemStyle.tPolygon;
}
itemPath_t item_get_path(T item)
{
    assert(item);
    assert(item->tKind == ITEM_KIND_PATH);

    return item->tItemStyle.tPath;
}
