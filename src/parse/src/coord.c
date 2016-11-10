
#include <stdlib.h>
#include "mem.h"
#include "assert.h"
#include "coord.h"

#define T coord_t
struct T
{
    float x;
    float y;
};

T coord_new(float x,float y)
{
    T coord;
    NEW(coord);

    coord->x = x;
    coord->y = y;

    return coord;
}
float coord_get_x(T coord)
{
    assert(coord);
    return coord->x;
}
float coord_get_y(T coord)
{
    assert(coord);
    return coord->y;
}
