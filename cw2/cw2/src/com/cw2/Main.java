package com.cw2;
import java.io.File;

public class Main {

    public static void main(String[] args) throws InterruptedException {

        int nThreads = 5; // ilosc watkow

        File fileA = new File("resources/A.txt");
        Matrix matrixA = new Matrix(fileA);
        System.out.println("Macierz A: ");
        matrixA.printMatrix();

        File fileB = new File("resources/B.txt");
        Matrix matrixB = new Matrix(fileB);
        System.out.println("Macierz B: ");
        matrixB.printMatrix();

        if(matrixA.getN() != matrixB.getM()){
            System.out.println("Złe wymiary macierzy!\n");
            System.exit(-1);
        }

        /*Utworzenie przedzialow*/
        Range[] ranges = new Range[nThreads];
        int rangeSize = matrixA.getM() * matrixB.getN() / nThreads; //ilosc elementow w przedziale
        for(int i = 0; i < nThreads; i++){
            int startM = i * rangeSize / matrixB.getN();
            int startN = i * rangeSize % matrixB.getN();
            int stopM = i != nThreads - 1 ? (i + 1) * rangeSize / matrixB.getN() : matrixA.getM();
            int stopN = i != nThreads - 1 ? (i + 1) * rangeSize % matrixB.getN() : matrixB.getN();
            ranges[i] = new Range(startM, startN, stopM, stopN);
        }


        Matrix matrixC = new Matrix(matrixA.getM(), matrixB.getN());
        Sums sums = new Sums();

        /*Utworzenie watkow*/
        CountThread[] threads = new CountThread[nThreads];

        for(int i = 0; i < nThreads; i++) {
            threads[i] = new CountThread(matrixA, matrixB, matrixC, ranges[i], sums);
            threads[i].start();
        }

        for(int i = 0; i < nThreads; i++) {
            threads[i].join();
        }

        System.out.println("Macierz wynikowa: ");
        matrixC.printMatrix();
        System.out.println("Suma elementów: " + sums.getSum());
        System.out.println("Norma Frobeniusa: " + Math.sqrt(sums.getSquraeSum()));
    }
}
