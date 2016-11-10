#ifndef _MATRIX_H
#define _MATRIX_H

#define T matrix_t
typedef struct T *T;

/*新建矩阵，矩阵中各值初始化为0.0*/
extern T 		matrix_new(int row,int col);
extern T        matrix_new_by_value(int row,int col, ...);
/*释放矩阵资源*/
extern void     matrix_free(T matrix);
/*打印矩阵*/
extern void 	matrix_print(T matrix);
/*获取矩阵的行数*/
extern int  	matrix_get_row(T matrix);
/*获取矩阵列数*/
extern int 		matrix_get_col(T matrix);
/*设置矩阵的值*/
extern int 	    matrix_put(T matrix,int row,int col,double value);
/*获取矩阵的一个值*/
extern double 	matrix_get(T matrix,int row,int col);
/*矩阵拷贝*/
extern T        matrix_copy(T matrix);
/*两个矩阵相加*/
extern T 		matrix_add(T matrix1,T matrix2);
/*两个矩阵相减matrix1-matrix2*/
extern T 		matrix_sub(T matrix1,T matrix2);
/*矩阵数乘*/
extern void     matrix_mul_and_number(T matrix,double num);
/*矩阵相乘*/
extern T        matrix_mul_and_matrix(T matrix1,T matrix2);
/*求矩阵的行列式*/
extern double   matrix_determinant(T matrix);
/*矩阵转置*/
extern T        matrix_transpose(T matrix);
/*矩阵逆运算*/
extern T        matrix_inverse(T matrix);
/*矩阵的缩放*/
extern T        matrix_scale(T matrix,double scalar);


#undef T
#endif
