
#ifndef __ENVELOP_H__
#define __ENVELOP_H__

#define T envelop_t
typedef struct T *T;

extern T        envelop_new(double left,double top,double right,double bottom);
extern T        envelop_new_point(double x,double y);
//extern T        envelop_new_box(double left,double top,double right,double bottom);
extern void     envelop_free(T *e);

/**
 *
 */
extern int      envelop_contains_point(T e,double x,double y);
extern int      envelop_contains(T e,T target);
extern double   envelop_distance_sq(T e,T target);
extern int      envelop_intersect(T e,T target);
extern int      envelop_intersect_box(T e,double left,double top,double right,double bottom);

extern void     envelop_set_left(T e,double left);
extern void     envelop_set_top(T e,double top);
extern void     envelop_set_right(T e,double right);
extern void     envelop_set_bottom(T e,double bottom);

extern double   envelop_get_left(T e);
extern double   envelop_get_top(T e);
extern double   envelop_get_right(T e);
extern double   envelop_get_bottom(T e);
extern double   envelop_get_width(T e);
extern double   envelop_get_height(T e);
extern double   envelop_get_center_x(T e);
extern double   envelop_get_center_y(T e);

#undef T
#endif
