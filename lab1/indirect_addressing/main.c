#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(){
  int n = 10;
  int m = 1000;
  int a = 3;

  /* generate vectors p,x & y */
  int *p;
  double *x,*y,*y2;
  p = malloc(n * sizeof(*p));
  x = malloc(n*sizeof(*x));
  y = malloc(n*sizeof(*y));
  srand(time(NULL));
  for( int ix=0; ix < n ;++ix ){
    p[ix] = ix;
    x[ix] = rand();
    y[ix] = rand();
  }

  int jx;
  /* indirect addressing */
  /*for (int kx=0; kx < n; ++kx){
    jx = p[kx];
    y[jx] += a*x[jx];
  }*/
  /* direct addressing */
  for (int kx=0; kx < n; ++kx){
    y[kx] += a*x[kx];
  }

}
