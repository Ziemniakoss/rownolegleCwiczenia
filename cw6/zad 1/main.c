#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "nwd.h"


int main(int argc, char**argv){

MPI_Init(&argc, &argv);
int size, rank, value;

MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

int n = (int)log2(size);
if(pow(2,n) != size || size < 2){
	printf("ilość procesów nie może być mniejsza od 2 i musi być potęgą 2");
	MPI_Abort(MPI_COMM_WORLD, 10);
}

if(size != argc-1){
	printf("zła ilość argumentów");
	MPI_Abort(MPI_COMM_WORLD, 11);
}


value = atoi(argv[rank+1]);
	if(value <= 0){
		printf("Liczby muszą być większe od 0");
		MPI_Abort(MPI_COMM_WORLD, 12);
	}

int i;

for(i = 0; i < n; i++){
	int sendTo, reciveFrom, step, valueRecv;
	step = pow(2, i);
	sendTo = (rank + step) % size;
	reciveFrom = (size + rank - step) % size;

	MPI_Send(&value, 1, MPI_INT, sendTo, 0, MPI_COMM_WORLD);
	MPI_Recv(&valueRecv, 1, MPI_INT, reciveFrom, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	value = findNWD(value, valueRecv);
}

if(rank == 0){
	printf("NWD = %d\n", value);
}

MPI_Finalize();
return 0;
}
