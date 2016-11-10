
#include <stdlib.h>
#include "mem.h"
#include "assert.h"
#include "color.h"

#define T color_t
struct T
{
    int red;
    int green;
    int blue;
    float alpha;
};

T color_new(int r,int g,int b,float a)
{
    T color;
    NEW0(color);

    //支持alpha为0-1
    assert(a >= 0 && a <= 1);

    color->red = r;
    color->green = g;
    color->blue = b;
    color->alpha = a;
    return color;
}
int color_get_red(T color)
{
    assert(color);
    return color->red;
}
int color_get_green(T color)
{
    assert(color);
    return color->green;
}
int color_get_blue(T color)
{
    assert(color);
    return color->blue;
}
float color_get_alpha(T color)
{
    assert(color);
    return color->alpha;
}
