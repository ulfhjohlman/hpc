#include<stdio.h>
#include<stdlib.h>
//#include<unistd.h>
#include<getopt.h>
#include<math.h>
#include<time.h>

int main(int argc, char *argv[]){
    /* parse command line arguments */
    unsigned int t=2;
    unsigned int n=0;
    int opt;
    while ((opt = getopt(argc, argv, "t:n:")) != -1) {
        switch (opt) {
            case 't': t = (unsigned int) strtol(optarg,(char**)NULL,10); break;
            case 'n': n = (unsigned int) strtol(optarg,(char**)NULL,10); break;
            default:
                fprintf(stderr, "Usage: %s [-t][threads]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    //make cells file
    if(n!=0){
        FILE * f =fopen("cells","w");
        if(f == NULL){
            printf("unable to create file");
            exit(1);
        }
        int l;
        srand(time(NULL));
        for(l=0;l<n;l++){
            float a = -10 + (float) (rand()) / ((float) (RAND_MAX/20));
            float b = -10 +(float) (rand()) / ((float) (RAND_MAX/20));
            float c = -10 + (float) (rand()) / ((float) (RAND_MAX/20));
            fprintf(f,"%.2f %.2f %.2F\n", a,b,c);
        }

        fclose(f);
    }


    FILE * inputFile;
    inputFile = fopen("cells", "r");
    if(inputFile == NULL){
        printf("file reading error\n");
        exit(1);
    }
    double * coord; //coordinates of all points
    coord = malloc(100000*3*sizeof(double));
    //float * coord; //coordinates of all points
    //coord = malloc(10e5*3*sizeof(float));
    int k=0;
    while(fscanf(inputFile,"%lf %lf %lf\n",&coord[k],&coord[k+1],&coord[k+2]) == 3){
    //while(fscanf(inputFile,"%f %f %f\n",&coord[k],&coord[k+1],&coord[k+2]) == 3){
        k+=3;
    }

    int * count; // holds count of distances between points in increments of 0.01
    int MAX_COUNT = 3465;// round_up(20 * sqrt(3) * 100)^2 ints (the max distance^2 possible is 34.65^2);
    count = calloc(MAX_COUNT,sizeof(int));

    double x,y,z;
    double dist;
    int i,j;
    #pragma omp parallel num_threads(t)
    {
        int * count_private= calloc(12006300, sizeof(int));
	    #pragma omp for schedule(dynamic) private(i,j,x,y,z,dist)
        for(i=0;i<k;i+=3){
            #pragma omp simd
	        for(j=i+3;j<k;j+=3){
                x = coord[i] - coord[j];
                y = coord[i+1] - coord[j+1];
                z = coord[i+2] - coord[j+2];
                //distance between 2 points rounded to hundredths, 12.232 -> 1223
                //dist = roundf((x*x+y*y+z*z)*100);
                dist = sqrt(x*x+y*y+z*z);
		        //printf("dist: %.2lf\n", (float)((int)roundf(dist*100))/100);
              count_private[(int) (dist*100)]++;
          }
        }
    	#pragma omp critical
    	{
    		for(int l=0;l<3465;l++){
    			count[l]+= count_private[l];
    		}
    	}
        free(count_private);
    }

    for(i=1;i<MAX_COUNT;i++){
        if(count[i] > 0){
            printf("%.2f %d\n",i*0.01,count[i]);
        }
    }
    fclose(inputFile);
    free(coord);
    free(count);
    return 0;
}
