#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "matrix.h"
#include "matrix_calc.h"

pthread_mutex_t sum_lock;

struct arg_struct {
	double **matrixA;
	int mA;
	int nA;
	double **matrixB;
	int mB;
	int nB;
	double **matrixC;
	int startM;
	int startN;
	int stopM;
	int stopN;
	double *sum;
	double *squares_sum;
};

void *thread_handle(void *arg) {
	struct arg_struct *args = arg;
	double sum=0, squares_sum=0;
	mulitply_matrix(args -> matrixA, args -> mA, args -> nA, args -> matrixB, args -> mB, args -> nB, args -> matrixC, args -> startM, args -> startN,  args -> stopM, args -> stopN, &sum, &squares_sum);

	pthread_mutex_lock(&sum_lock);
	*(args->sum) += sum;
	*(args->squares_sum) += squares_sum;
	pthread_mutex_unlock(&sum_lock);
}

int main(int argc, char ** argv){
	
	double **matrixA, **matrixB, **matrixC;
	double sum = 0, squares_sum = 0;
	int mA, mB, nA, nB;

	if(argc < 4){
		printf("./main ilosc_watkow plik_z_macierza_A plik_z_macierza_B \n");
		exit(1);
	}

	int nThreads = 0;
	nThreads = atoi(argv[1]);

	if(nThreads < 1){
		printf("Zla liczba watkow");
		exit(3);
	}

//Tworzenie macierzy
	matrixA = read_matrix_from_file(argv[2], &mA, &nA);
	print_matrix(matrixA, mA, nA);


	matrixB = read_matrix_from_file(argv[3], &mB, &nB);
	print_matrix(matrixB, mB, nB);

	matrixC = create_matrix(mA, nB);

    	if(nA != mB){
        	printf("Złe wymiary macierzy!\n");
        	return EXIT_FAILURE;
    	}

//podzial na przedzialy
	int i, j;
	int rangeN[nThreads+1], rangeM[nThreads+1];
	int sizeC = mA * nB / nThreads; //ilosc elementow w przedziale
	for(i = 0; i < nThreads; i++){
		rangeM[i] = i * sizeC / nB;
		rangeN[i] = i * sizeC % nB;
	}
	rangeM[nThreads] = mA;
	rangeN[nThreads] = nB;

//utworzenie watkow
    	pthread_t threads[nThreads];
	struct arg_struct args[nThreads];

    	for (int i = 0; i < nThreads; i++) {
		args[i].matrixA = matrixA;
		args[i].mA = mA;
		args[i].nA = nA;
		args[i].matrixB = matrixB;
		args[i].mB = mB;
		args[i].nB = nB;
		args[i].matrixC = matrixC;
		args[i].startM = rangeM[i];
		args[i].startN = rangeN[i];
		args[i].stopM = rangeM[i+1];
		args[i].stopN = rangeN[i+1];
		args[i].sum = &sum;
		args[i].squares_sum = &squares_sum;
		
		int *thread_number = (int *) malloc(sizeof(int));
        	(*thread_number) = i;
        	int result = pthread_create(&(threads[i]), NULL, thread_handle, (void *)&args[i]);
           	if (result != 0) {
                	perror("Nie udalo sie utowrzyc watku!");
        	}
    	}

    	for (int i = 0; i < nThreads; i++) {
            	if(pthread_join(threads[i], NULL) == -1)
			printf("Blad watku");
    	}	

	printf("wynik mnozenia macierzy:\n");
	print_matrix(matrixC, mA, nB);
	printf("suma elementow: %f\n", sum);	
//wyliczenie normy Frobeniusa
	printf("norma Frobeniusa: %f\n", sqrt(squares_sum));

	free_matrix(matrixA, mA);
	free_matrix(matrixB, mB);
	free_matrix(matrixC, mA);


	return 0;
}


