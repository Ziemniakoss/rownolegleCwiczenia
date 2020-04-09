#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "utils.h"
#include "integrals.h"
#include <math.h>


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
 * 	Do komuikkacji wykorzystujemy MPI_Scatter i MPI_Reduce.
 *
 * Króki opis działania
 * 	Proces z rangą 0 tworzy główny buffor o wielkości 4 * processes *sizeof(double).
 * 	W tym buforze umieszcza dane do wysłania. Każdemy procesowi(sobie też) wysyła 4 liczby double:
 * 	 - początek przedziału całkowania
 * 	 - długość przedziału całkowania
 * 	 - ile przedziałów ma zcałkować
 * 	 - kod metody
 * 	Następnie każdy proces oblicza lokalną całkę.
 * 	Po obliczeniu następuje operacja MPI_Reduce któ©a sumuje wszystkie lokalne
 * 	sumy w jedną globalną
 */
int main(int argc, char ** argv){	
        MPI_Init(&argc, &argv);
        int rank = -1;
        int processes = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	if(processes < 2){
		fprintf(stderr, "Potrzebne przynajmniej 2 rdzenie\n");
		MPI_Finalize();
		exit(1);
	}
	double * buffor;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0){
		double start, end;
		int parts, method;
		init(argc, argv, &start, &end, &parts, &method);
		//dzielimy dane
		double len = abs(end - start);
		double h = len / parts;
		int partsPerProc = parts > processes ? ceil(parts / processes * 1.0) : 1;
		int part = 0;
		buffor = malloc(4 * processes * sizeof(double));
		if(buffor == NULL){
			fprintf(stderr, "Błąd przy tworzeniu bufora\n");
			MPI_Abort(MPI_COMM_WORLD, 12);
		}
		for(int i = 0; i < processes; i++){
			if(part < parts){
				int baseIndex = i * 4;
				buffor[baseIndex] = start + h * partsPerProc * i;
				buffor[baseIndex + 1] = h;
				buffor[baseIndex + 2] = partsPerProc;
				buffor[baseIndex + 3] = method;
				part += partsPerProc;
			}else{
				//nieważne co byle liczba iteracji była równa 0
				buffor[2 + i * 4] = 0;
			}
		}
	}
	//teraz obliczenia
	double * recvBuffor = malloc(4 * sizeof(double));
	if(recvBuffor == NULL){
		fprintf(stderr, "%d Błąd przy tworzeniu bufora na odbiór\n", rank);
		MPI_Abort(MPI_COMM_WORLD, 13);
	}
	MPI_Scatter(buffor, 4, MPI_DOUBLE, 
			recvBuffor, 4, MPI_DOUBLE, 
			0, MPI_COMM_WORLD);
	double localSum = calculate(recvBuffor[0], recvBuffor[1], recvBuffor[3], recvBuffor[2]);
	free(recvBuffor);
	
	double globalSum = 0;
	MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if(rank == 0){
		printf("%lf\n", globalSum);
		free(buffor);
	}	
	MPI_Finalize();
        return 0;
}
