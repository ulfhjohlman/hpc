#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(int argc, char * argv[]){	
	MPI_Init(&argc, &argv);
	int nmb_mpi_proc, mpi_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &nmb_mpi_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	if(argc != 4){
		printf("wrong number of arguments. Aborting");
		return 1;
	}
	int start = atoi(argv[1]);
	int stop = atoi(argv[2]);
	char * graphFile = argv[3];
	FILE * gFile = fopen(graphFile,"r");

	char * p = graphFile;
	int degree = 0;
	int nVert = 0;
	int maxW = 0;
	while(*p){
		if(*p == '_'){
			p++;
			if(*p == 'd'){
				p++;
				if(*p== 'e'){
					p++;
					degree = pow(10,(int)*p-48);
				}
			} else if(*p == 'n'){
				p++;
				if(*p == 'e'){
					p++;
					nVert = pow(10,(int)*p-48);
				}
			} else if(*p == 'w'){
				p++;
				if(*p == 'e'){
					p++;	
					maxW = pow(10,(int)*p-48);
				}
			}
		} else{p++;}
	}
	if(degree == 0 || nVert == 0 || maxW == 0 || gFile == NULL){
		printf("filereading or parsing error at rank: %d\nFile should be of the form XXX/graph_deX_neX_weX\n",mpi_rank);
		return 1;
	}

	

	if(mpi_rank == 0){
		printf("d:%d v: %d, mw: %d",degree,nVert,maxW);
	
	}else{



	}


	MPI_Finalize();
	return 0;
}
