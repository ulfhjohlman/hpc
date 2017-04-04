#include<stdio.h>
#include<complex.h>
#include "mul_cpx2.h"

void mul_cpx2( double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im){
	double complex a = *a_re + *a_im * I;
	double complex b = *b_re + *b_im * I;
	double complex c;
	c = a*b;
	*c_re = creal(c);
	*c_im = cimag(c);
}
