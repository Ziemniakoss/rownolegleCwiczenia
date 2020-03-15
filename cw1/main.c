#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include "range.h"
#include "vector.h"
#include "memory.h"
#include <math.h>
#define N 900000

//To co robi worker
void worker(int signal){}

double vector_sum(double * vector, int start, int end, int n){
	double sum = 0;
	for(int i = start; i < end && i < n; i++){
		//for(int j = 0; j < N; j++) //utrudnienia
			sum += vector[i]/* * sin(20) * sin(40) * cos(40) * sin(30)*/;
	}
	return sum;
}
int main(int argc, char ** argv){
	//clock_t full = clock();
	//clock_t calculation;

	if(argc < 3){
		printf("./programik N plik_z_wektorem\n");
		exit(1);
	}
	int amount_of_workers = -1;
	amount_of_workers = atoi(argv[1]);
	if(amount_of_workers <= 0){
		printf("Zła liczba workerów: %d\n", amount_of_workers);
		exit(3);
	}
	double * vector;
	int *n = create_shared_memory(sizeof(int)); //długość wektora
	double * sums; 
	pid_t * workers; // wszystkie pid workeróœ
	struct range * ranges; 
	
	sums = create_shared_memory(sizeof(double) * amount_of_workers);
	for(int i = 0; i < amount_of_workers; i++){
		sums[i] = 0;
	}
	workers = create_shared_memory(sizeof(pid_t) * amount_of_workers);
	ranges = create_shared_memory(sizeof(struct range) * amount_of_workers);
	

	pid_t pid;
	vector = read_vector_from_file(argv[2] , n);
	printf("Tworzenie %d workerów...\n", amount_of_workers);
	
	for(int i = 0; i < amount_of_workers; i++){
		pid = fork();
		if(pid == 0){
			sigset_t mask; /* Maska sygnałów */
			/* Konfiguracja obsługi sygnału USR1 */
			struct sigaction usr1;
			sigemptyset(&mask); /* Wyczyść maskę */
			usr1.sa_handler = (&worker);
			usr1.sa_mask = mask;
			usr1.sa_flags = SA_SIGINFO;
			sigaction(SIGUSR1, &usr1, NULL);
			pause();

			int index = get_index(getpid(), workers, amount_of_workers);
			if(index == -1){
				printf("hekkkk\n");
				exit(6);
			}
			sums[index] = vector_sum(vector, ranges[index].start, ranges[index].finish, n[0]);
			exit(0);
		}else if(pid < 0){
			printf("Błąd przy tworzeniu workera %d\n", i);
			abort();
		}else{
			workers[i] = pid;
		}
	}	

	//wczytywanie i ustawieani przedziałów
	int h = n[0] / amount_of_workers + 1;
	int to = 0;
	for(int i = 0; i < amount_of_workers; i++){
		ranges[i].start = to;
		to += h;
		ranges[i].finish = to;
	}
	sleep(1);
	//calculation = clock();
	//wysłanie sygnału starrt do workerów i czekanie na zakończenie
	for(int i = 0; i < amount_of_workers; i++){
		kill(workers[i], SIGUSR1);
	}
	for(int i = 0; i < amount_of_workers; i++){
		wait();
	}		
	double s= vector_sum(sums, 0, amount_of_workers, amount_of_workers);
	printf("Suma elementów: %lf\n", s);
	//double full_ms = (double) (clock() - full) * 1000.0 / CLOCKS_PER_SEC;
	//double calculation_ms = (double)(clock() - calculation) * 1000.0 / CLOCKS_PER_SEC;
	//printf("\ttotal: %f\n", full_ms);
	//printf("\tcalc:  %f\n", calculation_ms);
	return 0;
}


