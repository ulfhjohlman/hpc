#include<mpi.h>
#include<stdio.h>

int main(int argc, char * argv[]){	
	MPI_Init(&argc, &argv);
	int nmb_mpi_proc, mpi_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &nmb_mpi_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

	if(mpi_rank==0){
		printf("nmb of process: %d\n",nmb_mpi_proc);
		{
		int msg = 10;
		int len = 1;
		int dest_rank=1;
		int tag = 12;
		MPI_Send(&msg, len, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
		printf("MPI message sent from master: %d\n",msg);
		}
		{
		int msg;
		int max_len = 1;
		int src_rank = 1;
		int tag = 10;
		MPI_Status stat;
		MPI_Recv(&msg, max_len, MPI_INT, src_rank,tag,MPI_COMM_WORLD,&stat);
		printf("MPI message rec at master: %d\n",msg);
		}
	}
	else if(mpi_rank=1){
		int msg;
		MPI_Status stat;
		MPI_Recv(&msg,1,MPI_INT,0,12,MPI_COMM_WORLD,&stat);
		printf("MPI msg rec at worker: %d\n",msg);
		msg++;
		MPI_Send(&msg,1,MPI_INT,0,10,MPI_COMM_WORLD);
		printf("MPI sent from worker: %d\n",msg);
	}

	MPI_Finalize();
	return 0;
}
