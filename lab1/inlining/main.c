#include<stdio.h>
#include<complex.h>
#include<time.h>

void mul_cpx( double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im){
	double complex a = *a_re + *a_im * I;
	double complex b = *b_re + *b_im * I;
	double complex c;
	c = a*b;
	*c_re = creal(c);
	*c_im = cimag(c);
}

int main(){
	/*	testing mul_cpx & mul_cpx2
	double a_re = 3;
	double a_im = 4;
	double b_re = 3;
	double b_im = -4;
	double c_re;
	double c_im;
	mul_cpx(&a_re,&a_im,&b_re,&b_im,&c_re,&c_im);
	printf("Inline: (%f + %fi)(%f + %fi)=%f + %fi\n",a_re,a_im,b_re,b_im,c_re,c_im);
	mul_cpx2(&a_re,&a_im,&b_re,&b_im,&c_re,&c_im);
	printf("Non-Inline: (%f + %fi)(%f + %fi)=%f + %fi",a_re,a_im,b_re,b_im,c_re,c_im);
	*/
	int arrlength = 30000;
	double a_re_array[arrlength];
	double a_im_array[arrlength];
	double b_re_array[arrlength];
	double b_im_array[arrlength];
	double c_re_array[arrlength];
	double c_im_array[arrlength];
	srand(time(NULL));
	int i;
	for (i = 0; i<arrlength ; i++){
		b_re_array[i] = rand();
		b_im_array[i] = rand();
		c_re_array[i] = rand();
		c_im_array[i] = rand();
	}
	/*
	for (i =0; i<arrlength; i++){
		mul_cpx(&b_re_array[i],&b_im_array[i],&c_re_array[i],&c_im_array[i],&a_re_array[i],&a_im_array[i]);
	}
	*/
	/*
	for (i =0; i<arrlength; i++){
		mul_cpx2(&b_re_array[i],&b_im_array[i],&c_re_array[i],&c_im_array[i],&a_re_array[i],&a_im_array[i]);
	}
	*/
	
	for (i =0; i<arrlength; i++){
		double complex a;
		double complex b = b_re_array[i] + b_im_array[i] * I;
		double complex c = c_re_array[i] + c_im_array[i] * I;
		a = b*c;
		a_re_array[i] = creal(a);
		a_im_array[i] = cimag(a);
	}
}
