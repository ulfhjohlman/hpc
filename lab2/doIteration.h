#ifndef DOITERATION_HEADER
#define DOITERATION_HEADER
#include<complex.h>
#include<pthread.h>
#include <math.h>
#include<pthread.h>

double * getRoots(int root);

void * runPixelCalc(void * args);

void newtonIteration(double * z_re, double * z_im,int d, double * numerator_abs,
   double * numerator_arg, double * denominator_abs, double * denominator_arg,
    double * numerator_re, double * numerator_im, double * z_abs, double * z_arg);

typedef struct{
  int * nIterations;
  int * attractor;
  double * roots;
  int exponent;
  int size;
  pthread_mutex_t mutex;
  int nextRowToDo;
  int nThreads;
}input_struct;

#endif
