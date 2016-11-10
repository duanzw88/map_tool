#ifndef _KALMAN_H
#define _KALMAN_H

/**
 * 卡尔曼滤波器
 */
#define T kalmanState_t
typedef struct T *T;

extern T    kalmanState_new();
extern void kalmanState_init(T filter);
extern void kalmanFilter_update(T state,FILE *fp);

#undef T
#endif
