package com.company;

public class Range {
    private int startM;
    private int startN;
    private int stopM;
    private int stopN;

    public Range(int startM, int startN, int stopM, int stopN){
        this.startM = startM;
        this.startN = startN;
        this.stopM = stopM;
        this.stopN = stopN;
    }

    public int getStartM() {
        return startM;
    }

    public int getStartN() {
        return startN;
    }

    public int getStopM() {
        return stopM;
    }

    public int getStopN() {
        return stopN;
    }

    public void printRange(){
        System.out.println("od elementu [" + startM + "," + startN + "] do elementu ["+ stopM + "," + startN + "]");
    }
}
