#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "routenode.h"
typedef enum
{
    ONEWAY = 0,
    TWOWAY
}route_style;

#define T route_t
typedef struct T *T;

extern T            route_new(int num,routenode_t start,routenode_t end,double weight,route_style style);
extern routenode_t  route_get_start_node(T route);
extern routenode_t  route_get_end_node(T route);
extern int          route_get_num(T route);
extern double       route_get_weight(T route);
extern route_style  route_get_style(T route);

#undef T
#endif
