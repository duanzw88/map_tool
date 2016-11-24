
#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "seq.h"
#include "envelop.h"
#include "feature.h"
#include "quadnode.h"

#define T quadtree_t
typedef struct T *T;


extern T            quadtree_new(double left,double top,double right,double bottom);
extern T            quadtree_new_space(envelop_t extent);
extern void         quadtree_free(T qt);

extern feature_t    quadtree_get(T qt,int id);
extern int          quadtree_get_count(T qt);
extern feature_t    quadtree_get_by_index(T qt,int index);
extern void         quadtree_insert(T qt,feature_t f);

extern int          quadtree_contains_point(T qt,double x,double y);
extern int          quadtree_intersect(T qt,double left,double top,double right,double bottom);
extern int          quadtree_intersect_by_envelop(T qt,envelop_t e);

extern seq_t        quadtree_search(T qt,double left,double top,double right,double bottom,double maxdistance,int maxcount,seq_t retlist);
extern seq_t        quadtree_search_by_envelop(T qt,envelop_t searchbox,double maxdistance,int maxcount,seq_t retlist);
#undef T
#endif
