package com.cw2;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Matrix {
    private double[][] matrix;
    private int m;
    private int n;

    public Matrix(int m, int n){
        this.m = m;
        this.n = n;
        matrix = new double[m][n];
    }

    public Matrix(File file){
        Scanner myReader = null;
        try {
            myReader = new Scanner(file);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        m = Integer.parseInt(myReader.nextLine());
        n = Integer.parseInt(myReader.nextLine());
        matrix = new double[m][n];

        for(int i = 0; i < m; i++){
            if(myReader.hasNextLine()) {
                String[] line = myReader.nextLine().trim().split(" ");
                for (int j = 0; j < n; j++) {
                    matrix[i][j] = Double.parseDouble(line[j]);
                }
            }
        }

        myReader.close();
    }

    public void multiplyMatrix(Matrix matrixB, Matrix matrixC, Range range){
        for(int i = range.getStartM(); i <= range.getStopM() &&  i < m; i++){
            for(int j = i == range.getStartM() ? range.getStartN() : 0; j < matrixB.getN() && !(i == range.getStopM() && j == range.getStopN()); j++){
                double s = 0;
                for(int k=0; k < n; k++){
                    s += matrix[i][k] * matrixB.getElement(k, j);
                }
                matrixC.setElement(i, j, s);
            }
        }
    }

    public double sumElements(Range range){
        double sum = 0;
        for(int i = range.getStartM(); i <= range.getStopM() &&  i < m; i++) {
            for (int j = i == range.getStartM() ? range.getStartN() : 0; j < n && !(i == range.getStopM() && j == range.getStopN()); j++) {
                sum += matrix[i][j];
            }
        }

        return sum;
    }

    public double squareSumElements(Range range){
        double squareSum = 0;
        for(int i = range.getStartM(); i <= range.getStopM() &&  i < m; i++) {
            for (int j = i == range.getStartM() ? range.getStartN() : 0; j < n && !(i == range.getStopM() && j == range.getStopN()); j++) {
                squareSum += matrix[i][j] * matrix[i][j];
            }
        }

        return squareSum;
    }

    public void printMatrix(){
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.print("\n");
        }
        System.out.print("\n");
    }

    public int getM() {
        return m;
    }

    public int getN() {
        return n;
    }

    public void setElement(int m, int n, double x){
        this.matrix[m][n] = x;
    }

    public double getElement(int m, int n){
        return matrix[m][n];
    }
}
