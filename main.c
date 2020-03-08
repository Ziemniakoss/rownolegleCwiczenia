#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define N 10
#define BUFFOR_SIZE 80
void printVector(double * vector, int n){
	printf("[%f", vector[0]);
	for(int i = 1; i < n; i++)
		printf(" %f", vector[i]);
	printf("]\n"); 
}

int main(int argc, char ** argv){
	pid_t workers[N];
	int n = 0; //długość wektora
	//TODO tworzenie workerów
	

	//wczytujemy wektor
	char buffor[BUFFOR_SIZE + 1];
	FILE* f = fopen("vector", "r");
	if(f == NULL){
		printf("Błąd przy otwieraniu pliku\n");
		return 1;
	}
	//fgets(buffor, BUFFOR_SIZE, f);
	fscanf(f, "%d", &n);
	double vector[n];
	printf("Wczytuje %d-elemetnowy wektor\n", n);
	double temp;
	for(int i = 0; i < n; i++){
		fscanf(f, "%lf", &temp);
		vector[i] = temp;
	}
	fclose(f);
	printVector(vector, n);
}


