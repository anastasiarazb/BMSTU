package ru.bmstu.nastasia.algorithm;

import ru.bmstu.nastasia.Line;
import ru.bmstu.nastasia.Parameters;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

public class MultipleSequences extends Aligner {

    static class IndexesListPair {
        ArrayList<Integer> list1;
        ArrayList<Integer> list2;
        IndexesListPair(ArrayList<Integer> list1, ArrayList<Integer> list2) {
            this.list1 = list1;
            this.list2 = list2;
        }
    }

    @Override
    public ArrayList<Line> getAlignment() {
        if (! isAligned ) {
            align();
        }
        return linesAligned;
    }
    @Override
    public float getScore() {
        if (! isScoreCounted ) {
            countScore();
        }
        return score;
    }

    @Override
    protected void align() {
        if (! isScoreCounted ) {
            countScore();
        }
        StringBuilder sb = new StringBuilder();
        for (int k = 0; k < linesRaw.size(); ++k) {
            sb.setLength(0);
            String raw = linesRaw.get(k).getLine();
            ArrayList<Integer> indexes = gapsIndexes.get(k);
//            System.out.println(raw + indexes);
            int j = 0; //Счетчик, бегающий по буквам в исходной строке
            for (int i = 0; i < indexes.size(); ++i) {
                while(j < indexes.get(i)) {
                    sb.append(raw.charAt(j++));
                }
                sb.append(GAP);
            }
            sb.append(raw.substring(j));
            linesAligned.get(k).setLine(sb.toString());
        }
        isAligned = true;
    }

    @Override
    protected void countScore() {
        score = 0;
        commonLength = 0;
        // Массив списков индексов gap-ов, полученных при выравнивании
        gapsIndexes = new ArrayList<>(linesRaw.size());
        // Таблица, в которой хранятся индексы gap-ов, добавленных при сравнении 2-х строк
        // Нужна для того, чтобы при вставке gap-ов, возникших в других строках не добавлять те, что возникли при
        // выравнивании с данной строкой
        gapsIndexesMatrix = new IndexesListPair[linesRaw.size()][linesRaw.size()];
        for (int i = 0; i < linesRaw.size(); ++i) {
            gapsIndexes.add(new ArrayList<>());
        }
        // Первый проход: запоминаем gap-ы, которые возникли при сравнении 2-х строк
        for (int i = 0; i < linesRaw.size(); ++i) {
            Line A = linesRaw.get(i);
            gapsIndexes.add(new ArrayList<>());
            for (int j = i + 1; j < linesRaw.size(); ++j) {
                Line B = linesRaw.get(j);
                dim2Aligner.setLines(A, B);
                ArrayList<Line> result = dim2Aligner.getAlignment();
//                Line.prettyPrint(result, 20);
                Line alignedA = result.get(0);
                Line alignedB = result.get(1);
                score += dim2Aligner.getScore();
                ArrayList<Integer> AGapsIndexes = gapsCoords(alignedA);
                ArrayList<Integer> BGapsIndexes = gapsCoords(alignedB);
                //Слияние старого набора gap-ов и нового
                gapsIndexes.set(i, mergeGaps(gapsIndexes.get(i), AGapsIndexes));
                gapsIndexes.set(j, mergeGaps(gapsIndexes.get(j), BGapsIndexes));
                gapsIndexesMatrix[i][j] = new IndexesListPair(AGapsIndexes, BGapsIndexes);
                gapsIndexesMatrix[j][i] = new IndexesListPair(BGapsIndexes, AGapsIndexes);
            }
        }
        // Второй проход: запоминаем gap-ы, которые должны быть добавлены в данную, так как возникли при сравнении
        // других строк. Используется матрица gapsIndexesMatrix
        for (int k = 0; k < linesRaw.size(); ++k) {
            // Обход матрицы на предмет возникновения gap-ов в других строках
            ArrayList<Integer> inducedGaps = new ArrayList<>();
            for (int i = 0; i < linesRaw.size(); ++i) {
                if (i == k) continue; //Интересуют gap-ы, возникшие в строках не в сравнении с данной
                ArrayList<Integer> otherStringGapIndexes = gapsIndexesMatrix[k][i].list2;
                int shift = shiftCount(otherStringGapIndexes);
//                System.out.println(linesRaw.get(k).getLine() + ":" + k +  "-" + i + otherStringGapIndexes + shift);
                for (int j = 0; j < linesRaw.size(); ++j) {
                    if (   j != k
                        && j != i) {
                        for (Integer idx: gapsIndexesMatrix[i][j].list1) {
                            //Учитываем то, что вторая строка может быть выровнена с первой добавлением начальных gap-ов
                            inducedGaps.add(idx + shift);
                            Collections.sort(inducedGaps);
//                            System.out.println(k + " " + i +"x" + j + "=" + (idx + shift));
                        }
                    }
                }
            }
            //Слияние собственных gap-ов и полученных после выравнивания других строк
            gapsIndexes.set(k, mergeGaps(gapsIndexes.get(k), inducedGaps));
        }
        isScoreCounted = true;
        isAligned      = false;
    }

