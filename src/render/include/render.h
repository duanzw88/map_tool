
#ifndef __RENDER_H__
#define __RENDER_H__

#include <gtk/gtk.h>
#include "building.h"
#include "parse.h"

// extern void drawing_profile(float x_scale,float y_scale);
extern void drawing_floor_grid(cairo_t *cr,int grid,float width,float height,float scale);
extern void drawing_click_point(cairo_t *cr,float x,float y);
extern void drawing_point(cairo_t *cr,itemPoint_t point,float x_scale,float y_scale);
extern void drawing_line(cairo_t *cr,itemPoint_t line,float x_scale,float y_scale);
extern void drawing_rect(cairo_t *cr,itemRect_t rect);
extern void drawing_circle(cairo_t *cr,itemCircle_t circle);
extern void drawing_ellipse(cairo_t *cr,itemEllipse_t ellipse);
extern void drawing_polyline(cairo_t *cr,feature_t feature,itemPolyline_t polyline,float x_scale,float y_scale);
extern void drawing_polygon(cairo_t *cr,feature_t feature,itemPolygon_t polygon);
extern void drawing_path(cairo_t *cr,feature_t feature,itemPath_t path);

#endif
