#ifndef DOITERATION_HEADER
#define DOITERATION_HEADER
#include<complex.h>
#include<pthread.h>
#include <math.h>

double * getRoots(int root);

void * runPixelCalc(void * args);

void newtonIteration(double * z_re, double * z_im, int d);

typedef struct{
  int * nIterations;
  int * attractor;
  double * roots;
  int exponent;
  int size;
  int start;
  int stop;
}input_struct;

#endif
