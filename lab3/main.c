#include<stdio.h>
#include<stdlib.h>
//#include<unistd.h>
#include<getopt.h>
#include<math.h>

int main(int argc, char *argv[]){
    /* parse command line arguments */
    unsigned int t;
    int opt;
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
            case 't': t = (unsigned int) strtol(optarg,(char**)NULL,10); break;
            default:
                fprintf(stderr, "Usage: %s [-t][threads]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    FILE * inputFile;
    inputFile = fopen("cells.txt", "r");
    if(inputFile == NULL){
        printf("file reading error\n");
        exit(1);
    }
    double * coord; //coordinates of all points
    coord = malloc(10000*3*sizeof(double));
    //float * coord; //coordinates of all points
    //coord = malloc(10e5*3*sizeof(float));

    char * count; // holds count of distances between points in increments of 0.01
    int MAX_COUNT = 3465;// round_up(20 * sqrt(3) * 100) chars (the max distance possible is 34.65);
    count = malloc(MAX_COUNT*sizeof(char));
    int k = 0;
    int n; //num of points
    while(fscanf(inputFile,"%lf %lf %lf\n",&coord[k],&coord[k+1],&coord[k+2]) == 3){
    //while(fscanf(inputFile,"%f %f %f\n",&coord[k],&coord[k+1],&coord[k+2]) == 3){
        k+=3;
    }
    n = k/3;

    double x,y,z;
    double dist;
    int i;
    for(i=0;i<k;i+=3){
        for(int j=i+3;j<k;j+=3){
            x = coord[i] - coord[j];
            y = coord[i+1] - coord[j+1];
            z = coord[i+2] - coord[j+2];
            //distance between 2 points rounded to hundredths, 12.232 -> 1223
            dist = sqrt(x*x+y*y+z*z);
            //printf("dist: %lf, (int)dist: %d, roundf: %d\n",dist,(int)dist,(int)roundf(dist));
            count[(int) roundf(dist*100)]++;
        }
    }
    for(i=0;i<MAX_COUNT;i++){
        if(count[i]>0){
            printf("%.2f %d\n",i*0.01 , count[i]);
        }
    }
    fclose(inputFile);
    free(coord);
    free(count);
    return 0;
}
