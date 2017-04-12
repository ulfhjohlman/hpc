#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include "makePPM.h"
#include "doIteration.h"

int main(int argc, char *argv[]){
  int exponent;
  int size;
  int threads;

/* parse command line arguments */
  int opt;
  while ((opt = getopt(argc, argv, "l:t:")) != -1) {
        switch (opt) {
        case 't': threads = (int) strtol(optarg,(char**)NULL,10); break;
        case 'l': size = (int) strtol(optarg,(char**)NULL,10); break;
        default:
            fprintf(stderr, "Usage: %s [-t][threads] [-l][rows&columns] [exponent d]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
  if(optind < argc)
    exponent = (int) strtol(argv[optind],(char**)NULL,10);

  /* stores number of iterations required before aborting computations */
  int ** matrixIterations = malloc(size*sizeof(int*));
  /* stores the enumeration of the root the element converges towards */
  int ** matrixAttractor = malloc(size*sizeof(int*));;
  for(int i =0 ; i< size; i++){
    matrixIterations[i] = malloc(size*sizeof(int));
    matrixAttractor[i] = malloc(size*sizeof(int));
  }
  /* gets roots of the equation f(x)=x^d-1 depending on chosen exponent d */
  double complex * roots = getRoots(exponent);

  int i,j;
  for(i=0; i < size; i++){
    for(j=0;j < size; j++){
      calcPixel(&matrixIterations[i][j], &matrixAttractor[i][j], roots, exponent, size, i, j);
    }
  }



/*
  for(int i =0 ; i< size; i++){
    for(int j =0 ; j< size; j++){
      printf("%d ",matrixAttractor[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  for(int i =0 ; i< size; i++){
    for(int j =0 ; j< size; j++){
      printf("%d ",matrixIterations[i][j]);
    }
    printf("\n");
  }*/

  makePPM(size,matrixAttractor,RGB,exponent);
  makePPM(size,matrixIterations,BW,exponent);

}
