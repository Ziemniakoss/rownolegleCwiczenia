#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char ** argv){
	MPI_Init(&argc, &argv);
	int processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	if(processes < 2){
		printf("NUU\n");
		MPI_Finalize();
		return 0;
	}
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("%d\n", rank);
	if(rank == 0){
		MPI_Request * requests = malloc((processes -1) * sizeof(MPI_Request));
        	MPI_Status * status = malloc((processes -1) * sizeof(MPI_Status));

		int * results = malloc((processes -1) * sizeof(int));
		for(int i = 0; i < processes -1; i++){
			printf("Wysyłam do %d\n", i);
			MPI_Irecv(results + i, 1, MPI_INTEGER,i+1,0,
					MPI_COMM_WORLD, requests + i);
		}
		printf("Wysyłano, tera cos licze\n");
		for(int i = 0; i < 99999; i++)
			sin(i);
		printf("Dobra teraz możemy kurde odebrać\n");
		MPI_Waitall(processes-1, requests, status);
		printf("Otrzymałem\n");
		for(int i = 0; i < processes -1; i++){
			printf("liczba: %d\n",results[i]);
		}
	}else{
		MPI_Send(&rank, 1, MPI_INTEGER, 0,0,MPI_COMM_WORLD);
		printf("Wysyłam %d\n", rank);

	}
	printf("KOńczymy %d\n", rank);
	MPI_Finalize();
	return 0;
}

