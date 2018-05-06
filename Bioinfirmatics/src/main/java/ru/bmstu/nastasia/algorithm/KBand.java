package ru.bmstu.nastasia.algorithm;

import ru.bmstu.nastasia.Line;
import ru.bmstu.nastasia.Parameters;

import java.util.ArrayList;

public class KBand extends Aligner {

    private int kBound;
    private int[][] weights;
    private String A, B;
    private String nameA, nameB;
    private String alignedA, alignedB;

    @Override
    public void setLines(Line... lines) throws IllegalArgumentException {
        if (lines == null || lines.length < 2) {
            throw new IllegalArgumentException("Naive.setLines: min 2 arguments needed");
        }
        nameA = lines[0].getName();
        nameB = lines[1].getName();
        A  = lines[0].getLine();
        B  = lines[1].getLine();
        weights = new int[kBound][Integer.max(A.length(), B.length())];
        isScoreCounted = isAligned = false;
        alignedA = alignedB = null;

    }

    public KBand(Line A, Line B, Parameters parameters) {
        super();

        kBound = parameters.getKBand();

    }

    @Override
    public ArrayList<Line> getAlignment() {
        return null;
    }

    @Override
    public float getScore() {
        return 0;
    }

    @Override
    protected void countScore() {
        super.countScore();
    }

    @Override
    protected void align() {
        super.align();
    }
}
