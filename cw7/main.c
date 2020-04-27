#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "str_int_map.h"
#include "extractors.h"
#define MASTER 0
#define INIT_SIZE 64 //początkowy rozmiar słownika i tablicy przechowujacej indeksy
#define IN_BUFFOR_SIZE_INCREASE 200 //o tyle będzie się zwiększał bufor przechowujący indeksy słów, gdy sie przepełni

/**
 * Program do tworzenia statystyk z plików z logami.
 *
 * Schemat algorytmu:
 * 	1. Utwórz mapę wszsytkich unikalnych wartości w pliku z logiem
 * 	2. Utwórz tablicę z indeksami w taki sposób, że i-ty element tablicy to wartość w i-tej lini pliku
 * 	3. Podziel i rozseślij tą tablicę
 * 	4. Na każdym podprocesie stwórz mapę gdzie:
 * 		- klucze to będą indeksy słów 
 * 		- wartości to liczba ich występowań w przesłanej podtablicy
 * 	5. Zredukuj wszystkie tablice do jednej wielkiej
 * 	6. Wypisz na stdout  zredukowaną ostateczną tablicę
 */
int main(int argc, char ** argv){
	MPI_Init(&argc, &argv);
	int rank, processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int * inBuffor; 	//przechowuje indeksy w mapie odpowiadajace wartosciom w pliku 
	int * sizes; 		//ile wyslac do kazdego procesu
	int * offsets; 		//poczatki przedzialow dla MPI_Scatterv
	struct map_t map;	// mapa wartości do redukcji
	if(rank == MASTER){
		if(argc != 3){
			fprintf(stderr, "%s [addr|stat|time] [logfile]\n", argv[0]);
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
		int mode;
		if(strcmp("addr", argv[1]) == 0){
			mode = MODE_ADDR;
		}else if(strcmp("stat", argv[1]) == 0){
			mode = MODE_STAT;
		}else if(strcmp("time", argv[1]) == 0){
			mode = MODE_TIME;
		}else{
			fprintf(stderr, "Nieznany tryb pracy\n");
			MPI_Abort(MPI_COMM_WORLD, 2);
		}
		if(init_map(&map, INIT_SIZE) != 0){
			fprintf(stderr, "Błąd przy inicjalizacji słownika\n");
			MPI_Abort(MPI_COMM_WORLD, 9);
		}
		FILE * logFile = fopen(argv[2], "r");
		if(logFile == NULL){
			fprintf(stderr, "Błąd przy otwieraniu pliku, może nie istnieć lub nie masz dostępu\n");
			MPI_Abort(MPI_COMM_WORLD, 3);
		}
		//wczytywanie pliku
		extr_init(mode, logFile);
		int c = 0;
		int inBufforSize = INIT_SIZE;
		inBuffor = malloc(INIT_SIZE * sizeof(int));
		if(inBuffor == NULL){
			fprintf(stderr, "Błąd przy alokacji pamięci na buffor przechowujący indeksy słów\n");
			MPI_Abort(MPI_COMM_WORLD, 11);
		}
		//tworzymy tablicę indeksów
		while(1){
			char * r = extr_next();
			if(r == NULL)
				break;
			int index = add(&map, r);
			if(c >= inBufforSize){
				inBufforSize += IN_BUFFOR_SIZE_INCREASE;
				inBuffor = realloc(inBuffor, inBufforSize * sizeof(int));
				if(inBuffor == NULL){
					fprintf(stderr, "Błąd przy powiększaniu bufora na indeksy\n");
					MPI_Abort(MPI_COMM_WORLD, 14);
				}
			}
			inBuffor[c++] = index; 			
		}
		extr_finalize();
		fclose(logFile);
		//dzielimy ją między procesy
		sizes = malloc(processes * sizeof(int));
		if(sizes == NULL){
			fprintf(stderr, "Błąd przy alokacji tablicę na ilości danych dla procesóœ\n");
			MPI_Abort(MPI_COMM_WORLD, 15);
		}
		offsets = malloc(processes * sizeof(int));
		if(offsets == NULL){
			fprintf(stderr, "Błąd przy alokacji tablicy na offsety\n");
			MPI_Abort(MPI_COMM_WORLD, 16);
		}
		int dataPerProccess = (int)(c * 1.0 / processes);
		sizes[0] = dataPerProccess;
		offsets[0] = 0;
		for(int i = 1; i < processes; i++){
			sizes[i] = dataPerProccess;
			offsets[i] = (i * dataPerProccess) - 1;
		}
		//czy coś zostało nie przydzielone? Jak tak, daj to ostatniemu procesowi
		if(dataPerProccess * processes < c){
			sizes[processes - 1] += (c - (dataPerProccess * processes));
		}
		
	}
	//wysyłamy rozmiary i tworzymy tablice na indeksy i ilosci ich powtorzen
	int size;
	MPI_Scatter(sizes, 1, MPI_INT, &size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
	int indexes[size];

	MPI_Scatterv(inBuffor, sizes, offsets, MPI_INT,
			&indexes, size, MPI_INT, MASTER, MPI_COMM_WORLD);
	if(rank == MASTER)
		free(inBuffor);

	//zliczanie
	int uniqueIndexes = 0;
	int currentCounterSize = INIT_SIZE;
	int * ints = malloc(currentCounterSize * sizeof(int));		//unikalne indexy
	int * occurances = malloc(currentCounterSize * sizeof(int));	//i ich wystąpienia
	if(ints == NULL || occurances == NULL){
		fprintf(stderr, "%d: Błąd alokacji pamięci na licznik\n", rank);
		MPI_Abort(MPI_COMM_WORLD, 21);
	}
	for(int i = 0; i < size; i++){
		int ui = -1;//index w tablicy unikalnych indexów
		for(int j = 0; j < uniqueIndexes; j++)
			if(ints[j] = indexes[i]){
				ui = j;
				break;
			}
		if(ui != -1){
			occurances[ui]++;
		}else{
			if(uniqueIndexes >= currentCounterSize){
				currentCounterSize *= 2;
				ints = realloc(ints, currentCounterSize * sizeof(int));
				occurances = realloc(occurances, currentCounterSize * sizeof(int));
				if(ints == NULL || occurances == NULL){
					fprintf(stderr, "%d: Błąd alokacji przy powiększaniu licznika\n", rank);
					MPI_Abort(MPI_COMM_WORLD, 22);
				}
			}
			ints[uniqueIndexes] = indexes[i];
			occurances[uniqueIndexes++] = 1;
		}
	}

	//teraz zbieramy
	MPI_Gather(&uniqueIndexes, 1, MPI_INT,
			sizes, processes, MPI_INT, MASTER, MPI_COMM_WORLD);
	int * mergedInts;
	int * mergedOccurances;
	int totalSize;
	if(rank == MASTER){
		totalSize = sizes[0];
		offsets[0] = 0;
		for(int i = 1; i < processes; i++){
			offsets[i] = offsets[i - 1] + sizes[i - 1];
			totalSize += sizes[i];
		}
		mergedInts = malloc(totalSize * sizeof(int));
		mergedOccurances = malloc(totalSize * sizeof(int));
		if(mergedInts == NULL || mergedOccurances == NULL){
			fprintf(stderr, "Błąd alokacji tablic na połączone dane\n");
			MPI_Abort(MPI_COMM_WORLD, 23);
		}
	}
	MPI_Gatherv(ints, uniqueIndexes, MPI_INT, 
			mergedInts, &totalSize, offsets, MPI_INT, 
			MASTER, MPI_COMM_WORLD);
	MPI_Gatherv(occurances, uniqueIndexes, MPI_INT,
			mergedOccurances, &totalSize, offsets, MPI_INT,
			MASTER, MPI_COMM_WORLD);
	free(ints);
	free(occurances);
	if(rank == MASTER){
		reduce(&map, mergedInts, mergedOccurances, totalSize);
		print(&map);
		free(mergedInts);
		free(mergedOccurances);
		free(sizes);
		free(offsets);
		free_map(&map);
	}
	MPI_Finalize();	
	return 0;
}


