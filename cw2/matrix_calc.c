#include <stdio.h>
#include <stdlib.h>

void mulitply_matrix(double **matrixA, int mA, int nA, double **matrixB, int mB, int nB, double **matrixC, int startN, int startM, int stopN, int stopM, double *sum, double *squares_sum){
    	int i, j, k;
    	double s, sumM = 0, s_sum = 0;
    	for(i=startN; i<=stopN && i<mA; i++){
        	for(j = i == startN ? startM : 0; j<nB && !(i == stopN && j == stopM); j++){
            		s = 0;
            		for(k=0; k<nA; k++){
                		s += matrixA[i][k] * matrixB[k][j];
			}
           		matrixC[i][j] = s;
			sumM += s;
			s_sum += s * s;
        	}
    	}
	*sum = sumM;
	*squares_sum = s_sum;
}




