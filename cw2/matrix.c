#include <stdio.h>
#include <stdlib.h>

void print_matrix(double **matrix, int m, int n){
	printf("matrix %d x %d\n", m, n);
	
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			printf(" %f", matrix[i][j]);
		}

		printf("\n");
	}
}

double **create_matrix(int m, int n){
	int i;
	double **matrix = malloc(m * sizeof(double));
    	for(i = 0; i < m; i++){
		matrix[i] = malloc(n * sizeof(double));
	}
	
	return matrix;
}

void free_matrix(double **matrix, int m){
	int i;
    	for(i = 0; i < m; i++){
        	free(matrix[i]);
    	}
    	free(matrix);
}

double **read_matrix_from_file(char *file, int *m, int *n){
	double **matrix;
	double temp;
	int i,j;
	FILE *f = fopen(file, "r");
	if(f == NULL){
		printf("Błąd przy otwieraniu pliku: %s\n", file);
		exit(2);
	}

	fscanf(f, "%d", m);
	fscanf(f, "%d", n);

	matrix = create_matrix(*m, *n);
    	for(i = 0; i < *m; i++){
        	for(j = 0; j < *n; j++){
            		fscanf(f, "%lf", &temp);
            		matrix[i][j] = temp;
        	}
    	}	

	fclose(f);
	return matrix;	
}