    private static int shiftCount(ArrayList<Integer> indexes) { //Счетчик начальных gap-ов
        if (indexes.isEmpty()) return 0;
        int counter = 0;
        while (indexes.get(counter) == 0) {
            ++counter;
        }
        return counter;
    }

    protected ArrayList<Line> linesRaw;
    protected ArrayList<Line> linesAligned;
    private float score = 0.0f;
    private int commonLength;
    ArrayList<ArrayList<Integer>> gapsIndexes;
    IndexesListPair[][] gapsIndexesMatrix;

    protected Aligner dim2Aligner;

    public MultipleSequences(ArrayList<Line> lines, Parameters parameters) {
        super();
        dim2Aligner = new NWClassic(parameters);
        linesRaw = lines;
        linesAligned = new ArrayList<>();
        for (Line l: lines) {
            linesAligned.add(new Line(l.getName(), null));
        }
    }

    @Override
    public void setLines(Line ... lines) {
        linesRaw = new ArrayList<>(Arrays.asList(lines));
        linesAligned = new ArrayList<>();
        for (Line l: lines) {
            linesAligned.add(new Line(l.getName(), null));
        }
        isAligned      = false;
        isScoreCounted = false;
    }

    // Координаты букв в исходной строке, на месте которых встали gap-ы (начиная с 0)
    // Берем индекс буквы, за которой встал gap, в исходной строке и прибавляем 1.
    // Например, была строка ABCCX, стала AB_CC_X_, координаты gap-ов 2, 4 и 5,
    // так как 1-й gap встал на место буквы С с индексом 2, второй встал на место 4-й X, 5-й встал следующим за X
    private ArrayList<Integer> gapsCoords(Line l) {
        String str = l.getLine();
        int gapsCounter = 0;
        ArrayList<Integer> gapsCoords = new ArrayList<>();
        for (int i = 0; i < str.length(); ++i) {
            if (str.charAt(i) == GAP) {
                gapsCoords.add(i - gapsCounter); //Сдвиг, как будто предыдущих gap-ов не было
                ++gapsCounter;
            }
        }
        return gapsCoords;
    }

    // Объединение 2-х упорядоченных последовательностей int-ов
    private static ArrayList<Integer> mergeGaps(ArrayList<Integer> arr1, ArrayList<Integer> arr2) {
        ArrayList<Integer> merged = new ArrayList<>();
        int i = 0, j = 0;
        while (i < arr1.size()
                && j < arr2.size()) {
            while (i < arr1.size()
                    && arr1.get(i) < arr2.get(j)) {  //Добавлять меньшие числа из первого списка с индексом i
                merged.add(arr1.get(i++));
            }
            if (i == arr1.size()) {
                break;
            }
            while (    i < arr1.size()
                    && j < arr2.size()
                    && arr1.get(i).equals(arr2.get(j))) { //Добавлять равные из обоих списков
                merged.add(arr1.get(i++));
                ++j;
            }
            if (i == arr1.size()
                    || j == arr2.size()) {
                break;
            };
            while (j < arr2.size()
                    && arr1.get(i) > arr2.get(j)) {  //Добавлять меньшие числа из второго списка с индексом j
                merged.add(arr2.get(j++));
            }
        }
        if (i == arr1.size()) {
            while (j < arr2.size()) {
                merged.add(arr2.get(j++)); //Добавить оставшийся хвост из второго списка
            }
        } else {
            while (i < arr1.size()) {
                merged.add(arr1.get(i++)); //Добавить оставшийся хвост из первого списка
            }
        }
        return merged;
    }

}
