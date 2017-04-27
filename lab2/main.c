#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<getopt.h>
#include<semaphore.h>

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
  double * roots = getRoots(exponent);

  pthread_t threads[nThreads];
  int i,start,stop;
  input_struct in_data;

  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex,NULL);

  in_data.nIterations = matrixIterations;
  in_data.attractor = matrixAttractor;
  in_data.roots = roots;
  in_data.exponent = exponent;
  in_data.size = size;
  in_data.mutex = mutex;
  in_data.nextRowToDo = 0;
  in_data.nThreads = nThreads;
  for(i=0;i<nThreads;i++){
    pthread_create(&threads[i], NULL ,runPixelCalc, &in_data);
  }
  pthread_t writeThread;
  pthread_create(&writeThread, NULL, runMakePPM,&in_data);

  printf("waiting for calculation threads\n");
  for(i=0;i<nThreads;i++){
    pthread_join(threads[i], NULL);
  }
  printf("waiting for file writer thread\n");
  pthread_mutex_destroy(&mutex);
  pthread_join(writeThread,NULL);
  free(matrixAttractor);
  free(matrixIterations);
  printf("DONE\n");
}
