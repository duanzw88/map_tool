
#ifndef __QUADNODE_H__
#define __QUADNODE_H__

// #include "seq.h"
#include "feature.h"
#include "envelop.h"

#define T  quadnode_t
typedef struct T *T;

extern T    quadnode_new(double left,double top,double right,double bottom);
extern T    quadnode_new_space(envelop_t space);

extern void     quadnode_insert(T qn,feature_t f);
extern void     quadnode_remove(T qn,feature_t f);

extern int      quadnode_contains_check_point(T qn,double x,double y);
extern int      quadnode_contains_intersect_check_box(T qn,envelop_t checkbox);

extern int      quadnode_search(T qn,envelop_t searchbox,double maxdistance,int maxcount,seq_t retlist);

#undef T

#endif
