
#ifndef __COORD_H__
#define __COORD_H__

#define T coord_t
typedef struct T *T;

extern T coord_new(float x,float y);
extern float coord_get_x(T coord);
extern float coord_get_y(T coord);

#undef T
#endif
