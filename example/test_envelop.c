
#include <stdio.h>
#include "envelop.h"

int main()
{
    envelop_t e1 = envelop_new(10,10,50,50);
    envelop_t ep1 = envelop_new_point(20,20);
    envelop_t e2 = envelop_new(20,20,30,30);    //e1包含e2
    envelop_t e3 = envelop_new(20,20,60,60);    //e1和e3相交
    envelop_t e4 = envelop_new(60,60,70,70);    //e1和e4不相交
    printf("测试点和矩形的关系\n");
    printf("e1 contains point (20,20) is %d\n",envelop_contains_point(e1,20,20));
    printf("e2 contains point (20,20) is %d\n",envelop_contains_point(e2,20,20));
    printf("e3 contains point (20,20) is %d\n",envelop_contains_point(e3,20,20));
    printf("e4 contains point (20,20) is %d\n",envelop_contains_point(e4,20,20));

    printf("测试矩形和矩形的包含关系\n");
    printf("e1 contains e2 is:%d\n",envelop_contains(e1,e2));
    printf("e1 contains e3 is:%d\n",envelop_contains(e1,e3));
    printf("e1 contains e4 is:%d\n",envelop_contains(e1,e4));

    printf("测试矩形和矩形的相交关系\n");
    printf("e1 intersect e2 is %d\n",envelop_intersect(e1,e2));
    printf("e1 intersect e3 is %d\n",envelop_intersect(e1,e3));
    printf("e1 intersect e4 is %d\n",envelop_intersect(e1,e4));

    printf("测试矩形和矩形的距离\n");
    printf("e1 intersect e2 is %lf\n",envelop_distance_sq(e1,e2));
    printf("e1 intersect e3 is %lf\n",envelop_distance_sq(e1,e3));
    printf("e1 intersect e4 is %lf\n",envelop_distance_sq(e1,e4));

    printf("矩形本身属性测试\n");
    printf("left:%.2lf\n",envelop_get_left(e1));
    printf("top:%.2lf\n",envelop_get_top(e1));
    printf("right:%.2lf\n",envelop_get_right(e1));
    printf("bottom:%.2lf\n",envelop_get_bottom(e1));
    printf("width:%.2lf\n",envelop_get_width(e1));
    printf("height:%.2lf\n",envelop_get_height(e1));
    printf("center_x:%.2lf\n",envelop_get_center_x(e1));
    printf("center_y:%.2lf\n",envelop_get_center_y(e1));
    return 0;
}
