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

/* performs the described newton iteration procedure on a row-wise basis. Next
   row to calculate is determined by the input variable shared by all calculation
   threads and accessed by a mutex.
   Saves number of iterations and the attracting root to the input matrixes */
void  * runPixelCalc(void *args){
  input_struct * input = args;
  int d = input->exponent;
  int blockrows = input->blockrows;
  long size = (long)input->size;
  long currentPixel=0;
  double x;
  double y;
  double z_re;
  double z_im;
  int iter;
  while(1){
    if(currentPixel % size == 0){ //start a new row
      pthread_mutex_lock(&input->mutex);

      //buffer till writer, if calculations threaten to loop around and
      //catch up to the writer they sleep for 1ms.
      if(input->nextRowToDo >= blockrows +input->currentWriteRow){
          pthread_mutex_unlock(&input->mutex);
          usleep(1000);
          continue;
      }
      currentPixel = input->nextRowToDo * size;
      input->nextRowToDo++;

      pthread_mutex_unlock(&input->mutex);
      if(currentPixel >= (long)size*(long)size){
        return 0; //all rows done
      }

      /* ONLY COMPATIBLE IF BLOCKROWS = ALL ROWS
      //if we are past halfway (+buffer), make use of the conjugate's solution symmetry
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
    double z_abs = sqrt(z_re * z_re + z_im * z_im);
    iter=0;
    while(1){
      /* check exit conditions */
      if(z_abs < 0.001 || z_re > 10E10 || z_im > 10E10){
        input->attractor[currentPixel%(blockrows * size)] = d;//  'exponent d' is used as the enumeration for the bonus root for divergent x
        if(iter>9){
          iter=9;
        }
        input->nIterations[currentPixel%(blockrows * size)] = iter;
        goto NEXT_PIXEL;
      }
      for(int i=0;i<d;i++){
        //if sqrt( abs(z-root)) < 0.001 equivalent with  abs(z-root) < 0.000001
	       if( (z_re-input->roots[i*2])*(z_re-input->roots[i*2])+(z_im-input->roots[i*2+1])*(z_im-input->roots[i*2+1]) < 0.000001 ){
	          input->attractor[currentPixel%(blockrows * size)] = i;
            if(iter>9){ //cap iter at 9 for cleaner output
              iter=9;
            }
            input->nIterations[currentPixel%(blockrows * size)] = iter;
            goto NEXT_PIXEL;
          }
      }

      newtonIteration(&z_re,&z_im,d,&z_abs);
      iter++;
    }
    NEXT_PIXEL: currentPixel++;
  }
}

/* 1 newton iteration of the given function*/
void newtonIteration(double * z_re, double * z_im, int d, double * z_abs){
  if(d==1){
    *z_re = 1;
    *z_im = 0;
    *z_abs = 1;
    return;
  }

  if(d==2){
    double z_abs2 = *z_re*(*z_re) + *z_im*(*z_im);
    *z_im = *z_im/2 - *z_im/(2*z_abs2);
    *z_re = *z_re/2 + *z_re/(2*z_abs2);
    *z_abs = sqrt(*z_re * *z_re + *z_im * *z_im);
    return;
  }
  double z_arg = atan2(*z_im,*z_re);
  //double z_abs = sqrt(*z_re * *z_re + *z_im * *z_im);
  double zd1_abs;
  if(d==3){
    zd1_abs = *z_abs * *z_abs ;
  }
  else if(d==4){
    zd1_abs = *z_abs * *z_abs * *z_abs;
  }
  else if(d==5){
    zd1_abs = *z_abs * *z_abs;
    zd1_abs = zd1_abs * zd1_abs;
  }
  else if(d==6){
      zd1_abs = *z_abs * *z_abs;
      zd1_abs = zd1_abs * zd1_abs;
      zd1_abs = zd1_abs * *z_abs;
  }
  else if(d==7){
    zd1_abs = *z_abs * *z_abs;
    double tmp = zd1_abs * zd1_abs;
    zd1_abs = zd1_abs * tmp;
  }
  double zd1_arg = z_arg * (d-1);

  *z_re = *z_re * (d-1)/d + cos(zd1_arg)/(d*zd1_abs);
  *z_im = *z_im * (d-1)/d + sin(-zd1_arg)/(d*zd1_abs);
  *z_abs = sqrt(*z_re * *z_re + *z_im * *z_im);
  return;
}
