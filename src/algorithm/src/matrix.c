#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "assert.h"
#include "mem.h"
#include "matrix.h"

#define T matrix_t

struct T
{
	//矩阵的维度
	int row;	//矩阵的行数
	int col;	//矩阵的列数
	double *data;
};
/**
 * 新建矩阵，矩阵中各值初始化为0.0
 * @param  row 新矩阵的行数
 * @param  col 新矩阵的列数
 * @return     新的矩阵
 */
T 	matrix_new(int row,int col)
{
	T matrix;
	int i;
	assert(row > 0 && col > 0);

	// matrix = ALLOC(sizeof(matrix) + sizeof(double) * (row * col));
	// matrix->row = row;
	// matrix->col = col;
	// matrix->data = (double *)(matrix + 1);
	// for(i = 0;i < row * col;i++)
	// {
	// 	matrix->data[i] = 0.0;
	// }

	matrix = ALLOC(sizeof(matrix));
	// NEW(matrix);
	matrix->row = row;
	matrix->col = col;
	matrix->data = ALLOC(sizeof(double) * row * col);
	for(i = 0;i < row * col;i++)
	{
		matrix->data[i] = 0.0;
	}

	return matrix;
}
T matrix_new_by_value(int row,int col, ...)
{
	assert(row > 0 && col > 0);
	T matrix = matrix_new(row,col);
	va_list argPtr;

	va_start(argPtr,col);
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			double value = va_arg(argPtr,double);
			// printf("value = %f\n",value);
			matrix_put(matrix,i+1,j+1,value);
		}
	}
	va_end(argPtr);
	return matrix;
}

void     matrix_free(T matrix)
{
	assert(matrix);
	FREE(matrix->data);
	FREE(matrix);
}
/**
 * 打印矩阵
 * @param  matrix 目标矩阵
 * @return        无
 */
void	matrix_print(T matrix)
{
	int i;
	assert(matrix);
	for(i = 0; i < matrix->row * matrix->col;i++)
	{
		printf("%.4f\t",matrix->data[i]);
		if( (i+1) % matrix->col == 0 )
		{
			printf("\n");
		}

	}
	printf("\n");

}
/**
 * 获取矩阵的行数
 * @param  matrix 目标矩阵
 * @return        返回矩阵行数
 */
int  matrix_get_row(T matrix)
{
	assert(matrix);
	return matrix->row;
}
/**
 * 获取矩阵列数
 * @param  matrix 目标矩阵
 * @return        返回矩阵列数
 */
int 	matrix_get_col(T matrix)
{
	assert(matrix);
	return matrix->col;
}

/**
 * 设置矩阵的值
 * @param  matrix 目标矩阵
 * @param  row    新值所在的行
 * @param  col    新值所在的列
 * @param  value  新值
 * @return        1 新值添加成功
 *                0 新值添加失败
 */
int 	matrix_put(T matrix,int row,int col,double value)
{
	assert(matrix);
	assert(row > 0 && col > 0);
	assert(row <= matrix->row && col <= matrix->col);

	int index = (row-1) * matrix->col + (col-1);
	assert(index <= (matrix->row * matrix->col));
	matrix->data[index] = value;

	return 0;
}
/**
 * 获取矩阵的一个值
 * @param  matrix 目标矩阵
 * @param  row    要获取值所在的行
 * @param  col    要获取的值所在的列
 * @return        返回获取的值
 */
double 	matrix_get(T matrix,int row,int col)
{
//	printf("%s:%d matrix(%d %d)--->get(%d,%d)\n",__FILE__,__LINE__,matrix_get_row(matrix),matrix_get_col(matrix),row,col);
	assert(matrix);
	assert(row > 0 && col > 0);
	assert(row <= matrix->row && col <= matrix->col);

	int index = (row-1) * matrix->col + (col-1);
	return matrix->data[index];
}

