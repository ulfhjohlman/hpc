#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int bcastEdges = 1;
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
	int	stop = atoi(argv[2]);
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
	//Crude parsing of graph info from the file name
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
/////////////////////////////////////////////////////////////////////////////
	//Read input graph and scatter to workers
	int * localEdges = malloc(2*sizeof(int)*nRespEdges);
	int * edges;
	if(mpi_rank == 0){
		//printf("d:%d v: %d, mw: %d\n",degree,nVert,maxW);
		int k=0;
		edges = malloc(2*nVert*degree*sizeof(int));
		while(fscanf(gFile,"%*d %d %d\n",&edges[k],&edges[k+1]) == 2){
			k+=2;
		}
	}
	MPI_Scatter(edges,nRespEdges*2,MPI_INT,localEdges,nRespEdges*2,MPI_INT,0,MPI_COMM_WORLD);

/////////////////////////////////////////////////////////////////////////////
	//Initialization of variables
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
	int localVertIndex;
	int localMinIndex = -1;
	int * visited = calloc(nRespVert,sizeof(int));
	struct {
		int val;
		int rank;
	} globalMin, localMin;
	localMin.val = 1000000000;
	localMin.rank = mpi_rank;
	int * globalMinIndex;
	int * currentEdges = malloc(sizeof(int)*degree*2);
	int rank_currentVert;
	if(mpi_rank==0){
		globalMinIndex = malloc(sizeof(int)*nmb_mpi_proc);
	}
	if(mpi_rank == rankOfVert(start)){
		localDistToVert[start%nRespVert]=0;
	}
/////////////////////////////////////////////////////////////////////////////
	//Run Dijkstras
	while(currentVert != stop){
		rank_currentVert = rankOfVert(currentVert);
		//mark the new node as visited, reset the local Min for the respective process, and find relevant outgoing edges
		if(rank_currentVert == mpi_rank){
			localVertIndex = currentVert%nRespVert;
			visited[localVertIndex]=1;
//			printf("visited: %d logged by rank %d at pos: %d\n",currentVert,mpi_rank,currentVert%nRespVert);
			distToCurrentVert = localDistToVert[localVertIndex];
			localMin.val = 1000000000;
			localMinIndex = -1;
			currentEdges = &localEdges[localVertIndex*degree*2];
		}
		//Broadcast outgoing edges and cost to currnet node being expanded
		MPI_Bcast(currentEdges,2*degree,MPI_INT,rank_currentVert,MPI_COMM_WORLD);
		MPI_Bcast(&distToCurrentVert,1,MPI_INT,rank_currentVert,MPI_COMM_WORLD);
		//let each process check locally for new shortest paths to their local nodes
		for(int i=0;i<degree;i++){
			if(currentEdges[i*2]/nRespVert == mpi_rank){
				to = currentEdges[i*2]%nRespVert;
				newDist = distToCurrentVert + currentEdges[i*2+1];
				if(newDist < localDistToVert[to]){
					localDistToVert[to] = newDist;
					localFromVert[to] = currentVert;
//					printf("Examining edge from  %d to %d at rank %d new min %d\n",currentVert,currentEdges[i*2],mpi_rank,newDist);
				}
			}
		}
		//Find each local min
		for(int i=0;i<nRespVert;i++){
			if(!visited[i] && localDistToVert[i] < localMin.val){
				localMin.val = localDistToVert[i];
				localMinIndex = nRespVert*mpi_rank+i;
			}
		}
		//reduce to global min
//		printf("Current local min for rank %d is %d at node %d\n",mpi_rank,localMin,localMinIndex);
		MPI_Reduce(&localMin,&globalMin,1,MPI_2INT,MPI_MINLOC,0,MPI_COMM_WORLD);
		MPI_Gather(&localMinIndex,1,MPI_INT, globalMinIndex,1,MPI_INT,0,MPI_COMM_WORLD);
		if(mpi_rank==0){
			currentVert = globalMinIndex[globalMin.rank];
//			printf("currentVert to expand: %d, with value: %d\n",currentVert,globalMin.val);
		}
		//broadcast the new node to expand
		MPI_Bcast(&currentVert,1,MPI_INT,0,MPI_COMM_WORLD);
		if(currentVert == -1){printf("ERROR: currentVert == -1 Abort\n");MPI_Finalize();return 1;}
	}
/////////////////////////////////////////////////////////////////////////////
	//Gather and build the final path
	int * globalFromVert;
	if(mpi_rank ==0){
		globalFromVert = malloc(sizeof(int)*nVert);
	}
	MPI_Gather(localFromVert,nRespVert,MPI_INT,globalFromVert,nRespVert,MPI_INT,0,MPI_COMM_WORLD);
	if(mpi_rank ==0){
		while(currentVert != start){
			if(currentVert == stop){
				printf("\nShortest path: %d",stop);
			}
			currentVert = globalFromVert[currentVert];
			printf(" <- %d",currentVert);
		}
		printf("\n\n");
	}

	//Print length of final path
	if(mpi_rank == rankOfVert(stop)){
		printf("Shortest path length: %d\n",localDistToVert[stop%nRespVert]);
	}


//	free(currentEdges); only a pointer for some processes
	free(localFromVert);
	free(localEdges);
	free(localDistToVert);
	if(mpi_rank == 0){
		free(edges);
		free(globalMinIndex);
		free(globalFromVert);
	}
	MPI_Finalize();
	return 0;
}
