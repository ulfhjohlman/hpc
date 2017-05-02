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
        case 'l': size = (long) strtol(optarg,(char**)NULL,10); break;
        default:
            fprintf(stderr, "Usage: %s [-t][threads] [-l][rows&columns] [exponent d]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
  if(optind < argc)
    exponent = (int) strtol(argv[optind],(char**)NULL,10);

  int blockrows = 100; //nRows in block
  /* stores number of iterations required before aborting computations */
  char * matrixIterations = malloc(blockrows*size*sizeof(char));
  /* stores the enumeration of the root the element converges towards */
  char * matrixAttractor = malloc(blockrows*size*sizeof(char));;
  if(matrixIterations == NULL || matrixAttractor == NULL){
    printf("Memmory allocation failure.\n");
  }
  /* gets roots of the equation f(x)=x^d-1 depending on chosen exponent d */
  double * roots = getRoots(exponent);

  pthread_t threads[nThreads];
  int i,start,stop;
  input_struct in_data;

  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex,NULL);

  in_data.blockrows = blockrows;
  in_data.nIterations = matrixIterations;
  in_data.attractor = matrixAttractor;
  in_data.roots = roots;
  in_data.exponent = exponent;
  in_data.size = size;
  in_data.mutex = mutex;
  in_data.nextRowToDo = 0;
  in_data.currentWriteRow = 0;
  in_data.nThreads = nThreads;
  for(i=0;i<nThreads;i++){
    pthread_create(&threads[i], NULL ,runPixelCalc, &in_data);
  }
  pthread_t writeThread;
  //pthread_create(&writeThread, NULL, runMakePPM,&in_data);
  pthread_create(&writeThread, NULL, runMakePPM,&in_data);
  printf("waiting for calculation threads\n");
  for(i=0;i<nThreads;i++){
    pthread_join(threads[i], NULL);
  }


  printf("waiting for file writer thread\n");
  pthread_join(writeThread,NULL);
  pthread_mutex_destroy(&mutex);
  free(matrixAttractor);
  free(matrixIterations);

  printf("DONE\n");
}
