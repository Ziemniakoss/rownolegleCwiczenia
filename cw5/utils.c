#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "integrals.h"
#include <mpi.h>

void init(int argc, char ** argv, double * start, double * end, int * parts, int * method){
	printf("%s %s %s %s\n", argv[0], argv[1], argv[2], argv[3]); 
	if(argc != 5){
		fprintf(stderr, "%s [start] [koniec] [liczba_przedziałow] [metoda]\n", argv[0]);
		fprintf(stderr, "Metody:\n\t1 - prostokątów\n\t2 - trapezów\n\t3- simpsona\n");
		MPI_Abort(MPI_COMM_WORLD, 2);
	}
	if(sscanf(argv[1], "%lf", start) != 1){
		fprintf(stderr, "Nie udało sie wczytać początku przedziału\n");
		MPI_Abort(MPI_COMM_WORLD, 3);
	}
	if(sscanf(argv[2], "%lf", end) != 1){
		fprintf(stderr, "Nie udało sie wczytać końca przedziału\n");
		MPI_Abort(MPI_COMM_WORLD, 4);
	}
	if(*end < *start){
		fprintf(stderr, "Zły przedział\n");
		MPI_Abort(MPI_COMM_WORLD, 5);
	}
	if(sscanf(argv[3], "%d", parts) != 1){
		fprintf(stderr, "Nie udało sie wczytać ilości przedziałów\n");
		MPI_Abort(MPI_COMM_WORLD, 6);
	}
	if(*parts < 1){
		fprintf(stderr, "Liczba części musi byc większa od zera\n");
		MPI_Abort(MPI_COMM_WORLD, 7);
	}
	if(sscanf(argv[4], "%d", method) != 1){
		fprintf(stderr, "Nie udało się wczytać kodu metody\n");
      		MPI_Abort(MPI_COMM_WORLD, 8);
  	}
	if(*method != RECTANGLE && *method != TRAPEZOID && *method != SIMPSON){
		fprintf(stderr, "Nieznana metoda\n");
		MPI_Abort(MPI_COMM_WORLD, 9);
	}
}


/*
 * Funkcja dla której liczymy całkę
 */
double f(double x){
	return sin(x) * cos(x) * x / x / sin(x) * sin(x * cos(x));
	//return x * x;
}

double calculate(double start, double h, int method, int intervals){
        double (*integralFun)(double (*func)(double x), double s, double e);
        switch(method){
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
                        fprintf(stderr, "Błąd przy odbieraniu rodzaj metody, niepoprawny kod: %d\n", method);
                        MPI_Abort(MPI_COMM_WORLD, 10);
        }
        #ifdef DEBUG
        printf("%d: start = %lf, h = %lf, intervals = %d\n", rank, start, h, intervals);
        #endif
        double result = 0;
        for(int i = 0; i < intervals; i++){
                result += (*integralFun)(f, start, start + h);
                start += h;
        }
        return result;
}

