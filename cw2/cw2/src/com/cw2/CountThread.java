package com.cw2;

public class CountThread extends Thread {

    private Matrix matrixA;
    private Matrix matrixB;
    private Matrix matrixC;
    private Range range;
    private Sums sums;

    public CountThread(Matrix matrixA, Matrix matrixB, Matrix matrixC, Range range, Sums sums){
        this.matrixA = matrixA;
        this.matrixB = matrixB;
        this.matrixC = matrixC;
        this.range = range;
        this.sums = sums;
    }

    @Override
    public void run() {
        matrixA.multiplyMatrix(matrixB, matrixC, range);
        sums.sumAdd(matrixC.sumElements(range));
        sums.squareSumAdd(matrixC.squareSumElements(range));
    }
}
