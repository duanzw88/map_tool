
#ifndef __COLOR_H__
#define __COLOR_H__

#define T color_t
typedef struct T *T;

extern T color_new(int r,int g,int b,float a);
extern int color_get_red(T color);
extern int color_get_green(T color);
extern int color_get_blue(T color);
extern float color_get_alpha(T color);

#undef T
#endif
