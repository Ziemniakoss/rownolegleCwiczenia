#include "integrals.h"
/*
 * Liczenie całki metodą prostokątów
 */
double rectangle(double (*func)(double x), double start, double end){
	return (func)((start + end) /2) * (end - start);
}
/*
 * Liczenie całki metodą trapezów
 */
double trapezoid(double (*func)(double x), double start, double end){
	double a = (func)(start);
	double b = (func)(end);
	return (a + b) * (end - start) / 2;
}
	

/*
 * Liczneie całki metoda simpsona
 */
double simpson(double (*func)(double x), double start, double end){
	double midPoint = (end + start) / 2;
	double h = (end - start) / 2;
	double y0 = (func)(start);
	double y1 = (func)(midPoint);
	double y2 = (func)(end);
	return h / 3 * (y0 + 4 * y1 + y2);	
}
