#ifndef DOITERATION_HEADER
#define DOITERATION_HEADER
#include<complex.h>

double complex * getRoots(int root);

void calcPixel(int* nIterations, int* attractor, double complex * roots,int exponent, int size, int i, int j);

#endif
