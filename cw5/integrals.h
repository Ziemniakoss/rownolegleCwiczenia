#ifndef _INTEGRALS_H_
#define _INTEGRALS_H_

/*
 * Liczenie całki metodą prostokątów
 */
double rectangle(double (*func)(double x), double start, double end);

/*
 * Liczenie całki metodą trapezów
 */
double trapezoid(double (*func)(double x), double start, double end);

/*
 * Liczneie całki metoda simpsona
 */
double simposn(double (*func)(double x), double start, double end);
#endif
