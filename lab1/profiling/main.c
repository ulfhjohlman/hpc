#include<stdio.h>
#include<time.h>
#include<stdlib.h>

void row_sums(double * sums,const double ** matrix,size_t nrs,size_t ncs){
  size_t ix;
  for ( ix=0; ix < nrs; ++ix ) {
    double sum = 0;
    size_t jx;
    for (jx=0; jx < ncs; ++jx )
      sum += matrix[ix][jx];
    sums[ix] = sum;
  }
}

void col_sums(double * sums,const double ** matrix,size_t nrs,size_t ncs){
  size_t jx;
  for ( jx=0; jx < ncs; ++jx ) {
    double sum = 0;
    size_t ix;
    for ( ix=0; ix < nrs; ++ix )
      sum += matrix[ix][jx];
    sums[jx] = sum;
  }
}

/* a faster version of colsum due to locality. */
void col_sums2(double * sums,const double ** matrix,size_t nrs,size_t ncs){
  size_t jx;
  for ( jx=0; jx < ncs; ++jx )
    sums[jx]=0;
  size_t ix;
  for ( ix=0; ix < ncs; ++ix ) {
    for ( jx=0; jx < nrs; ++jx )
      sums[jx] += matrix[ix][jx];
  }
}

/* prints a matrix for debugging purposes*/
void printMatrix(double **a,int size){
  int i,j;
  for(i=0;i<size;i++){
    for(j=0;j<size;j++){
      printf(" %f ",a[i][j]);
    }
    printf("\n");
  }
}
int main(){
  int z;
  for(z=0;z<1000;z++){
  int size = 1000;
  double **a;
  double asdf = 2;
  srand(time(NULL));
  a = malloc(size * sizeof(*a));
  int i,j;
  for(i=0;i<size;i++){
    a[i] = malloc(size * sizeof(**a));
  }
  /*printMatrix(a,size);*/
  for (i=0; i<size; i++){
    for (j=0; j<size; j++){
      a[i][j] = rand();
    }
  }
  /*printMatrix(a,size);*/
  double rowSum[size];
  double colSum[size];
  double colSum2[size];
  row_sums(rowSum,(const double **)a,size,size);
  col_sums(colSum,(const double **)a,size,size);
  col_sums2(colSum2,(const double **)a,size,size);

  /* prints results for debugging*/
  /*
  printf("Row sums:\n");
  for(j=0;j<size;j++){
    printf(" %f \n",rowSum[j]);
  }
  
  printf("Col sums:\n");
  for(j=0;j<10;j++){
    printf(" %f ",colSum[j]);
  }
  
  printf("Col2 sums:\n");
  for(j=0;j<size;j++){
    printf(" %f ",colSum2[j]);
  }
  */
}
}
