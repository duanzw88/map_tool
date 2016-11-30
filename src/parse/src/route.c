
#include <stdlib.h>
#include "mem.h"
#include "assert.h"

#include "route.h"

#define T route_t
struct T
{
    int num;
    routenode_t start;
    routenode_t end;
    route_style style;
    double weight;
};

T route_new(int num,routenode_t start,routenode_t end,double weight,route_style style)
{
    T route;
    NEW0(route);

    route->num = num;
    route->start = start;
    route->end = end;
    route->weight = weight;
    route->style = style;

    return route;
}
routenode_t  route_get_start_node(T route)
{
    assert(route);
    return route->start;
}
routenode_t  route_get_end_node(T route)
{
    assert(route);
    return route->end;
}
int          route_get_num(T route)
{
    assert(route);
    return route->num;
}
double       route_get_weight(T route)
{
    assert(route);
    return route->weight;
}
route_style  route_get_style(T route)
{
    assert(route);
    return route->style;
}
