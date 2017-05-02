#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "doIteration.h"

/* returns the roots to the equation f(x)=(x^exp)-1 in form [root1_re, root1_im, root2_re,...]*/
double * getRoots(int exp){
  double * ret;
  ret = malloc(sizeof(double)*exp*2);
  for(int i =0; i<exp; i++){
    ret[2*i] = cos(2*M_PI* i/exp );
    ret[2*i+1] = sin(2*M_PI* i/exp );
  }
	return ret;
}

/* performs the described newton iteration procedure on a row-wise basis.
   Saves number of iterations and the attracting root to the input matrixes */
void  * runPixelCalc(void *args){
  input_struct * input = args;
  int d = input->exponent;
  int blockrows = input->blockrows;
  int size = input->size;
  long currentPixel=0;
  double x;
  double y;
  double z_re;
  double z_im;
  int iter;
  while(1){
    if(currentPixel % size == 0){ //start a new row
      pthread_mutex_lock(&input->mutex);
      if(input->nextRowToDo >= blockrows +input->currentWriteRow){ //50 row buffer till writer
          pthread_mutex_unlock(&input->mutex);
          usleep(1000);
          //printf("calc pulling ahead; sleeping\n");
          continue;
      }
      currentPixel = input->nextRowToDo * size;
      input->nextRowToDo++;

      pthread_mutex_unlock(&input->mutex);
      if(currentPixel >= size*size){
        return 0; //all rows done
      }
      /*
      //if we are past halfway (+buffer), make use of the conjugate's solution
      if(currentPixel>= size*(size/2 +10+input->nThreads)){
        long conjugateToCurrentPixel = size*(size-1) - currentPixel;
        for(long k =0;k<size;k++){
          if(input->attractor[conjugateToCurrentPixel]==0 || input->attractor[conjugateToCurrentPixel]==d){
            input->attractor[currentPixel] = input->attractor[conjugateToCurrentPixel];
          }
          else{
            input->attractor[currentPixel] = d - input->attractor[conjugateToCurrentPixel];
          }
          input->nIterations[currentPixel] = input->nIterations[conjugateToCurrentPixel];
          currentPixel++;
          conjugateToCurrentPixel++;
        }
        continue;
      } */
    }
    x = currentPixel % size;
    y = (currentPixel - x)/size;
    z_re = -2 + x* 4/(double)size;
    z_im = 2 - y * 4/(double)size;
    iter=0;
    while(1){
      /* check exit conditions */
      for(int i=0;i<d;i++){
        //if(cabs(input->roots[i]-z) < 0.001){
	       if( hypot(z_re-input->roots[i*2],z_im-input->roots[i*2+1]) < 0.001 ){
	          input->attractor[currentPixel%(blockrows * size)] = i;
            if(iter>9){
              iter=9;
            }
            input->nIterations[currentPixel%(blockrows * size)] = (char)iter;
            goto NEXT_PIXEL;
          }
      }
      if((fabs(z_re)<0.0007 && fabs(z_im)<0.0007) || z_re > 10E10 || z_im > 10E10){
        input->attractor[currentPixel%(blockrows * size)] = d; /* 'exponent d' is used as the enumeration for the bonus root for divergent x */
        if(iter>9){
          iter=9;
        }
        input->nIterations[currentPixel%(blockrows * size)] = (char)iter;
        goto NEXT_PIXEL;
      }
      newtonIteration(&z_re,&z_im,d);
      iter++;
    }
    NEXT_PIXEL: currentPixel++;
  }
}

/* 1 newton iteration of the given function*/
void newtonIteration(double * z_re, double * z_im, int d){
  if(d==1){
    *z_re = 1;
    *z_im = 0;
    return;
  }

  if(d==2){
    double z_abs2 = *z_re*(*z_re) + *z_im*(*z_im);
    double tmp;
    tmp = *z_re/2 + *z_re/(2*z_abs2);
    *z_im = *z_im/2 - *z_im/(2*z_abs2);
    *z_re = tmp;
    return;
  }
  double z_arg = atan2(*z_im,*z_re);
  double z_abs = hypot(*z_re,*z_im);
  //double zd1_abs = pow(z_abs,d-1);
  double zd1_abs;
  if(d==3){
    zd1_abs = z_abs * z_abs ;
  }
  else if(d==4){
    zd1_abs = z_abs * z_abs * z_abs;
  }
  else if(d==5){
    zd1_abs = z_abs * z_abs * z_abs * z_abs;
  }
  else if(d==6){
    zd1_abs = z_abs * z_abs * z_abs * z_abs * z_abs;
  }
  else if(d==7){
    zd1_abs = z_abs * z_abs * z_abs * z_abs * z_abs * z_abs;
  }
  double zd1_arg = z_arg * (d-1);

  *z_re = *z_re * (d-1)/d + cos(zd1_arg)/(d*zd1_abs);
  *z_im = *z_im * (d-1)/d + sin(-zd1_arg)/(d*zd1_abs);

  return;
}
