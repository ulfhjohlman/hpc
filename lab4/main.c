#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>

int main(int argc, char *argv[]){
    /* parse command line arguments */
    int w=10;
    int h=10;
    double i=100;
    int n=1;
    double d=0.1;
    int opt;
    while ((opt = getopt(argc, argv, "i:n:d:")) != -1) {
        switch (opt) {
            case 'i': i = (double) strtod(optarg,(char**)NULL); break;
            case 'n': n = (int) strtol(optarg,(char**)NULL,10); break;
            case 'd': d = (double) strtod(optarg,(char**)NULL); break;
            default:
                fprintf(stderr, "Usage: %s [width] [height] [-i][centerValue] [-d][diffusionConstant] [-n][nIterations]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    int index;
    w = strtol(argv[optind],(char**)NULL,10);
    h = strtol(argv[optind+1],(char**)NULL,10);
    printf("w: %d, h: %d i:%fl n: %d, d :%lf\n",w,h,i,n,d);



    return 0;
}
