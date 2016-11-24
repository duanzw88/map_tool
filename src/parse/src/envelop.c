#include <stdlib.h>

#include "mem.h"
#include "assert.h"
#include "envelop.h"


#define T envelop_t
struct T
{
    double left;
    double top;
    double right;
    double bottom;
};

T envelop_new(double left,double top,double right,double bottom)
{
    T e;

    NEW0(e);
    e->left = left;
    e->top = top;
    e->right = right;
    e->bottom = bottom;

    return e;
}
T envelop_new_point(double x,double y)
{
    T e;
    NEW0(e);

    e->left = x;
    e->top = y;
    e->right = x;
    e->bottom = y;

    return e;
}
//T        envelop_new_box(double left,double top,double right,double bottom);
void     envelop_free(T *e)
{
    assert(e && *e);
    FREE(*e);
}

/**
 *
 */
int      envelop_contains_point(T e,double x,double y)
{
    assert(e);

    if(x >= e->left && x <= e->right && y >= e->top && y <= e->bottom)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int      envelop_contains(T e,T target)
{
    assert(e);
    assert(target);
    if(target->left >= e->left && target->right <= e->right && target->top >= e->top && target->bottom <= e->bottom)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
double   envelop_distance_sq(T e,T target)
{
    T left,right,upper,lower;
    double xdiff,ydiff;

    assert(e);
    assert(target);

    if(envelop_intersect(e,target))
    {
        return 0;
    }

    //执行到此说明两个矩形不想交
    //求出最左边的矩形
    left = e->left < target->left ? e : target;
    //求出最右边的矩形
    right = e->right > target->right ? e : target;
    //求出最上边的矩形
    upper = e->top < target->top ? e : target;
    //求出最下边的举行
    lower = e->bottom > target->bottom ? e : target;

    xdiff = (left->left == right->left) ? 0 : right->left - left->right;
    if(xdiff < 0 )
    {
        xdiff *= -1;
    }

    ydiff = (upper->top == lower->top) ? 0 : lower->top - upper->bottom;
    if(ydiff < 0)
    {
        ydiff *= -1;
    }

    return xdiff * xdiff + ydiff * ydiff;
}
int      envelop_intersect(T e,T target)
{
    assert(e);
    assert(target);

    return envelop_intersect_box(e,target->left,target->top,target->right,target->bottom);
}
int      envelop_intersect_box(T e,double left,double top,double right,double bottom)
{
    assert(e);
    if(left > e->right || right < e->left || top > e->bottom || bottom < e->top)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void     envelop_set_left(T e,double left)
{
    assert(e);
    e->left = left;
}
void     envelop_set_top(T e,double top)
{
    assert(e);
    e->top = top;
}
void     envelop_set_right(T e,double right)
{
    assert(e);
    e->right = right;
}
void     envelop_set_bottom(T e,double bottom)
{
    assert(e);
    e->bottom = bottom;
}
double   envelop_get_left(T e)
{
    assert(e);
    return e->left;
}
double   envelop_get_top(T e)
{
    assert(e);
    return e->top;
}
double   envelop_get_right(T e)
{
    assert(e);
    return e->right;
}
double   envelop_get_bottom(T e)
{
    assert(e);
    return e->bottom;
}
double   envelop_get_width(T e)
{
    assert(e);
    return e->right - e->left;
}
double   envelop_get_height(T e)
{
    assert(e);
    return e->bottom - e->top;
}
double   envelop_get_center_x(T e)
{
    assert(e);
    return (e->left + e->right) / 2.0;
}
double   envelop_get_center_y(T e)
{
    assert(e);
    return (e->top + e->bottom) / 2.0;
}
