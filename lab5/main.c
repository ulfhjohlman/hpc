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
	int start = atoi(argv[2]); //flip start and stop -> search backwards
	int stop = atoi(argv[1]);
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
	for(int i =0;i<nRespVert;i++){
		localDistToVert[i] = 1000000000;
	}
	int currentVert = start;
	int distToCurrentVert = 0;
	int to;
	int * localFromVert = malloc(sizeof(int)*nRespVert);
	for (int i=0;i<nRespVert;i++){
		localFromVert[i] = -1;
	}
	
	int newDist;
	
	int localMinIndex = -1;
	int * visited = calloc(nRespVert,sizeof(int));
	struct {
		int val;
		int rank;
	} globalMin, localMin;
	localMin.val = 1000000000;
	localMin.rank = mpi_rank;
	int * globalMinIndex;
	int tmpMin;
	int tmpMinIndex;
	int rank_currentVert;
	if(mpi_rank==0){
//		globalMin = malloc(sizeof(int)*nmb_mpi_proc);
		globalMinIndex = malloc(sizeof(int)*nmb_mpi_proc);	
	}
	if(mpi_rank == start/nRespVert){
		localDistToVert[start%nRespVert]=0;
	}
	MPI_Bcast(&currentVert,1,MPI_INT,0,MPI_COMM_WORLD);
//	int nVistited=0;
//	int k=0;
	while(currentVert != stop){
//	while(k<6){
		rank_currentVert = currentVert/nRespVert;	
		if(rank_currentVert == mpi_rank){
			visited[currentVert%nRespVert]=1;
//			printf("visited: %d logged by rank %d at pos: %d\n",currentVert,mpi_rank,currentVert%nRespVert);
			distToCurrentVert = localDistToVert[currentVert%nRespVert];
			localMin.val = 1000000000;
			localMinIndex = -1;
		}
		MPI_Bcast(&distToCurrentVert,1,MPI_INT,rank_currentVert,MPI_COMM_WORLD);
		for(int i=0;i<nRespEdges;i++){
			to = i/degree;
			//if(currentVert == localEdges[i*2] && !visited[to]){	
			if(currentVert == localEdges[i*2]){	
				newDist = distToCurrentVert + localEdges[i*2+1];
				if(localDistToVert[to] > newDist ){
					localDistToVert[to] = newDist;
//					printf("localFromVert[%d] rank %d changed to: %d from %d\n",to,mpi_rank,currentVert,localFromVert[to]);
					localFromVert[to] = currentVert;
				}
			}
		}
		for(int i=0;i<nRespVert;i++){
			if(!visited[i] && localDistToVert[i] < localMin.val){	
				localMin.val = localDistToVert[i];
				localMinIndex = nRespVert*mpi_rank+i;
			}
		}
//		printf("Current local min for rank %d is %d at node %d\n",mpi_rank,localMin,localMinIndex);
//		MPI_Gather(&localMin,1,MPI_INT, globalMin,1,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Reduce(&localMin,&globalMin,1,MPI_2INT,MPI_MINLOC,0,MPI_COMM_WORLD);
		MPI_Gather(&localMinIndex,1,MPI_INT, globalMinIndex,1,MPI_INT,0,MPI_COMM_WORLD);
		if(mpi_rank==0){
			currentVert = globalMinIndex[globalMin.rank];

/*			nVisited++;
			tmpMin = globalMin[0];
			tmpMinIndex = globalMinIndex[0]; 	
			for(int j=1;j<nmb_mpi_proc;j++){
				if(globalMin[j] < tmpMin){
					tmpMin = globalMin[j];
					tmpMinIndex = globalMinIndex[j];
				}
			}
			currentVert = tmpMinIndex;
*/
//			printf("currentVert to expand: %d\n",currentVert);
		}
//		MPI_Barrier(MPI_COMM_WORLD); k++;
		MPI_Bcast(&currentVert,1,MPI_INT,0,MPI_COMM_WORLD);	
	}
	
	int * globalFromVert;
	if(mpi_rank ==0){
		globalFromVert = malloc(sizeof(int)*nVert);
	}
	MPI_Gather(localFromVert,nRespVert,MPI_INT,globalFromVert,nRespVert,MPI_INT,0,MPI_COMM_WORLD);
	if(mpi_rank ==0){
//		printf("Visited a total of: %d nodes\n",nVisited);
		/*for(int i=0;i<nVert;i++){
			printf(" %d ",globalFromVert[i]);
		}*/
		while(currentVert != start){
			if(currentVert == stop){
				printf("\nShortest path: %d",stop);
			}
			currentVert = globalFromVert[currentVert];
			printf(" -> %d",currentVert);
		}
		printf("\n\n");
	}
	
	if(mpi_rank == rankOfVert(stop)){
		printf("Shortest path length: %d\n",localDistToVert[stop%nRespVert]);
	}
	
	free(localFromVert);
	free(localEdges);
	free(localDistToVert);
	if(mpi_rank == 0){
		free(edges);
//		free(globalMin);
		free(globalMinIndex);
	}
	MPI_Finalize();
	return 0;
}
