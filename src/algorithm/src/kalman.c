#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "matrix.h"

#include "kalman_test.h"
#include "kalman.h"


#define T kalmanState_t
struct T
{
	matrix_t A;
	matrix_t B;
	//matrix_t Uk;
	matrix_t Xk;
	matrix_t Pk;

	matrix_t H;
	matrix_t R;
	matrix_t Q;
	matrix_t I;

	matrix_t Yk;
};

T kalmanState_new()
{
	T kalmanState;

	NEW(kalmanState);
	kalmanState->A = matrix_new_by_value(1,1,1.0);
	kalmanState->B = matrix_new_by_value(1,1,1.0);
	//kalmanState->Uk = matrix_new(1,1);
	kalmanState->H = matrix_new_by_value(1,1,1.0);
	kalmanState->R = matrix_new_by_value(1,1,1.0);
	kalmanState->Q = matrix_new_by_value(1,1,10e-7);
	kalmanState->I = matrix_new_by_value(1,1,1.0);

	kalmanState->Xk = matrix_new(1,1);
	kalmanState->Pk = matrix_new(1,1);

	kalmanState->Yk = matrix_new(1,1);


	return kalmanState;
}
void kalmanState_init(T state)
{
	matrix_put(state->Xk,1,1,x1[0]);
	matrix_put(state->Pk,1,1,0.3);
	matrix_put(state->Yk,1,1,x1[1]);
}
void kalmanFilter_update(T state,FILE *fp)
{
	fprintf(fp,"%.2f %.2f %.2f\n", x1[0],x1[0],0.0);
	fprintf(fp,"%.2f %.2f %.2f\n", x1[1],x1[1],0.0);
	matrix_t prev1 = matrix_new(1,1);
	matrix_t prev2 = matrix_new(1,1);
	matrix_put(prev1,1,1,x1[0]);
	matrix_put(prev2,1,1,x1[1]);
	for(int i = 3;i < 92;i++)
	{
		matrix_t Uk = matrix_sub(prev2,prev1);
		printf("Uk:\n");
		matrix_print(Uk);
		matrix_t bu = matrix_mul_and_matrix(state->B,Uk);
		matrix_t ax = matrix_mul_and_matrix(state->A,state->Xk);
		matrix_t XkMid = matrix_add(ax,bu);
		printf("Xkmid:\n");
		matrix_print(XkMid);

		matrix_t temp = matrix_mul_and_matrix(state->A,state->Pk);
		matrix_t aT   = matrix_transpose(state->A);
		matrix_t xx   = matrix_mul_and_matrix(temp,aT);
		matrix_t PkMid = matrix_add(xx,state->Q);
		printf("PkMid:\n");
		matrix_print(PkMid);

		matrix_t temp2 = matrix_mul_and_matrix(state->H,PkMid);
		matrix_t hT	   = matrix_transpose(state->H);
		matrix_t temp3 = matrix_mul_and_matrix(temp2,hT);
		matrix_t temp4 = matrix_add(temp3,state->R);
		matrix_t temp5 = matrix_inverse(temp4);
		matrix_t temp6 = matrix_mul_and_matrix(PkMid,hT);
		matrix_t Kk 	= matrix_mul_and_matrix(temp6,temp5);
		printf("Kk:\n");
		matrix_print(Kk);


		matrix_t temp7 = matrix_mul_and_matrix(state->H,XkMid);
		matrix_put(state->Yk,1,1,x1[i]);
		matrix_t temp8 = matrix_sub(state->Yk,temp7);
		matrix_t temp9 = matrix_mul_and_matrix(Kk,temp8);
		state->Xk = matrix_add(XkMid,temp9);
		printf("Xk:\n");
		matrix_print(state->Xk);

		matrix_t temp10 = matrix_mul_and_matrix(Kk,state->H);
		matrix_t temp11 = matrix_sub(state->I,temp10);
		state->Pk = matrix_mul_and_matrix(temp11,PkMid);
		printf("Pk:\n");
		matrix_print(state->Pk);

		prev1 = matrix_copy(prev2);
		prev2 = matrix_copy(state->Xk);
		//
		printf("i = %d(%.2f,%.2f) Pk = %.2f\n", i,x1[i],matrix_get(state->Xk,1,1),matrix_get(state->Pk,1,1));
		// fprintf(fp,"%.2f %.2f %.2f\n", x1[i],matrix_get(state->Xk,1,1),matrix_get(state->Pk,1,1));

		printf("\n\n\n\n");
		//matrix_free();

	}
}