T matrix_copy(T matrix)
{
	assert(matrix);
	T destMatrix = matrix_new(matrix->row,matrix->col);
	for(int i = 0; i <  matrix->row; i++)
	{
		for(int j = 0; j < matrix->col; j++)
		{
			double value = matrix_get(matrix,i+1,j+1);
			matrix_put(destMatrix,i+1,j+1,value);
		}
	}

	return destMatrix;
}
/**
 * 两个矩阵相加
 * @param  matrix1 目标矩阵1
 * @param  matrix2 目标矩阵2
 * @return         返回相加的结果矩阵
 */
T matrix_add(T matrix1,T matrix2)
{
	T matrix;
	int i,j;
	assert(matrix1 && matrix2);
	assert(matrix1->row ==  matrix2->row);
	assert(matrix1->col == matrix2->col);


	int row = matrix1->row;
	int col = matrix1->col;
	matrix = matrix_new(row,col);
	for(i = 1;i <= row;i++)
	{
		for(j = 1;j <= col;j++)
		{
			double value1 = matrix_get(matrix1,i,j);
			double value2 = matrix_get(matrix2,i,j);
			matrix_put(matrix,i,j,value1 + value2);
		}
	}

	return matrix;

}
/**
 * 两个矩阵相减matrix1-matrix2
 * @param  matrix1 被减矩阵
 * @param  matrix2 减数矩阵
 * @return         返回结果矩阵
 */
T 		matrix_sub(T matrix1,T matrix2)
{
	T matrix;
	int i,j;
	assert(matrix1 && matrix2);
	assert(matrix1->row ==  matrix2->row);
	assert(matrix1->col == matrix2->col);

	int row = matrix1->row;
	int col = matrix1->col;
	matrix = matrix_new(row,col);
	for(i = 1;i <= row;i++)
	{
		for(j = 1;j <= col;j++)
		{
			double value1 = matrix_get(matrix1,i,j);
			double value2 = matrix_get(matrix2,i,j);
			matrix_put(matrix,i,j,value1 - value2);
		}
	}

	return matrix;
}

/**
 * 矩阵数乘
 * @param  matrix 矩阵
 * @param  num    数
 * @return        结果矩阵
 */
void matrix_mul_and_number(T matrix,double num)
{
	int i,j;
	assert(matrix);
	int row = matrix_get_row(matrix);
	int col = matrix_get_col(matrix);
	for(i = 0;i < row; i++)
	{
		for(j = 0; j < col;j++)
		{
			matrix_put(matrix,i,j,matrix_get(matrix,i,j)*num);
		}
	}
}
/**
 * 矩阵相乘
 * @param  matrix1 矩阵1
 * @param  matrix2 矩阵2
 * @return         结果矩阵
 */
T matrix_mul_and_matrix(T matrix1,T matrix2)
{
	assert(matrix1);
	assert(matrix2);
	assert(matrix_get_col(matrix1) == matrix_get_row(matrix2));

	int newRow = matrix_get_row(matrix1);
	int newCol = matrix_get_col(matrix2);
	T matrix = matrix_new(newRow,newCol);
	int i,j,k;
	for(i = 1;i <= newRow;i++)
	{
		for(j = 1;j <= newCol;j++)
		{
			double value = 0.0;
			//printf("matrix1:%f\n",matrix_get(matrix1,i,j));
			for(k = 1;k <= matrix_get_col(matrix1);k++)
			{
				value += matrix_get(matrix1,i,k) * matrix_get(matrix2,k,j);
			}
			matrix_put(matrix,i,j,value);
		}
	}

	return matrix;
}
/**
 * 矩阵转置
 * @param  matrix 目标矩阵
 * @return        转置后的矩阵
 */
T matrix_transpose(T matrix)
{
	assert(matrix);
	T newMatrix = matrix_new(matrix_get_col(matrix),matrix_get_row(matrix));
	int i,j;
	for(i = 0;i <  matrix_get_row(newMatrix);i++)
	{
		for(j = 0;j < matrix_get_col(newMatrix);j++)
		{
			matrix_put(newMatrix,i+1,j+1,matrix_get(matrix,j+1,i+1));
		}
	}
	return newMatrix;
}
/**
 * 矩阵逆运算
 * @param  matrix 目标矩阵
 * @return        目标矩阵的逆矩阵
 */
