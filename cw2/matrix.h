#ifndef matrix_h_included
#define matrix_h_included
void print_matrix(double **matrix, int m, int n);
double **read_matrix_from_file(char * file, int *m, int *n);
void free_matrix(double **matrix, int m);
double **create_matrix(int m, int n);
#endif
