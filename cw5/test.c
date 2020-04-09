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
	MPI_Request * requests = malloc((processes -1) * sizeof(MPI_Request));
	MPI_Status * status = malloc((processes -1) * sizeof(MPI_Status));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("%d\n", rank);
	if(rank == 0){
		for(int i = 0; i < processes -1; i++){
			int x = i * i;
			printf("Wysyłam do %d\n", i);
			MPI_Send(&x, 1, MPI_INTEGER,i+1,0,
					MPI_COMM_WORLD);
		}
	}else{
		MPI_Request req;
		int get = -1;
		MPI_Irecv(&get,1, MPI_INTEGER,0 , MPI_ANY_TAG, MPI_COMM_WORLD, &req);
		printf("ulalalal liczymy cos\n");
		for(int i = 0; i < 1000; i++){
			sin(i);
		}
		printf("Dobra teraz możemy odebrać\n");
		MPI_Status s;
		MPI_Wait(&req, &s);
		printf("Dostałem %d\n", get);

	}
	printf("KOńczymy %d\n", rank);
	MPI_Finalize();
	return 0;
}

