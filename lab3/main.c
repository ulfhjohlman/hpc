#include<stdio.h>
#include<stdlib.h>
//#include<unistd.h>
#include<getopt.h>
#include<math.h>
#include<time.h>

void scan(FILE * inputFile,double * coord, int *r){
    int k=0;
    while(fscanf(inputFile,"%lf %lf %lf\n",&coord[k],&coord[k+1],&coord[k+2]) == 3){
    //while(fscanf(inputFile,"%f %f %f\n",&coord[k],&coord[k+1],&coord[k+2]) == 3){
        k+=3;
    }
    *r = k;
    return;
}

void compute(double * coord, int * count,int * low_count,int k,int t){
    double x,y,z;
    double dist;
    int i,j;
    #pragma omp parallel num_threads(t)
    {
        int * count_private= calloc(120063, sizeof(int));
	    int * low_count_private = calloc(100,sizeof(int));
        #pragma omp for schedule(dynamic) private(i,j,x,y,z,dist)
        for(i=0;i<k;i+=3){
            #pragma omp simd
	    for(j=i+3;j<k;j+=3){
                x = coord[i] - coord[j];
                y = coord[i+1] - coord[j+1];
                z = coord[i+2] - coord[j+2];
                //distance^2 between 2 points rounded to hundredths, 12.232 -> 1223
                //dist = roundf((x*x+y*y+z*z)*100);
                dist = (x*x+y*y+z*z)*100;
		//printf("dist: %.2lf\n", (float)((int)roundf(dist*100))/100);
		//#pragma omp atomic
                //count[(int) dist]++;
		if(dist<100){
			low_count_private[ (int)(sqrt(dist/100)*100)]++;
		}
        else{
		      count_private[(int) dist]++;
        }
            }
        }
	#pragma omp critical
	{
		for(int l=0;l<120063;l++){
			count[l]+= count_private[l];
		}
		for(int l=0;l<100;l++){
			low_count[l]+=low_count_private[l];
		}
	}
    }
    return;
}

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

    int * count; // holds count of distances between points in increments of 0.01
    int * low_count;
    int MAX_COUNT = 120063;// round_up(20 * sqrt(3) * 100)^2 ints (the max distance^2 possible is 34.65^2);
    count = calloc(MAX_COUNT,sizeof(int));
    low_count = calloc(100,sizeof(int));
    int k;
    scan(inputFile,coord,&k);


    int i,j;
    compute(coord,count,low_count,k,t);


    for(i=0;i<100;i++){
        if(low_count[i]>0){
            printf("%.2f %d\n", i*0.01 , low_count[i]);
        }
    }
    int c=0;
    j=101;
    double sqrtBin = (int)( (j / 100.0)*(j / 100.0)*100.0);
    for(i=100;i<MAX_COUNT;i++){
        if(count[i]>0){
        //printf("uBin: %.2lf , i: %d, j: %d, count[i]: %d\n",sqrtBin,i,j,count[i]);
        }
        if( i < sqrtBin){
            c+=count[i];
        }else
        {
            if(c>0){
                printf("%.2f %d\n",(j-1)*0.01 , c);
                c=0;
            }
            j++;
            sqrtBin = (int)( (j / 100.0)*(j / 100.0)*100.0);
            c+=count[i];
        }
    }
    fclose(inputFile);
    free(coord);
    free(count);
    return 0;
}
