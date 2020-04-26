#include <stdio.h>
#include <stdlib.h>
#include "str_int_map.h"
#include <string.h>


int getIndex(struct map_t * map, char * word){
	for(int i = 0; i <= map->maxIndex; i++){
		if(strcmp(word, map->strings[i]) == 0)
			return i;
	}
	return -1;
}

int add(struct map_t * map, char * word){
	int index = getIndex(map, word);
	if(index != -1){
		free(word);
		return index;
	}
	map->maxIndex++;
	if(map->maxIndex > map->size){
		printf("teraz %ld\n", map->size*sizeof(char *));
		map->size *= 2;
		printf("%d\n", map->size);
		printf("nowe %ld\n", sizeof(char **) * map->size);
		map->strings = realloc(map->strings, map->size * sizeof(char *));
		if(map->strings == NULL){
			return -1;
		}
		printf("aaa\n");
		map->values = realloc(map->values, map->size * sizeof(int));
		if(map->values == NULL){
			return -1;
		}
	}
	map->strings[map->maxIndex] = word;
	map->values[map->maxIndex] = 0;
	return map->maxIndex;
}

void free_map(struct map_t * map){
	
	for(int i = 0; i < map->maxIndex; i++){
		free(map->strings[i]);
	}
	printf("aaaaaaaa\n");
	free(map->values);
	free(map->strings);
}


void reduce(struct map_t * map, int * indexes, int * values, int n){
	if(map == NULL || indexes == NULL, values == NULL)
		return;
	for(int i = 0; i < n; i++)
		map->values[indexes[i]] += values[i];
}


void print(struct map_t * map){
	for(int i = 0; i <= map->maxIndex; i++)
		printf("%s: %d razy\n", map->strings[i], map->values[i]);
}

void init_map(struct map_t * map, int size){
	if(size < 1){
		fprintf(stderr, "size mniejszy od zera\n");
		map = NULL;
		return;
	}
	map->size = size;
	map->maxIndex = -1;
	map->strings = malloc(size * sizeof(char *));
	if(map->strings == NULL){
		fprintf(stderr, "Błąd przy alokacji pamięci na stringi\n");
		map = NULL;
		return;
	}
	map->values = malloc(size * sizeof(int));
	if(map->values == NULL){
		fprintf(stderr, "Błąd przy alokacji pamięci na wartości\n");
		map = NULL;
		return;
	}
}


