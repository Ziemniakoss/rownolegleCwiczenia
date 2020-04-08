#ifndef _UTILS_H_
#define _UTILS_H_
//Kody metod
#define RECTANGLE 1
#define TRAPEZOID 2
#define SIMPSON 3

/*
 * Inicjalizacja głównego procesu, ustala:
 *  - zakres całkowania, 
 *  - na ile części podzielić,
 *  - jakiej metody użyć do całkowania
 */
void init(int argc, char ** argv, double * start, double * end, int * parts, int * method);


/*
 * Funkcja dla której liczymy całkę
 */
double f(double x);
#endif
