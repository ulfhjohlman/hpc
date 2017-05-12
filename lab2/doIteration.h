#ifndef DOITERATION_HEADER
#define DOITERATION_HEADER
#include<complex.h>
#include<pthread.h>
#include <math.h>
#include<pthread.h>

double * getRoots(int root);

void * runPixelCalc(void * args);

void newtonIteration(double * z_re, double * z_im, int d, double * z_abs);

typedef struct{
  int blockrows;
  int * nIterations;
  int * attractor;
  double * roots;
  int exponent;
  long size;
  pthread_mutex_t mutex;
  int nextRowToDo;
  int currentWriteRow;
  int nThreads;
}input_struct;

#endif
