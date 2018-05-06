package ru.bmstu.nastasia.algorithm;

import ru.bmstu.nastasia.Line;
import ru.bmstu.nastasia.Parameters;

public class NWAffinePenalty extends NWClassic {

    // Scores - хранятся в родительском классе Aligner
    //    final static int m =  4; //match reward
    //    final static int s =  1; //mismatch penalty
    //    protected int d = -2; //first gap penalty
    //    protected int e = -1; //follow gap penalty
    //    protected char GAP= '_';


// Поля, унаследованные от классической версии
//    private int[][]       weights;
//    private Direction[][] path;
//
//    private String A;
//    private String B;
//
//    private StringBuilder alignedA;
//    private StringBuilder alignedB;

//    public String getAlignedA() {
//        return alignedA.toString();
//    }
//
//    public String getAlignedB() {
//        return alignedB.toString();
//    }

    public NWAffinePenalty(Line A, Line B, Parameters parameters) {
        super(A, B, parameters);
    }


    @Override
    public void countScore() {
        //Таблица с весом наилучшего пути от [0][0] до [A.length][A.length], проходящего через каждую ячейку
        weights = new float[this.A.length() + 1][this.B.length() + 1];
        float[][] vertical_gaps   = new float[A.length() + 1][B.length() + 1]; // vertical gaps weights
        float[][] horizontal_gaps = new float[A.length() + 1][B.length() + 1]; // horizontal gaps weights
        // Таблица, где хранится направление, в котором следует двигаться при обратном проходе и расстановке gap-ов
        path    = new Direction[this.A.length() + 1][this.B.length() + 1];
        // Инициализация
//        int[] gapsColumn = new int[A.length() + 1]; // Столбец счетчиков длины последней непрерывной цепочки gap-ов в B
//        int[] gapsLine   = new int[B.length() + 1]; // Строка счетчиков -//- в строке A
        //Нулевые координаты инициализируем отдельно, чтобы не городить if в цикле
        weights[0][0] = 0;
//        gapsColumn[0] = 0;
//        gapsLine[0]   = 0;
        path[0][0] = Direction.DIAG;
        //Стандартная начальная инициализация
        for (int i = 1; i <= A.length(); ++i) {
            weights[i][0] = d + (i - 1) * e; //Аффинная функция вместо d * i
            path[i][0] = Direction.UP;
//            gapsColumn[i] = 0;
        }
        for (int j = 1; j <= B.length(); ++j) {
            weights[0][j] = d + (j - 1) * e;
            path[0][j] = Direction.LEFT;
//            gapsLine[j] = 0;
        }

        // Заполнение матрицы весов (коэффициентов, суммарной длины пути, etc.)

        for (int i = 1; i <= A.length(); ++i) {
            for (int j = 1; j <= B.length(); ++j) {
                float match  = weights[i-1][j-1] + S(A.charAt(i-1), B.charAt(j-1));
                float delete = weights[i-1][j] + (path[i-1][j] == Direction.UP ? e : d);
                float insert = weights[i][j-1] + (path[i][j-1] == Direction.LEFT? e : d);
//                float delete = weights[i-1][j]   + f(gapsLine[j]); //Вычисление f на основе длины gap-а в B
//                float insert = weights[i][j-1]   + f(gapsColumn[i]); //Вычисление f на основе длины gap-а в A
                float maxWeight = Float.max(match, Float.max(insert, delete));
                weights[i][j] = maxWeight;
                if (maxWeight == match) { //Не надо вставлять gap
                    path[i][j] = Direction.DIAG;
//                    gapsColumn[i] = 0;
//                    gapsLine[j] = 0;
                } else if (maxWeight == delete) { //Надо вставить gap в строку В (удалить символ из А)
                    path[i][j] = Direction.UP; //Идем по матрице АхB вверх (при обратном проходе)
//                    ++gapsLine[j];
//                    gapsColumn[i] = 0;
                } else if (maxWeight == insert) { //Надо вставить gap в строку А
                    path[i][j] = Direction.LEFT; //Идем по матрице АхB влево
//                    ++gapsColumn[i];
//                    gapsLine[j] = 0;
                }
            }
        }
        isScoreCounted = true;
        isAligned = false;
    }

    // align, traceback - см. NWClassic
}