
#include <stdlib.h>
#include "assert.h"
#include "mem.h"
#include "routenode.h"

#define T routenode_t
struct T
{
    int floor_index;
    int key;
    double x;
    double y;
};

T    routenode_new(int floor_index,int key,double x,double y)
{
    T routenode;
    NEW0(routenode);

    routenode->floor_index = floor_index;
    routenode->key = key;
    routenode->x = x;
    routenode->y = y;

    return routenode;
}
void routenode_set_floorindex(T routenode,int index)
{
    assert(routenode);
    routenode->floor_index = index;
}
void routenode_set_x(T routenode,double x)
{
    assert(routenode);
    routenode->x = x;
}
void routenode_set_y(T routenode,double y)
{
    assert(routenode);
    routenode->y = y;
}
void routenode_set_key(T routenode,int key)
{
    assert(routenode);
    routenode->key = key;
}

int routenode_get_floorindex(T routenode)
{
    assert(routenode);
    return routenode->floor_index;
}
double routenode_get_x(T routenode)
{
    assert(routenode);
    return routenode->x;
}
double routenode_get_y(T routenode)
{
    assert(routenode);
    return routenode->y;
}
long routenode_get_key(T routenode)
{
    assert(routenode);
    return routenode->key;
}
