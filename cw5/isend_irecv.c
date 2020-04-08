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
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	if(processes < 2){
		fprintf(stderr, "Potrzebne przynajmniej 2 rdzenie\n");
		MPI_Finalize();
		exit(1);
	}
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0){
		double start, end;
		int parts, method;
		init(argc, argv, &start, &end, &parts, &method);
		printf("start %lf end %lf parts %d met %d\n", start, end, parts, method);
		//wczytujemy i sprawdzamy parametry
		//dzielimy i wysyłamy
		double len = abs(end - start);
		double h = len / parts;
		int partsPerProc = parts > processes ? ceil(parts / processes * 1.0) : 1;
		int part = 0;
		double * data = malloc(4 * sizeof(double));
		for(int i = 0; i < processes; i++){
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
			#ifdef DEBUG
			printf("Wysyłam do %d start %lf parts %lf h %lf metoda %lf\n", i, data[0],data[2],data[1],data[3]);
			#endif	
			MPI_Send(data, 4, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);

		}
	}
	//teraz obliczenia
	double * a = malloc(4 * sizeof(double));
	MPI_Recv(a, 4, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
	double start = a[0];
	double h = a[1];
	int m = a[3];
	double (*integralFun)(double (*func)(double), double s, double e);
	int intervals = a[2];
	switch(m){
		case RECTANGLE:
			integralFun = rectangle;
			break;
		case TRAPEZOID:
			integralFun = &trapezoid;
			break;
		case SIMPSON:
			integralFun = &simpson;
			break
			;
		default:
			fprintf(stderr, "Błąd przy odbieraniu rodzaj metody, niepoprawny kod: %d\n",(int) a[3]); 
			MPI_Abort(MPI_COMM_WORLD, 10);
	}
	#ifdef DEBUG	
	printf("%d: start = %lf, h = %lf, intervals = %d\n", rank, start, h, intervals);
	#endif
	free(a);
	double result = 0;
	for(int i = 0; i < intervals; i++){
		result += (*integralFun)(f, start, start + h);
		start += h;
	}
	MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	
	//Teraz zbieramy wszystko do razem
	if(rank == 0){
		double sum = 0;
		double subSum;
		#ifdef DEBUG
		printf("Czekam na wyniki\n");
		#endif
		for(int i = 0; i < processes; i++){
			MPI_Recv(&subSum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			#ifdef DEBUG
			printf("Z %d dostałem %lf\n", i, subSum);
			#endif
                	sum += subSum;
		}
		printf("Wynik całkowania: %lf\n", sum);
	}
	MPI_Finalize();
        return 0;
}
