/**
 * 向量接口
 * 其实可以把向量看做是一段有向的线段
 */

#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#define T vector2d_t
typedef struct T *T;

//辅助定义point结构
struct T
{
    double x;
    double y;
};

typedef struct point
{
    double x;
    double y;
}point;

//线段
typedef struct segment
{
    point s;
    point e;
}segment;

/**
 * 构造向量
 * @param  start 起点
 * @param  end   终点
 * @return       向量的实例
 */
extern T        vector2d_new(point start,point end);
/**
 * 求向量的模，即向量的长度
 * @param  vec2d 向量
 * @return       长度
 */
extern double   vector2d_length(T vec2d);
/**
 * 向量和
 * @param  vec2d1 向量
 * @param  vec2d2 向量
 * @return        返回量向量之和
 */
extern T        vector2d_add(T vec2d1,T vec2d2);
/**
 * 向量减
 * @param  vec2d1 被减向量
 * @param  vec2d2 减向量
 * @return        差
 */
extern T        vector2d_sub(T vec2d1,T vec2d2);
/**
 * 向量缩放
 * @param  vec2d 向量
 * @param  scale 缩放因子
 * @return       返回新的向量
 */
extern T        vector2d_scale(T vec2d,double scale);
//旋转，以p点位中心点逆时针旋转r弧度，返回新的向量
/**
 * 旋转，以p点位中心点逆时针旋转r弧度，返回新的向量
 * @param  vec2d 向量
 * @param  p     基准点
 * @param  r     旋转弧度
 * @return       [description]
 */
extern T        vector2d_rotate_base_point(T vec2d,point p,double r);
/**
 * 以点位基准缩放向量
 * @param  vec2d 向量
 * @param  p     基准点
 * @param  scale 缩放因子
 * @return       新的向量
 */
extern T        vector2d_scale_base_point(T vec2d,point p,double scale);

/**
 * 向量点乘
 * @param  vec2d1 向量
 * @param  vec2d2 向量
 * @return        点乘结果
 */
extern double        vector2d_dotmul(T vec2d1,T vec2d2);
//向量叉乘
extern double       vector2d_crossmul(T vec2d1,T vec2d2);


/**
 * 点到线段的距离
 * @param  p     点
 * @param  start 线段的起点
 * @param  end   线段的终点
 * @param  vertp 点到线段的最近点
 * @return       点到线段的距离
 */
extern double   vector2d_projection_by_point(point p,point start,point end,point *nearp);
// //向量是否平行
// extern int      vector2d_is_paraell(T vec2d1,T vec2d2);
// //向量是否垂直
// extern int      vector2d_is_vertical(T vec2d1,T vec2d2);
////////////////////////////////
#undef T
#endif
