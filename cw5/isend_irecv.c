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
 * 	Program do komuniacji używa funkcji MPI_Isend i MPI_Irecv.
 * 
 * 	Główny procesy wysyła do potomnych tablicę 4 double:
 * 	- pierwszy element to początek przedziału całkowania
 * 	- drugi elemtnt to długość pojedynczego przedziału całkowania
 * 	- trzeci element to ilość przedziałów całkowania
 * 	- czwarty element to metoda całkowania(taka sama numeracja 
 * 	  jak przy wywyoływaniu programu.
 * 	
 * 	
 *	Procesy liczące odsyłają do procesu główngo jedynie jedną liczbę double.
 *
 * 	W przypadku, gdyby nie dałoby się podzielić przedziałów między 
 * 	wszystkie procesy, do procesu zostanie przekazany zerowa liczba przedziałów całkowania
 * 	Proces który ją otrzyma zwraca od razu 0;
 */
int main(int argc, char ** argv){	
	MPI_Init(&argc, &argv);
	int rank = -1;
	int processes = 0;
	double * buffor = malloc(4 * sizeof(double));
	if(buffor == NULL){
		fprintf(stderr, "Nie udało się zainicjalizować bufora\n");
		MPI_Abort(MPI_COMM_WORLD, 10);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	if(processes < 2){
		fprintf(stderr, "Potrzebne przynajmniej 2 rdzenie\n");
		MPI_Abort(MPI_COMM_WORLD,11);
	}
	MPI_Request calculationRequest;
	MPI_Irecv(buffor, 4, MPI_DOUBLE, 
			0, 0, MPI_COMM_WORLD, &calculationRequest);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0){
		double start, end;
		int parts, method;
		init(argc, argv, &start, &end, &parts, &method);
		
		//dzielimy i wysyłamy
		double len = abs(end - start);
		double h = len / parts;
		int partsPerProc = parts > processes ? ceil(parts / processes * 1.0) : 1;
		int part = 0;
		double * data = malloc(4 * sizeof(double));
		for(int i = 1; i < processes; i++){
			if(part < parts){
				data[0] = start + h * partsPerProc * i;
				data[1] = h;
				data[2] = partsPerProc;
				data[3] = method;
				part += partsPerProc;
			}else{
				//nieważne co byle data[2] było 0
				data[2] = 0;
			}
			MPI_Request rrr;
			MPI_Isend(data, 4, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &rrr); 

		}
		free(data);
		//przygotowujemy sie do odebrania
		double * results = malloc((processes - 1) * sizeof(double));
		MPI_Request * requests = malloc((processes - 1) * sizeof(MPI_Request));
		MPI_Status * statuses = malloc((processes - 1) * sizeof(MPI_Status));	
		for(int i = 0; i < processes - 1; i++){
			MPI_Irecv(results + i, 1, MPI_DOUBLE,	
			i + 1, 0, MPI_COMM_WORLD, requests + i);
		}
		//teraz liczymy i zbieramy
		double sum = calculate(start, h, method, partsPerProc);
		MPI_Waitall(processes - 1, requests, statuses);
		for(int i = 0; i < processes -1; i++){
			sum += results[i];
		}
		free(results);
		printf("%lf\n", sum);
	}else{
		MPI_Status status;
		MPI_Wait(&calculationRequest, &status);
		double result = calculate(buffor[0], buffor[1],buffor[3], buffor[2]);
		MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	free(buffor);
	MPI_Finalize();
        return 0;
}
