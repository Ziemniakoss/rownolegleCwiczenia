#include <stdio.h>
#include <stdlib.h>
#include "str_long_map.h"
#include <string.h>


int getIndex(struct sl_map_t * map, char * word){
	for(int i = 0; i < map->maxIndex; i++){
		if(strcmp(word, map->strings[i]) == 0)
			return i;
	}
	return -1;
}

int add(struct sl_map_t * map, char * word){
	int index = getIndex(map, word);
	if(index != -1)
		return index;
	map->maxIndex++;
	if(map->maxIndex > map->size){
		map->size *= 2;
		map->strings = realloc(map->strings, map->size * sizeof(char **));
		if(map->strings == NULL){
			return -1;
		}
		map->values = realloc(map->values, map->size * sizeof(long *));
		if(map->values == NULL){
			return -1;
		}
	}
	map->strings[map->maxIndex] = word;
	map->values[map->maxIndex] = 0;
	return map->maxIndex;
}

void free_map(struct sl_map_t * map){
	free(map->values);
	free(map->strings);
	free(map);
}

struct sl_map_t * create_map(int size){
	if(size <= 0){
		fprintf(stderr, "Niepoprawny rozmiar\n");
		return NULL;
	}
	struct sl_map_t * map = malloc(sizeof(sl_map));
	map->size = size;
	map->maxIndex = -1;
	map->strings = malloc(size * sizeof(char **));
	if(map->strings == NULL){
		fprintf(stderr, "Błąd przy alokacji pamięci na klucze\n");
		free(map);
		return NULL;
	}
	map->values = malloc(size * sizeof(long *));
	if(map->values == NULL){
		fprintf(stderr, "Błąd przy alokacji pamięci na wartości\n");
		free(map->strings);
		free(map);
		return NULL;
	}
	return map;
}

void reduce(struct sl_map_t * map, int * indexes, long * values, int n){
	if(map == NULL || indexes == NULL, values == NULL)
		return;
	for(int i = 0; i < n; i++)
		map->values[indexes[i]] += values[i];
}


void print(struct sl_map_t * map){
	for(int i = 0; i <= map->maxIndex; i++)
		printf("%s: %ld razy\n", map->strings[i], map->values[i]);
}
