#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

int main(int argc, char**argv){

MPI_Init(&argc, &argv);
int size, rank, rankRec, rankSend;

MPI_Comm_size(MPI_COMM_WORLD, &size);

if(size < 2){
	printf("ilość procesów nie może być mniejsza od 2");
	MPI_Abort(MPI_COMM_WORLD, 10);
}

int ndims[] = {size};
int periods[] = {false};
MPI_Comm cartComm;
MPI_Cart_create(MPI_COMM_WORLD, 1, ndims, periods, true, &cartComm);

MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Cart_shift(cartComm, 1, 1, &rankRec, &rankSend);

int vectorSize = 1000000, partSize = 1000;
int vector[vectorSize], part[partSize];
int i, j;
long sum = 0;

//zainicjowanie wektora
if(rank == 0){	
	for(i = 0; i < vectorSize; i++){
		vector[i] = i;	
	}
}

for(i = 0; i < vectorSize / partSize; i++){
	//utworzenie części
	if(rank == 0){	
		for(j = 0; j < partSize; j++){
			part[j] = vector[i * partSize + j];
		}
	}
	MPI_Recv(part, partSize, MPI_INT, rankRec, 0, cartComm, MPI_STATUS_IGNORE);
	
	//zapisanie części w wektorze
	if(rank != 0){	
		for(j = 0; j < partSize; j++){
			vector[i * partSize + j] = part[j];
		}
	}

	MPI_Send(part, partSize, MPI_INT, rankSend, 0, cartComm);
}

//policzenie sumy elementów
for(j = 0; j < vectorSize; j++){
	sum += vector[j];		
}
printf("rank: %d, sum: %ld, lastElement: %d \n", rank, sum, vector[vectorSize-1]);


MPI_Finalize();
return 0;
}
