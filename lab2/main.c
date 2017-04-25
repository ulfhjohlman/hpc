#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<getopt.h>

#include "makePPM.h"
#include "doIteration.h"

int main(int argc, char *argv[]){
  int exponent;
  int size;
  int nThreads;

/* parse command line arguments */
  int opt;
  while ((opt = getopt(argc, argv, "l:t:")) != -1) {
        switch (opt) {
        case 't': nThreads = (int) strtol(optarg,(char**)NULL,10); break;
        case 'l': size = (int) strtol(optarg,(char**)NULL,10); break;
        default:
            fprintf(stderr, "Usage: %s [-t][threads] [-l][rows&columns] [exponent d]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
  if(optind < argc)
    exponent = (int) strtol(argv[optind],(char**)NULL,10);

  /* stores number of iterations required before aborting computations */
  int * matrixIterations = malloc(size*size*sizeof(int));
  /* stores the enumeration of the root the element converges towards */
  int * matrixAttractor = malloc(size*size*sizeof(int));;

  /* gets roots of the equation f(x)=x^d-1 depending on chosen exponent d */
  double complex * roots = getRoots(exponent);


  pthread_t threads[nThreads];
  int i,start,stop;
  input_struct in_data[nThreads];


  for(i=0;i<nThreads;i++){
    in_data[i].nIterations = matrixIterations;
    in_data[i].attractor = matrixAttractor;
    in_data[i].roots = roots;
    in_data[i].exponent = exponent;
    in_data[i].size = size;
    in_data[i].start = i*size*size/nThreads;
    in_data[i].stop = (i+1)*size*size/nThreads-1;
    pthread_create(&threads[i], NULL ,runPixelCalc, &in_data[i]);
  }




  printf("waiting for threads\n");
  for(i=0;i<nThreads;i++){
    pthread_join(threads[i], NULL);
  }

/*
    for(int i =0 ; i< size; i++){
      for(int j =0 ; j< size; j++){
        printf("%d ",matrixAttractor[i+size*j]);
      }
      printf("\n");
    }
    printf("\n");
    for(int i =0 ; i< size; i++){
      for(int j =0 ; j< size; j++){
        printf("%d ",matrixIterations[i+size*j]);
      }
      printf("\n");
    }
*/
  printf("PRINTING\n");
  makePPM(size,matrixAttractor,RGB,exponent);
  makePPM(size,matrixIterations,BW,exponent);
  printf("DONE\n");
}
