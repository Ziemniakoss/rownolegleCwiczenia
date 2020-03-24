package com.cw2;

public class Sums {
    private double sum;
    private double squraeSum;

    public Sums(){
        this.sum = 0;
        this.squraeSum = 0;
    }

    public void sumAdd(double x){
        synchronized (this) {
            sum += x;
        }
    }

    public void squareSumAdd(double x){
        synchronized (this) {
            squraeSum += x;
        }
    }

    public double getSquraeSum() {
        return squraeSum;
    }

    public double getSum() {
        return sum;
    }
}
