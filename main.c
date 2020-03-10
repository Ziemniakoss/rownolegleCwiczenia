#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>


struct range{
	int start;//włącznie
	int finish;//wyłącznie
};


/*
 * Tworzy pamięć współdzieloną o określonym rozmiarze
 */
void * create_shared_memory(size_t size){
	int protection = PROT_READ | PROT_WRITE; //można zapisywać i odczytywać
	int visiblity = MAP_SHARED | MAP_ANONYMOUS; //tylko procesy potomne będą wiedzieć o pamięci
	return mmap(NULL, size, protection, visiblity, -1, 0);
}

void print_vector(double * vector, int n){
	printf("[%f", vector[0]);
	for(int i = 1; i < n; i++)
		printf(" %f", vector[i]);
	printf("]\n"); 
}

double * read_vector_from_file(char * file, int * n){
	double * vector;
	double temp;
	FILE* f = fopen(file, "r");
	if(f == NULL){
		printf("Błąd przy otwieraniu pliku\n");
		exit(2);
	}
	fscanf(f, "%d", n);
	vector = create_shared_memory(sizeof(double) * n[0]);
	for(int i = 0; i < (*n); i++){
		fscanf(f, "%lf", &temp);
		vector[i] = temp;
	}
	fclose(f);
	return vector;	
}

int get_index(pid_t pid, pid_t * array, int n){	
	for(int i = 0; i < n; i++){
		if(pid == array[i])
			return i;
	}
	return -1;
}
	


//To co robi worker
void worker(int signal){}

double vector_sum(double * vector, int start, int end, int n){
	double sum = 0;
	for(int i = start; i < end && i < n; i++)
		sum += vector[i];
	return sum;
}
int main(int argc, char ** argv){
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
	//wysłanie sygnału starrt do workerów i czekanie na zakończenie
	for(int i = 0; i < amount_of_workers; i++){
		kill(workers[i], SIGUSR1);
	}
	for(int i = 0; i < amount_of_workers; i++){
		wait();
	}		
	double s= vector_sum(sums, 0, amount_of_workers, amount_of_workers);
	printf("Suma elementów: %lf\n", s);
	return 0;
}


