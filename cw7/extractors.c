#include "extractors.h"
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>

int extr_mode;
char * buffor;
int buffSize;
FILE * extr_in;

void extr_init(int mode, FILE * in){
	buffSize = 3000;
	buffor = malloc(sizeof(char) * buffSize);
	if(buffor == NULL){
		fprintf(stderr, "Błąd przy inicjalizacji buffora wejsściowego\n");
		MPI_Abort(MPI_COMM_WORLD, 4);
	}
	switch(mode){
		case MODE_ADDR:
		case MODE_STAT:
		case MODE_TIME:
			extr_mode = mode;
			break;
		default:
			fprintf(stderr, "NIeznany tryb\n");
			MPI_Abort(MPI_COMM_WORLD, 5);
	}
	extr_in = in;
}

char * extr_next(){
	switch(extr_mode){
		case MODE_ADDR:
			return extr_addr();
		case MODE_STAT:
			return extr_endpoint();
		case MODE_TIME:
			return extr_time();
		default:
			return NULL;
	}
}

char * extr_addr(){
	char c;
	int len = 0;
	c = fgetc(extr_in);
	if(c == EOF)
		return NULL;
	buffor[len++]= c; 
	while((c = fgetc(extr_in)) != EOF &&  c != (int)' '){
		buffor[len++] = c;
	}
	char * address = malloc((len + 1) * sizeof(char));
	if(address == NULL){
		fprintf(stderr, "Błąd przy rezerwacji pamięci na nowy adres\n");
		MPI_Abort(MPI_COMM_WORLD, 6);
	}
	for(int i = 0; i < len; i++)
		address[i] = buffor[i];
	address[len] = '\0';
	//do nowej linii
	while((c = fgetc(extr_in)) != EOF && c != '\n');
	return address;
}


char * extr_endpoint(){
	char c = fgetc(extr_in);
	if(c == EOF){
		return NULL;
	}
	int len = 0;
	//pomijamy wszsytko do pierwszej spacji po pierwszym cudzyslowu
	while((c = fgetc(extr_in)) != '"');
	while((c = fgetc(extr_in)) != ' ');

	//skanowanko wszystkiego do spacji(końca endpointu)
	while((c = fgetc(extr_in)) != ' '){
		if(len > buffSize){
			buffSize += 200;
			buffor = realloc(buffor, buffSize * sizeof(char));
			if(buffor == NULL){
				fprintf(stderr, "Błąd przy powiększaniu bufora\n");
				MPI_Abort(MPI_COMM_WORLD, 8);
			}
		}
		buffor[len++] = c;
	}
	//do nowej lini
        while((c = fgetc(extr_in)) != EOF && c != '\n');

	char * endpoint = malloc((len + 1) * sizeof(char));
	if(endpoint == NULL){
		fprintf(stderr, "Błąd przy alokacji pamięci na endpoint\n");
		MPI_Abort(MPI_COMM_WORLD, 9);
	}
	for(int i = 0; i < len; i++)
		endpoint[i] = buffor[i];
	endpoint[len] = '\0';
	return endpoint;
}

char * extr_time(){
	char c;
	int len = 0;
	c = fgetc(extr_in);
	if(c == EOF){
		return NULL;
	}
	//pomin 3 pierwsze spacje
	while((c = fgetc(extr_in)) != ' ');
        while((c = fgetc(extr_in)) != ' ');
        while((c = fgetc(extr_in)) != ' ');
	fgetc(extr_in);//pomin nawias
	//teraz skanujemy wszsytko az naliczymy dwa znaki po 3 ':'
	int colons = 0;
	while(colons < 3){
		c = fgetc(extr_in);
		if(c == ':')
			colons++;
		buffor[len++] = c;
	}
	buffor[len++] = fgetc(extr_in);
	buffor[len++] = fgetc(extr_in);
	//teraz do nowej lini
        while((c = fgetc(extr_in)) != EOF && c != '\n');

	char * date = malloc((len+1) * sizeof(char));
	if(date == NULL){
		fprintf(stderr, "Błąd przy alokacji pamięci na datę\n");
		MPI_Abort(MPI_COMM_WORLD, 7);
	}
	for(int i = 0; i < len; i++)
		date[i] = buffor[i];
	date[len] = '\0';
	return date; 
}


void extr_finalize(){
	free(buffor);
}


