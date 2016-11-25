#include <stdlib.h>
#include <math.h>

#include "assert.h"
#include "mem.h"
#include "vector2d.h"

#define T vector2d_t

//定义精度误差
const double eps = 1e-6;

/**
 * 构造向量
 * @param  start 起点
 * @param  end   终点
 * @return       向量的实例
 */
T   vector2d_new(point start,point end)
{
    T vec;
    NEW(vec);

    vec->x = end.x - start.x;
    vec->y = end.y - start.y;

    return vec;
}

/**
 * 求向量的模，即向量的长度
 * @param  vec2d 向量
 * @return       长度
 */
double   vector2d_length(T vec2d)
{
    assert(vec2d);

    return sqrt(vec2d->x * vec2d->x + vec2d->y * vec2d->y);
}
/**
 * 向量和
 * @param  vec2d1 向量
 * @param  vec2d2 向量
 * @return        返回量向量之和
 */
T        vector2d_add(T vec2d1,T vec2d2)
{
    T ret;
    NEW(ret);
    assert(vec2d1);
    assert(vec2d2);

    ret->x = vec2d1->x + vec2d2->x;
    ret->y = vec2d1->y + vec2d2->y;
    return ret;
}

/**
 * 向量减
 * @param  vec2d1 被减向量
 * @param  vec2d2 减向量
 * @return        差
 */
T        vector2d_sub(T vec2d1,T vec2d2)
{
        T ret;
        NEW(ret);

        ret->x = vec2d1->x - vec2d2->x;
        ret->y = vec2d1->y - vec2d2->y;

        return ret;
}

//向量缩放
T        vector2d_scale(T vec2d,double scale)
{
    assert(vec2d);
    T ret;
    NEW(ret);

    ret->x = vec2d->x * scale;
    ret->y = vec2d->y * scale;

    return ret;
}

/**
 * 以点位基准缩放向量
 * @param  vec2d 向量
 * @param  p     基准点
 * @param  scale 缩放因子
 * @return       新的向量
 */
T        vector2d_scale_base_point(T vec2d,point p,double scale)
{
    assert(vec2d);
    T ret;
    NEW(ret);

    ret->x = (vec2d->x - p.x) * scale;
    ret->y = (vec2d->y - p.y) * scale;

    return ret;
}

/**
 * 旋转，以p点位中心点逆时针旋转r弧度，返回新的向量
 * @param  vec2d 向量
 * @param  p     基准点
 * @param  r     旋转弧度
 * @return       [description]
 */
T        vector2d_rotate_base_point(T vec2d,point p,double r)
{
    return NULL;
}

/**
 * 向量点乘
 * @param  vec2d1 向量
 * @param  vec2d2 向量
 * @return        点乘结果向量
 */
double       vector2d_dotmul(T vec2d1,T vec2d2)
{
    return vec2d1->x * vec2d2->x + vec2d1->y * vec2d2->y;
}

//向量叉乘
double        vector2d_crossmul(T vec2d1,T vec2d2)
{
    assert(vec2d1);
    assert(vec2d2);

    return vec2d1->x * vec2d2->y - vec2d1->y * vec2d2->x;
}

/**
 * 点到线段的距离
 * @param  p     点
 * @param  start 线段的起点
 * @param  end   线段的终点
 * @param  nearp 点到线段的最近点
 * @return       点到线段的距离
 */
double   vector2d_projection_by_point(point p,point start,point end,point *nearp)
{
    T se = vector2d_new(start,end);
    T sp = vector2d_new(start,p);
    T ep = vector2d_new(end,p);

    //printf("se = (%.2f,%.2f)\n",se->x,se->y);
    //printf("sp = (%.2f,%.2f)\n",sp->x,sp->y);
    //printf("ep = (%.2f,%.2f)\n",ep->x,ep->y);
    double se_length = vector2d_length(se);
    double seMulsp = vector2d_dotmul(se,sp);

    //printf("seMulsp = %.2f\n",seMulsp);
    if(seMulsp < 0)
    {
        //printf("start near\n");
        nearp->x = start.x;
        nearp->y = start.y;
        return vector2d_length(sp);
    }

    if(vector2d_dotmul(sp,se) > se_length * se_length)
    {
        //printf("end near\n");
        nearp->x = end.x;
        nearp->y = end.y;
        return vector2d_length(ep);
    }

    double proj_model = vector2d_dotmul(sp,se) / se_length;

    //printf("se model(%.2f,%.2f) proj_model = %.2f\n",se->x / se_length,se->y / se_length,proj_model);
    nearp->x = start.x + se->x / se_length * proj_model;
    nearp->y = start.y + se->y / se_length * proj_model;

    T vec = vector2d_new(*nearp,p);
    return vector2d_length(vec);

}
