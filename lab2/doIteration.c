#include<stdio.h>
#include<stdlib.h>
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
void  * runPixelCalc(void *args){
  input_struct * input = args;
  int currentPixel = input->start;
  do{
    int x = currentPixel % input->size;
    int y = (currentPixel - x)/input->size;
    double z_re = -2 + (double)y * 4/(double)input->size;
    double z_im = 2 - (double)x * 4/(double)input->size;
    double complex z = z_re + z_im *I;
    int iter=0;
    while(1){
      /* check exit conditions */
      for(int i=0;i<input->exponent;i++){
        if(cabs(input->roots[i]-z) < 0.001){
          input->attractor[currentPixel] = i;
          input->nIterations[currentPixel] = iter;
          goto NEXT_PIXEL;
        }
      }
      if(cabs(z)<0.001 || creal(z) > 10E10 || cimag(z) > 10E10){
        input->attractor[currentPixel] = input->exponent; /* 'exponent' is used as the enumeration for the bonus root for divergent x */
        input->nIterations[currentPixel] = iter;
        goto NEXT_PIXEL;
      }
      /* 1 newton iteration */
      z = z - (cpow(z,input->exponent)-1)/(input->exponent*cpow(z,input->exponent-1));
      iter++;
    }
    NEXT_PIXEL: currentPixel++;
  } while(currentPixel <= input->stop);
  return 0;
}
