#include<stdio.h>
#include<stdlib.h>
#include "doIteration.h"

/* returns the roots to the equation f(x)=(x^exp)-1 */
double * getRoots(int exp){
	if(exp == 1){
		double static ret[2] = {1,0};
		return ret;
	} 
	if(exp == 2){
                double static ret[4] = {1,0,-1,0};
                return ret;
        }
	if(exp == 3){
                double static ret[6] = {1,0,-0.5,sqrt(3)/2,-0.5,sqrt(3)/(-2)};
                return ret;
        }
	//TODO 4-5-6-7
	exit(1);
}

/* takes one pixel and performs the described newton iteration procedure.
   Saves number of iterations and the attracting root to the input pointers */
void  * runPixelCalc(void *args){
  input_struct * input = args;
  int d = input->exponent;
  int currentPixel = input->start;
  do{
    double x = currentPixel % input->size;
    double y = (currentPixel - x)/input->size;
    double z_re = -2 + y* 4/(double)input->size;
    double z_im = 2 - x * 4/(double)input->size;
    int iter=0;
    double z_abs,z_arg,term2_arg,term2_abs,term3_abs,term3_arg,term2_re,term2_im;
    while(1){
      /* check exit conditions */
      for(int i=0;i<d;i++){
        //if(cabs(input->roots[i]-z) < 0.001){
	if( hypot(z_re-input->roots[i*2],z_im-input->roots[i*2+1]) ){
	  input->attractor[currentPixel] = i;
          input->nIterations[currentPixel] = iter;
          goto NEXT_PIXEL;
        }
      }
      if((z_re<0.0007 && z_im<0.0007) || z_re > 10E10 || z_im > 10E10){
        input->attractor[currentPixel] = d; /* 'exponent d' is used as the enumeration for the bonus root for divergent x */
        input->nIterations[currentPixel] = iter;
        goto NEXT_PIXEL;
      }
      /* 1 newton iteration */
      //z = z - (cpow(z,d)-1)/(d*cpow(z,d-1));
	z_abs = hypot(z_re,z_im);
	z_arg = atan2(z_re,z_im);

	term2_abs = pow(z_abs,d);
	term2_arg = z_arg * d;

	term2_re = term2_abs*cos(term2_arg)-1;
	term2_im = term2_abs*sin(term2_arg);	

	term3_abs = d*pow(z_abs,d-1);
	term3_arg = z_arg * (d-1);
	
	term2_abs = hypot(term2_re,term2_abs);
	term2_arg = atan2(term2_re,term2_abs);

	term2_abs = term2_abs/term3_abs;
	term2_arg = term2_arg - term3_arg;

	z_re = z_re - term2_abs*cos(term2_arg);
	z_im = z_im - term2_abs*sin(term2_arg);
      iter++;
    }
    NEXT_PIXEL: currentPixel++;
  } while(currentPixel <= input->stop);
  return 0;
}
