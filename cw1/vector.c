#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "memory.h"

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
