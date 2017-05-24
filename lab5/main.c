#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int nRespEdges;
int nRespVert;

int rankOfVert(int vert){
	return vert/nRespVert;
}

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
	if(strcmp("example_graph",graphFile) ==0){
		degree=3;nVert=8;maxW=100;
	}
	else{
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
	}
	if(degree == 0 || nVert == 0 || maxW == 0 || gFile == NULL){
		printf("filereading or parsing error at rank: %d\nFile should be of the form XXX/graph_deX_neX_weX\n",mpi_rank);
		return 1;
	}
	if(nVert % nmb_mpi_proc != 0){
		printf("num of Verticies not evenly divisible by num of Proceses. Aborting.\n");
		return 1;
	}
	nRespVert = nVert/nmb_mpi_proc;
	nRespEdges = nRespVert * degree;
	
	int firstRespVert = mpi_rank * nRespVert;
	int lastRespVert = (mpi_rank+1) * nRespVert - 1;
	int * localEdges = malloc(2*sizeof(int)*nRespEdges);
	int * edges;	
	
	if(mpi_rank == 0){
		printf("d:%d v: %d, mw: %d\n",degree,nVert,maxW);
		int k=0;
		edges = malloc(2*nVert*degree*sizeof(int));
		while(fscanf(gFile,"%*d %d %d\n",&edges[k],&edges[k+1]) == 2){
			k+=2;
		}



	}
	MPI_Scatter(edges,nRespEdges*2,MPI_INT,localEdges,nRespEdges*2,MPI_INT,0,MPI_COMM_WORLD);

	int * localDistToVert = malloc(nRespVert*sizeof(int));
	int currentVert = start;
	int distToCurrentVert = 0;
	int to;
	int * localFromVert = malloc(sizeof(int)*nRespVert);
	int cost;
	int newDist;
	int localMin;
	int localMinIndex;
	int * visited = calloc(nRespVert,sizeof(int));
	int * globalMin;
	int * globalMinIndex;
	int tmpMin;
	int tmpMinIndex;
	if(mpi_rank==0){
		globalMin = malloc(sizeof(int)*nmb_mpi_proc);
		globalMinIndex = malloc(sizeof(int)*nmb_mpi_proc);	
	}

	while(currentVert != stop){
		//if(mpi_rank == rankOfVert(currentVert)){
		//	find currentVert -> neighbour_verts
		//	send_msg_update(neighbour_verts);
		//}
		//listen_msg();
		//currentVert = getNextVert();
		
		if(currentVert>firstRespVert && currentVert<lastRespVert){
			visited[currentVert%nRespVert];
		}
			
		MPI_Bcast(&currentVert,1,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Bcast(&distToCurrentVert,1,MPI_INT,0,MPI_COMM_WORLD);
		for(int i=0;i<nRespEdges;i++){
			if(currentVert == localEdges[i*2]){
				to = i/degree;
				cost = localEdges[i*2+1];
				newDist = distToCurrentVert + cost;
				if(localDistToVert[to] > newDist ){
					localDistToVert[to] = newDist;
					localFromVert[to] = currentVert;
				}
			}
		}
		localMin = localDistToVert[0];
		localMinIndex = nRespVert*nmb_mpi_proc;
		for(int i=0;i<nRespVert;i++){
			if(visited[i]){
				continue;
			}
			if(localDistToVert[i] < localMin){
				localMin = localDistToVert[i];
				localMinIndex = nRespVert*nmb_mpi_proc+i;
			}
		}
		MPI_Gather(&localMin,1,MPI_INT, &globalMin,1,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Gather(&localMinIndex,1,MPI_INT, &globalMinIndex,1,MPI_INT,0,MPI_COMM_WORLD);
		if(mpi_rank==0){
			tmpMin = globalMin[0];
			tmpMinIndex = globalMinIndex[0]; 	
			for(int j=1;j<nmb_mpi_proc;j++){
				if(globalMin[j] < tmpMin){
					tmpMin = globalMin[j];
					tmpMinIndex = globalMinIndex[j];
				}
			}
			currentVert = tmpMinIndex;
			printf("currentVert: %d\n",currentVert);
		}
	}

	free(localFromVert);
	free(localEdges);
	free(localDistToVert);
	if(mpi_rank == 0){
		free(edges);
		free(globalMin);
		free(globalMinIndex);
	}
	MPI_Finalize();
	return 0;
}
