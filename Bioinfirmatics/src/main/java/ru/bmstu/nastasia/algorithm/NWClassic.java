package ru.bmstu.nastasia.algorithm;

import ru.bmstu.nastasia.Line;
import ru.bmstu.nastasia.Parameters;
import ru.bmstu.nastasia.comparator.CharComparator;

import java.util.ArrayList;

public class NWClassic extends Aligner {
    enum Direction {
        DIAG, LEFT, UP;
    }

    // Scores - хранятся в родительском классе Aligner
//    protected int d = -2; //first gap penalty
//    protected int e = -1; //follow gap penalty
//    protected char GAP= '_';

    protected CharComparator comparator;

    protected float[][]       weights;
    protected Direction[][] path;

    protected String A;
    protected String B;

    protected String nameA;
    protected String nameB;

    protected StringBuilder alignedA;
    protected StringBuilder alignedB;

    private Line getAlignedA() {
        return new Line(nameA, alignedA.toString());
    }

    private Line getAlignedB() {
        return new Line(nameB, alignedB.toString());
    }

    @Override
    public ArrayList<Line> getAlignment() {
        if (! isAligned) {
            align();
        }
        ArrayList<Line> lines = new ArrayList<>();
        lines.add(getAlignedA());
        lines.add(getAlignedB());
        return lines;
    }

    @Override
    public void setLines(Line ... lines) {
        this.nameA = lines[0].getName();
        this.nameB = lines[1].getName();
        this.A     = lines[0].getLine();
        this.B     = lines[1].getLine();
        isAligned      = false;
        isScoreCounted = false;
    }

    public void setParameters(Parameters parameters) {
        this.comparator = parameters.getComparator();
        this.d = parameters.getGapPenalty();
        this.e = parameters.getExtGapPenalty();
        isAligned      = false;
        isScoreCounted = false;
    }

    public NWClassic(Parameters parameters) {
        setParameters(parameters);
    }

    public NWClassic(Line A, Line B, Parameters parameters) {
        super();
        setParameters(parameters);
        setLines(A, B);
        countScore();
    }

    // Функция сравнения символов: награда за совпадение / штраф за несовпадение
    protected float S(char x, char y) {
        return comparator.compare(x, y);
    }

    protected NWClassic()
    {}

    @Override
    public float getScore() {
        if (!isScoreCounted) {
            countScore();
        }
        return weights[A.length()][B.length()];
    }

    public void countScore() {
        //Таблица с весом наилучшего пути от [0][0] до [A.length][A.length], проходящего через каждую ячейку
        weights = new float[this.A.length() + 1][this.B.length() + 1];
        // Таблица, где хранится направление, в котором следует двигаться при обратном проходе и расстановке gap-ов
        path    = new Direction[this.A.length() + 1][this.B.length() + 1];
        // Инициализация
        for (int i = 0; i <= A.length(); ++i) {
            weights[i][0] = i * d;
            path[i][0] = Direction.UP;
        }
        for (int j = 0; j <= B.length(); ++j) {
            weights[0][j] = j * d;
            path[0][j] = Direction.LEFT;
        }
        // Заполнение матрицы весов (коэффициентов, суммарной длины пути, etc.)

        for (int i = 1; i <= A.length(); ++i) {
            for (int j = 1; j <= B.length(); ++j) {
                // Вычисление веса каждой возможности для следующего шага для текущего пути
                float match  = weights[i-1][j-1] + S(A.charAt(i-1), B.charAt(j-1));
                float delete = weights[i-1][j]   + d;
                float insert = weights[i][j-1]   + d;
                float maxWeight = Float.max(match, Float.max(insert, delete));
                weights[i][j] = maxWeight;
                if (maxWeight == match) {  //Не надо вставлять gap
                    path[i][j] = Direction.DIAG;
                }
                if (maxWeight == delete) { //Надо вставить gap в строку В (удалить символ из А)
                    path[i][j] = Direction.UP;
                }
                if (maxWeight == insert) {
                    path[i][j] = Direction.LEFT; //Надо вставить gap в строку A
                }
            }
        }
        isScoreCounted = true;
        isAligned = false;
    }

    @Override
    protected void align() {
        if (!isScoreCounted) {
            countScore();
        }
        traceBack();
        isAligned = true;
    }

    protected void traceBack() {
        // Выравнивание
        // Делаем обратный проход по таблице из ячейки [A.length][B.length] в ячейку [0][0], направления берем из
        // таблицы path

        alignedA = new StringBuilder(A.length() + B.length());
        alignedB = new StringBuilder(A.length() + B.length());
        int i = A.length();
        int j = B.length();
        while (i > 0 || j > 0) {
            Direction direction = path[i][j];
//            System.out.println(path[i][j]);
            if (i > 0 && j > 0 && direction == Direction.DIAG) {
                --i;
                --j;
                alignedA.append(A.charAt(i));
                alignedB.append(B.charAt(j));
            } else if(i > 0 && direction == Direction.UP) {
                --i;
                alignedA.append(A.charAt(i));
                alignedB.append(GAP);
            } else if (j > 0 && direction == Direction.LEFT) {
                alignedA.append(GAP);
                --j;
                alignedB.append(B.charAt(j));
            }
        }

        alignedA.reverse();
        alignedB.reverse();
    }

    public String getMatrix() {
        StringBuilder res = new StringBuilder("      ");
        for (char c: B.toCharArray()) {
            res.append("     ").append(c);
        }
        res.append("\r\n  ");
        for (float val : weights[0]) {
            res.append(String.format("% 5.1f ", val));
        }
        for (int i = 1; i < weights.length; ++ i) {
            res.append("\r\n").append(A.charAt(i-1)).append(" ");
            for (float val: weights[i]) {
                res.append(String.format("% 5.1f ", val));
            }
        }
        return res.toString();
    }
}