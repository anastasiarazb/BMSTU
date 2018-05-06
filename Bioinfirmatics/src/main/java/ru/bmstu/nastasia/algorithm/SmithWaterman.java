package ru.bmstu.nastasia.algorithm;

import ru.bmstu.nastasia.Line;
import ru.bmstu.nastasia.Parameters;
import ru.bmstu.nastasia.comparator.CharComparator;

import java.util.ArrayList;

public class SmithWaterman extends Aligner {

    class Cell {
        int i = 0, j = 0;
        float value = 0;

        void set(int i, int j, float value) {
            this.i = i;
            this.j = j;
            this.value = value;
        }

        Cell (int i, int j, float value) {
            set(i, j, value);
        }

        @Override
        public String toString() {
            return "[" + i + "][" + j + "] " + value;
        }
    }

    protected float[][] weights;
    protected String  A;
    protected String  B;

    protected String nameA;
    protected String nameB;

    protected ArrayList<Cell> maxValCells;
    protected float max_val;

    protected CharComparator comparator;

    protected ArrayList<StringBuilder> alignedA;
    protected ArrayList<StringBuilder> alignedB;

    protected void init(Line line1, Line line2) {
        this.A = line1.getLine();
        this.B = line2.getLine();
        this.nameA = line1.getName();
        this.nameB = line2.getName();
        isAligned  = false;
        isScoreCounted = false;
    }

    protected void setParameters(Parameters parameters) {
        comparator = parameters.getComparator();
        d          = parameters.getGapPenalty();
        e          = parameters.getExtGapPenalty();
    }

    @Override
    public void setLines(Line... lines) {
        init(lines[0], lines[1]);
    }

    public SmithWaterman(Line line1, Line line2, Parameters  parameters) {
        super();
        init(line1, line2);
        setParameters(parameters);
        countScore();

    }

    @Override
    public ArrayList<Line> getAlignment() {
        if (!isAligned){
            align();
        }
        ArrayList<Line> alignment = new ArrayList<>();
        for (int i = 0; i < alignedA.size(); ++i) {
            alignment.add(new Line(nameA + "_" + String.valueOf(i+1), alignedA.get(i).toString()));
            alignment.add(new Line(nameB + "_" + String.valueOf(i+1), alignedB.get(i).toString()));
        }
        return alignment;
    }

    @Override
    public float getScore() {
        return max_val;
    }

    @Override
    protected void countScore() {
        //Таблица с весом наилучшего пути от [0][0] до [A.length][A.length], проходящего через каждую ячейку
        weights = new float[this.A.length() + 1][this.B.length() + 1];
        // Инициализация
        for (int i = 0; i <= A.length(); ++i) {
            weights[i][0] = 0;
        }
        for (int j = 0; j <= B.length(); ++j) {
            weights[0][j] = 0;
        }
        // Запоминатель координат ячейки с максимальным значением
        maxValCells = new ArrayList<>();
        max_val = 0.0f;
        // Заполнение матрицы весов (коэффициентов, суммарной длины пути, etc.)

        for (int i = 1; i <= A.length(); ++i ) {
            for (int j = 1; j <= B.length(); ++j) {
                // Вычисление веса каждой возможности для следующего шага для текущего пути
                float match  = weights[i-1][j-1] + comparator.compare(A.charAt(i-1), B.charAt(j-1));
                float delete = weights[i-1][j]   + d;
                float insert = weights[i][j-1]   + d;
                float maxWeight = Float.max(0.0f, Float.max(match, Float.max(insert, delete)));
                weights[i][j] = maxWeight;
                if (max_val < maxWeight) {
                    maxValCells = new ArrayList<>();
                    max_val = maxWeight;
                    maxValCells.add(new Cell(i, j, maxWeight));
                } else if (max_val == maxWeight) {
                    maxValCells.add(new Cell(i, j, maxWeight));
                }
            }
        }

        super.countScore();
        alignedA = alignedB = null;
    }

    @Override
    protected void align() {
        if (!isScoreCounted) {
            countScore();
        }
        alignedA = new ArrayList<>();
        alignedB = new ArrayList<>();
//        System.out.println(getMatrix());
        StringBuilder reversedA = new StringBuilder();
        StringBuilder reversedB = new StringBuilder();
        for (Cell cell: maxValCells) {
            reversedA.setLength(0);
            reversedB.setLength(0);
            int i = cell.i;
            int j = cell.j;
            float value = weights[i][j];

//            System.out.printf("[%d][%d] %f\n", i, j, value);
            while (value != 0
                    && (i > 0 || j > 0)) {
                if (i > 0 && j > 0  //match
                        && value == weights[i-1][j-1] + comparator.compare(A.charAt(i-1), B.charAt(j-1))) {
                    --i;
                    --j;
                    reversedA.append(A.charAt(i));
                    reversedB.append(B.charAt(j));
                } else if(i > 0  //delete
                        && value == weights[i-1][j] + d) {
                    --i;
                    reversedA.append(A.charAt(i));
                    reversedB.append(GAP);
                } else if (j > 0 //insert
                        && value == weights[i][j-1] + d) {
                    reversedA.append(GAP);
                    --j;
                    reversedB.append(B.charAt(j));
                }
                value = weights[i][j];
            }
            alignedA.add(reversedA.reverse());
            alignedB.add(reversedB.reverse());
        }
        super.align();
    }

    public String getMatrix() {
        StringBuilder res = new StringBuilder("    ");
        for (char c: B.toCharArray()) {
            res.append("  ").append(c);
        }
        res.append("\r\n  ");
        for (float val : weights[0]) {
            res.append(String.format("%2f ", val));
        }
        for (int i = 1; i < weights.length; ++ i) {
            res.append("\r\n").append(A.charAt(i-1)).append(" ");
            for (float val: weights[i]) {
                res.append(String.format("%2f ", val));
            }
        }
        return res.toString();
    }
}
