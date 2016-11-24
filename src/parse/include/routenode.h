
#ifndef __ROUTENODE_H__
#define __ROUTENODE_H__

#define T routenode_t
typedef struct T *T;

extern T        routenode_new(int floor_index,int key,double x,double y);
extern void     routenode_set_floorindex(T routenode,int index);
extern void     routenode_set_x(T routenode,double x);
extern void     routenode_set_y(T routenode,double y);
extern void     routenode_set_key(T routenode,int key);

extern int      routenode_get_floorindex(T routenode);
extern double   routenode_get_x(T routenode);
extern double   routenode_get_y(T routenode);
extern long     routenode_get_key(T routenode);


#undef T
#endif
