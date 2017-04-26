#include<stdio.h>
#include<stdlib.h>
#include "doIteration.h"

/* returns the roots to the equation f(x)=(x^exp)-1 */
double * getRoots(int exp){
  double * ret;
  ret = malloc(sizeof(double)*exp*2);
  for(int i =0; i<exp; i++){
    ret[2*i] = cos(2*M_PI* i/exp );
    ret[2*i+1] = sin(2*M_PI* i/exp );
  }
	return ret;
}

/* takes one pixel and performs the described newton iteration procedure.
   Saves number of iterations and the attracting root to the input pointers */
void  * runPixelCalc(void *args){
  input_struct * input = args;
  int d = input->exponent;
  int currentPixel = input->id*input->size;
  do{
    double x = currentPixel % input->size;
    double y = (currentPixel - x)/input->size;
    double z_re = -2 + y* 4/(double)input->size;
    double z_im = 2 - x * 4/(double)input->size;
    int iter=0;
    while(1){
      /* check exit conditions */
      for(int i=0;i<d;i++){
        //if(cabs(input->roots[i]-z) < 0.001){
	       if( hypot(z_re-input->roots[i*2],z_im-input->roots[i*2+1]) < 0.001 ){
	          input->attractor[currentPixel] = i;
            input->nIterations[currentPixel] = iter;
            goto NEXT_PIXEL;
          }
      }
      if((fabs(z_re)<0.0007 && fabs(z_im)<0.0007) || z_re > 10E10 || z_im > 10E10){
        input->attractor[currentPixel] = d; /* 'exponent d' is used as the enumeration for the bonus root for divergent x */
        input->nIterations[currentPixel] = iter;
        goto NEXT_PIXEL;
      }
      newtonIteration(&z_re,&z_im,d);
      iter++;
    }
    NEXT_PIXEL: currentPixel++;
    if(currentPixel % input->size == 0){ //start a new row
      currentPixel += (input->nThreads-1) * input->size;
    }
  } while(currentPixel <= input->stop);
  return 0;
}

/* 1 newton iteration of the given function*/
void newtonIteration(double * z_re, double * z_im,int d){
  //z = z - (cpow(z,d)-1)/(d*cpow(z,d-1));
  double z_abs = hypot(*z_re,*z_im);
  double z_arg = atan2(*z_im,*z_re);

  double numerator_abs = pow(z_abs,d);
  double numerator_arg = z_arg * d;

  double numerator_re = numerator_abs*cos(numerator_arg)-1;
  double numerator_im = numerator_abs*sin(numerator_arg);

  numerator_abs = hypot(numerator_re,numerator_im);
  numerator_arg = atan2(numerator_im,numerator_re);

  double denominator_abs = d*pow(z_abs,d-1);
  double denominator_arg = z_arg * (d-1);

  numerator_abs = numerator_abs/denominator_abs;
  numerator_arg = numerator_arg - denominator_arg;

  *z_re = *z_re - numerator_abs*cos(numerator_arg);
  *z_im = *z_im - numerator_abs*sin(numerator_arg);
  return;
}
