package ru.bmstu.nastasia.algorithm;

import ru.bmstu.nastasia.Line;

import java.util.ArrayList;

public abstract class Aligner {

    protected float d = -2; //first gap penalty
    protected float e = -1; //follow gap penalty
    protected char GAP= '-';

    protected boolean isScoreCounted = false;
    protected boolean isAligned      = false;

    abstract public void setLines(Line ... lines);

    public Aligner() {
        isAligned = false;
        isScoreCounted = false;
    }

    abstract public ArrayList<Line> getAlignment();
    abstract public float getScore();

    protected void countScore() {
        isScoreCounted = true;
        isAligned      = false;
    }

    protected void align() {
        isAligned = true;
    }
}
