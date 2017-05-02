#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "makePPM.h"
#include<math.h>

/* Gets a color to represent an integer between 0-6 */
char* getPPMColor(char x){
  if(x == 0)
    return " 0 0 2 ";
  if(x == 1)
    return " 0 2 0 ";
  if(x == 2)
    return " 2 0 0 ";
  if(x == 3)
    return " 2 2 0 ";
  if(x == 4)
    return " 0 2 2 ";
  if(x == 5)
    return " 2 0 2 ";
  if(x == 6)
    return " 1 1 1 ";
  if(x == 7)
    return " 1 0 1 ";
  printf("ERR: no color #%d:\n",x);
  exit(1);
}


/* Prints to file an int matrix as a PPM visualisation.
Either in attractor/color mode where the input matrix element is a number enumerationg the root of the pixel
or convergens/black&white mode where the input matrix element is iteration number 0-10*/
void * runMakePPM(void * args){
  input_struct * input = args;
  char *fileRGB = (char*)malloc(30 * sizeof(char));
  char *fileBW = (char*)malloc(30 * sizeof(char));
  int maxColorValueRGB = 2;
  int maxColorValueBW = 9;
  sprintf(fileRGB,"newton_attractors_x%d.ppm",input->exponent);
  sprintf(fileBW,"newton_convergence_x%d.ppm",input->exponent);
  FILE *frgb = fopen(fileRGB,"w");
  FILE *fbw = fopen(fileBW,"w");
  if (fbw == NULL || frgb == NULL)
  {
    printf("Error opening files!\n");
    exit(1);
  }
  fprintf(frgb,"P3\n%ld %ld\n%d\n", input->size, input->size, maxColorValueRGB);
  fprintf(fbw,"P3\n%ld %ld\n%d\n", input->size, input->size, maxColorValueBW);
  int i,j;
  char x;
  char* str = malloc(8*sizeof(char));
  str[0] = ' ';
  str[2] = ' ';
  str[4] = ' ';
  str[6] = ' ';
  str[7] = '\0';
  j=0;
  while(1){
    pthread_mutex_lock(&input->mutex);
    input->currentWriteRow = j;
    //10 buffer rows to prevent writing uncalculated rows
    if(j > input->nextRowToDo-(25+input->nThreads) && input->nextRowToDo < input->size){
      pthread_mutex_unlock(&input->mutex);
      usleep(10000);
      //printf("writer caught up; sleeping\n");
      continue;
    }
    pthread_mutex_unlock(&input->mutex);

    for(i=0;i<input->size;i++){
      //fprintf(frgb,"%s", getPPMColor(input->attractor[input->size*j+i]));
      //x = input->nIterations[input->size*j+i];
      //fprintf(fbw," %d %d %d ", x,x,x);

      fwrite(getPPMColor(input->attractor[(input->size*j+i)%(input->blockrows * input->size)]),1,7,frgb);
      x = input->nIterations[(input->size*j+i)%(input->blockrows * input->size)]+48;
      str[1]=x;
      str[3]=x;
      str[5]=x;
      fwrite(str,1,7,fbw);

    }
    fprintf(frgb,"\n");
    fprintf(fbw,"\n");
    j++;
    if(j== input->size){
      break;
    }
  }
  fclose(frgb);
  fclose(fbw);
  free(fileRGB);
  free(fileBW);
  return 0;
}
