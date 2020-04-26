#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "str_int_map.h"
#include "extractors.h"
#define MASTER 0
#define INIT_SIZE 64 //początkowy rozmiar słownika

int main(int argc, char ** argv){
	MPI_Init(&argc, &argv);
	int rank, processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int * inBuffor;
	struct map_t map;//mapa wartości do redukcji
	//sprawdzamy argumenty
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
		while(1){
			char * r = extr_next();
			if(r == NULL)
				break;
			c++;
			add(&map, r);
		}
		printf("%d linii\n", c);
		printf("%d wartości\n", map.maxIndex + 1);
		extr_finalize();
		fclose(logFile);			

	}
	//TODO obliczenia
	if(rank == MASTER){
		//TODO reduce i print
		//print(&map);
		free_map(&map);
	}
	MPI_Finalize();	
	return 0;
}


