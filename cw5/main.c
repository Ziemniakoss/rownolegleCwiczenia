#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "main.h"
#include "integrals.h"


/* 
 * Program liczący całkę z wykorzystaniem wielu procesów
 * komnikujących się przy pomocy protokołu MPI. Program musi 
 * mieć minium 2 procesy.
 *
 * Proces główny to proces o randze 0.
 *
 * Wywołanie:
 * 	main [start] [koniec] [liczba_czesci] [metoda]
 *
 * 	start         - początek całkowania(włącznie)
 * 	koniec        - koniec całkowania(włacznie)
 * 	liczba_czesci - na ile części podzielić obszar całkowania.
 *      metoda        - numer metody(opisane poniżej)		
 *
 * Obsługiwane metody
 * 	1 - prostokątóœ
 * 	2 - trapezów
 * 	3 - simpsona
 *
 */

double f(double x){
	return x * x;
}
int main(int argc, char ** argv){
	printf("%lf\n", rectangle(f, 0,1));
	
        MPI_Init(&argc, &argv);
        int rank = -1;
        int size = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size < 2){
		fprintf(stderr, "Potrzebne przynajmniej 2 rdzenie\n");
		MPI_Finalize();
		exit(1);
	}
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        printf("Rozmiar %d)\n", size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        printf("Ja jestem %d\n", rank);
        MPI_Finalize();
        return 0;
}

