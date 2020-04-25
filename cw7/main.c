#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "str_long_map.h"

int main(int argc, char ** argv){
	struct sl_map_t * map = create_map(3);
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
	return 0;
}


