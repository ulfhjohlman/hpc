#ifndef DOITERATION_HEADER
#define DOITERATION_HEADER
#include<complex.h>
#include<pthread.h>
#include <math.h>
#include<pthread.h>

double * getRoots(int root);

void * runPixelCalc(void * args);

void newtonIteration(double * z_re, double * z_im, int d);

typedef struct{
  char * nIterations;
  char * attractor;
  double * roots;
  int exponent;
  int size;
  pthread_mutex_t mutex;
  int nextRowToDo;
  int nThreads;
}input_struct;

#endif
