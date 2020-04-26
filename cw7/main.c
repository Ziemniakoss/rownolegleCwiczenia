#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "str_int_map.h"
#include "extractors.h"
#define MASTER 0

int main(int argc, char ** argv){
	MPI_Init(&argc, &argv);
	int rank, processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int * inBuffor;
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
			printf("%d %s\n", c, r);
			free(r);
		}
		printf("%d linii\n", c);
		extr_finalize();
		fclose(logFile);			

	}
	//TODO obliczenia
	if(rank == MASTER){
		//TODO reduce i print
	}
	MPI_Finalize();	
	/*	struct sl_map_t * map = create_map(3);
	printf("aaa\n");
	add(map, "aaa");
	add(map, "bbb");
	add(map, "ccc");
	add(map, "ddd");
	int * indexes = malloc(sizeof(int) * 3);
	long * values = malloc(sizeof(long) * 3);
	indexes[0] = 0;
	indexes[1] = 0;
	indexes[2] = 3;
	values[0] = 10;
	values[1] = 2;
	values[2] = 5;
	reduce(map, indexes, values, 3);
        reduce(map, indexes, values, 3);
        reduce(map, indexes, values, 3);
	
	print(map);
*/	return 0;
}


