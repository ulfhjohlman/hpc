#ifndef DOITERATION_HEADER
#define DOITERATION_HEADER
#include<complex.h>
#include<pthread.h>

double complex * getRoots(int root);

void * runPixelCalc(void * args);

typedef struct{
  int * nIterations;
  int * attractor;
  double complex * roots;
  int exponent;
  int size;
  int start;
  int stop;
}input_struct;

#endif
