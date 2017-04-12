#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "doIteration.h"

/* returns the roots to the equation f(x)=(x^exp)-1 */
double complex * getRoots(int exp){
  static double complex * ret;
  ret = malloc(exp * sizeof(double complex));
  double x;
  int i;
  for(i=0;i<exp;i++){
    x = i * 2 * M_PI / exp;
    ret[i] = cexp(I*x);
  }
  return ret;
}

/* takes one pixel and performs the described newton iteration procedure.
   Saves number of iterations and the attracting root to the input pointers */
void calcPixel(int* nIterations, int* attractor, double complex * roots, int exponent,int size, int i, int j){
  double x_re = -2 + (double)j * 4/(double)size;
  double x_im = 2 - (double)i * 4/(double)size;
  double complex x = x_re + x_im *I;
  int iter=0;

  while(1){
    /* check exit conditions */
    for(i=0;i<exponent;i++){
      if(cabs(roots[i]-x) < 0.001){
        *attractor = i;
        *nIterations = iter;
        return;
      }
    }
    if(cabs(x)<0.001 || creal(x) > 10E10 || cimag(x) > 10E10){
      *attractor = exponent; /* 'exponent' is used as the enumeration for the bonus root for divergent x */
      *nIterations = iter;
      return;
    }
    /* 1 newton iteration */
    x = x - (cpow(x,exponent)-1)/(exponent*cpow(x,exponent-1));
    iter++;
  }
}
