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
 * Komunikacja
 * 	 
 *
 */
int main(int argc, char ** argv){	
        MPI_Init(&argc, &argv);
        int rank = -1;
        int size = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size < 2){
		fprintf(stderr, "Potrzebne przynajmniej 2 rdzenie\n");
		MPI_Finalize();
		exit(1);
	}
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        printf("Ja jestem %d\n", rank);
	if(rank == 0){
		double start, end;
		int parts, method;
		if(argc != 5){
			fprintf(stderr, "%s [start] [koniec] [liczba_przedziałow] [metoda]\n", argv[0]);
			fprintf(stderr, "Metody:\n\t1 - prostokątów\n\t2 - trapezów\n\t3- simpsona\n");
			MPI_Finalize();
			exit(2);
		}
		if(scanf(argv[1], "%lf", &start) != 1){
			fprintf(stderr, "Nie udało sie wczytać początku przedziału\n");
			MPI_Finalize();
			exit(3);
		}
		if(scanf(argv[2], "%lf", &end) != 1){
			fprintf(stderr, "Nie udało sie wczytać końca przedziału\n");
			MPI_Finalize();
			exit(4);
		}
		if(end > start){
			fprintf(stderr, "Zły przedział\n");
			MPI_Finalize();
			exit(5);
		}
		if(scanf(argv[3], "%d", &parts) != 1){
			fprintf(stderr, "Nie udało sie wczytać ilości przedziałów\n");
			MPI_Finalize();
			exit(6);
		}
		if(parts < 1){
			fprintf(stderr, "Liczba części musi byc większa od zera\n");
			MPI_Finalize();
			exit(7);
		}
		if(scanf(argv[4], "%d", &method) != 1){
			fprintf(stderr, "Nie udało się wczytać kodu metody\n");
	      		MPI_Finalize();
			exit(8);
	  	}
		if(method != RECTANGLE && method != TRAPEZOID && method != SIMPSON){
			fprintf(stderr, "Nieznana metoda\n");
			MPI_Finalize();
			exit(9);
		}
				

		printf("%d parametrów\n", argc);
		
		//MPI_Send(a, 3, MPI_INT,1, 0, MPI_COMM_WORLD);
	}else{
		int * a = malloc(4 * sizeof(int));
		MPI_Recv(a, 3, MPI_INT, 0,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE );
//		printf("%d\n", a);
		printf("Otrzymano %d %d %d\n", a[0], a[1], a[2]);

	}
        MPI_Finalize();
        return 0;
}

double f(double x){
	return x * x;
}
