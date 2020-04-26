#ifndef _EXTRACTORS_H_
#define _EXTRACTORS_H_

#include <stdio.h>
#define MODE_ADDR 1
#define MODE_STAT 2
#define MODE_TIME 3


/*
 * inicjalizacja bufora i trybu wczytywania
 */
void extr_init(int mode, FILE * in);

/*
 * Wydobywa nową wartość z pliku.
 * Kiedy nie ma więcej wartości zwraca NULL
 */
char * extr_next();

/*
 * Zniszczenie bufora. NIe zamyka pliku
 */
void extr_finalize();

char * extr_addr();

char * extr_endpoint();

char * extr_time();
#endif
