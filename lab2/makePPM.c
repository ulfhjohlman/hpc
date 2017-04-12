#include<stdio.h>
#include<stdlib.h>
#include "makePPM.h"

/* Gets a color to represent an integer between 0-6 */
char* getPPMColor(int x){
  if(x == 0)
    return "0 0 2";
  if(x == 1)
    return "0 2 0";
  if(x == 2)
    return "2 0 0";
  if(x == 3)
    return "2 2 0";
  if(x == 4)
    return "0 2 2";
  if(x == 5)
    return "2 0 2";
  if(x == 6)
    return "1 1 1";
  exit(3);
}

/* gets a shade of grey to represent an integer in range 0-10 */
char* getPPMBW(int x){
  char * str = (char*)malloc(6 * sizeof(char));
  sprintf(str,"%d %d %d",x,x,x);
  return str;
}

/* Prints to file an int matrix as a PPM visualisation.
Either in attractor/color mode where the input matrix element is a number enumerationg the root of the pixel
or convergens/black&white mode where the input matrix element is iteration number 0-10*/
void makePPM(int matrixSize, int** matrix,enum ppmMode mode,int d){
  char *fileName = (char*)malloc(30 * sizeof(char));
  int maxColorValue;
  if(mode == RGB){
    sprintf(fileName,"newton_attractors_x%d.ppm",d);
    maxColorValue = 2;
  }
  else if(mode == BW){
    sprintf(fileName,"newton_convergence_x%d.ppm",d);
    maxColorValue = 20;
  }
  else
  {
    printf("Invalid write mode. Nothing printed.\n");
    exit(2);
  }
  FILE *f = fopen(fileName,"w");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f,"P3\n%d %d\n%d\n", matrixSize, matrixSize, maxColorValue);
  int i,j;
  if(mode == RGB){
    for(i=0;i<matrixSize;i++){
      for(j=0;j<matrixSize;j++){
        fprintf(f," %s ", getPPMColor(matrix[i][j]));
      }
      fprintf(f,"\n");
    }
  }
  else if(mode == BW){
    for(i=0;i<matrixSize;i++){
      for(j=0;j<matrixSize;j++){
        fprintf(f," %s ", getPPMBW(matrix[i][j]));
      }
      fprintf(f,"\n");
    }
  }
  fclose(f);
  free(fileName);
  return;
}