T matrix_inverse(T matrix)
{
	assert(matrix);
	assert(matrix_get_col(matrix) == matrix_get_row(matrix));

	int i,j,k;
	int n;
	double max,temp;
	n = matrix_get_row(matrix);
	//定义临时矩阵
	matrix_t tmp = matrix_new(n,n);

	//将原矩阵存放在临时矩阵中
	for(i = 1;i <= n;i++)
	{
		for(j = 1;j <= n;j++)
		{
			matrix_put(tmp,i,j,matrix_get(matrix,i,j));
		}
	}
	// printf("临时矩阵:");
	// matrix_print(tmp);

	//初始化retMatrix为单位矩阵
	matrix_t matrixB = matrix_new(n,n);
	// printf("初始化retMatrix为单位矩阵\n");
	for(i=1;i<=n; i++)
	{
		for(j = 1;j<= n;j++)
		{
			double num = (i == j) ? 1.0 : 0.0;
			matrix_put(matrixB,i,j,num);
		}
	}
	// printf("单位矩阵\n");
	// matrix_print(matrixB);

	//高斯消元
	for(i = 1;i <= n;i++)
	{
		//寻找主元
		max = matrix_get(tmp,i,i);
		k = i;
		for(j= i+1;j<=n;j++)
		{
			if(fabs(matrix_get(tmp,j,i)) > fabs(max))
			{
				max = matrix_get(tmp,j,i);
				k = j;
			}
		}

		//判断主元是否在第i行，进行行交换
		if(k != i)
		{
			for(j = 1;j <= n;j++)
			{
				temp = matrix_get(tmp,i,j);
				matrix_put(tmp,i,j,matrix_get(tmp,k,j));
				matrix_put(tmp,k,j,temp);

				//b矩阵伴随交换
				//printf("n = %d i = %d j = %d k = %d\n",n,i,j,k);
				temp = matrix_get(matrixB,i,j);
				matrix_put(matrixB,i,j,matrix_get(matrixB,k,j));
				matrix_put(matrixB,k,j,temp);
			}
		}
		// printf("消元后的临时矩阵:");
		// matrix_print(tmp);
		//判断主元是否为0 如果为0则矩阵不是满秩矩阵 不存在逆矩阵
		if(matrix_get(tmp,i,i) == 0)
		{
			printf("主元为0\n");
			return NULL;
		}
		// printf("行交换以后的矩阵:\n");
		// matrix_print(tmp);
		// printf("--------\n");
		// matrix_print(matrixB);
		//消去A的第i列出去i行以外的各行元素
		temp = matrix_get(tmp,i,i);
		for(j = 1;j <= n;j++)
		{
			matrix_put(tmp,i,j,matrix_get(tmp,i,j) / temp);
			matrix_put(matrixB,i,j,matrix_get(matrixB,i,j) / temp);
		}

		for(j=1;j<=n;j++)
		{
			if(j != i)
			{
				temp = matrix_get(tmp,j,i);
				for(k=1;k<=n;k++)
				{
					matrix_put(tmp,j,k,matrix_get(tmp,j,k) - matrix_get(tmp,i,k) * temp);
					matrix_put(matrixB,j,k,matrix_get(matrixB,j,k) - matrix_get(matrixB,i,k) * temp);
				}
			}
		}

	}

	return matrixB;
}

T matrix_scale(T matrix,double scalar)
{
	assert(matrix);

	T destMatrix = matrix_new(matrix->row,matrix->col);

	for(int i = 0; i < matrix->row; i++)
	{
		for(int j = 0; j < matrix->col; j++)
		{
			double value = matrix_get(matrix,i+1,j+1) * scalar;
			matrix_put(destMatrix,i+1,j+1,value);
		}
	}

	return destMatrix;
}
