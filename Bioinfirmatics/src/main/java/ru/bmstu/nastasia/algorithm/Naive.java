package ru.bmstu.nastasia.algorithm;

import ru.bmstu.nastasia.Line;
import ru.bmstu.nastasia.Parameters;
import ru.bmstu.nastasia.comparator.CharComparator;

import java.util.ArrayList;
import java.util.PriorityQueue;

// Лаба 0, отправленная Терешкиной как SequenceComparator
// Изменения: наследование от Aligner, аффинный штраф за делецию, изменение стека с OrderedSet на PriorityQueue,
// фикс функции, вставляющий gap-ы в выровненную строку (buildString),
// изменение функции сравнения двух вариантов выравнивания.

// Особенности наивной реализации: попытка пройти по всем-всем выравниваниям, практически построение дерева сравнений в
// каждой позиции и его обход. Сродни алгоритму Эрли (разбор слова в КС-грамматике) хранятся списки ситуаций
// (очередь CompareElements), которые на каждой итерации преобразуется.
// Благодаря тому, что из очереди каждый раз достается элемент с максимальным количеством очков на данный момент,
// первую пару, выровненную до конца, берем за итоговый результат.
// Если нужна 100% оптимальная последовательность, в функции count Score в первом if надо break заменить на continue
//if (pos1 == str1.length() && pos2 == str2.length()) {
//        if (bestSolution.score < elem.score) {
//        bestSolution = elem;
//        }
////                continue;
//        break;
//        }
// Тогда алгоритм не будет останавливаться на первой найденной последовательности, а честно проверит их все. Но тогда
// результата не будет долго, ибо сложность по времени и памяти показательная (примерно 3^n).

public class Naive extends Aligner {

//    float d = -2; // штраф за открытие делеции
//    float e = -2; // штраф за продолжение делеции
    float m =  1; // награда за совпадение
    float s = -1; // штраф за несовпадение
    CharComparator comparator;

    // Класс, который хранит промежуточный вариант выравнивания: позицию, очки, уже вставленные gap-ы
    class CompareElement implements Comparable{

        private int pos = 0;
        private float score  = 0.0f;
        private ArrayList<Integer> gapsUp;   //Индексы gap'ов, вставленных в верхнюю строку
        private ArrayList<Integer> gapsDown; //Индексы gap'ов, вставленных в верхнюю строку

        public CompareElement() {
            score = 0;
            pos = 0;
            gapsUp = new ArrayList<>();   //Индексы gap'ов, вставленных в верхнюю строку
            gapsDown = new ArrayList<>(); //Индексы gap'ов, вставленных в верхнюю строку
        }

        private CompareElement(int pos, ArrayList<Integer> gapsUp, ArrayList<Integer> gapsDown, float score) {
            this.score = score;
            this.pos = pos;
            this.gapsUp = gapsUp;
            this.gapsDown = gapsDown;
        }

        // ******************************* Shift functions *******************************
        public CompareElement shiftUp() {
            ArrayList<Integer> newUp = new ArrayList<>(gapsUp);
            float penalty = (newUp.size() == 0 || newUp.get(newUp.size() - 1) != (pos - 1))
                    ? d  //Открытие делеции
                    : e; // Продолжение делеции
            newUp.add(pos);
            return new CompareElement(pos + 1, newUp, gapsDown, score + penalty);
        }

        public CompareElement shiftDown() {
            ArrayList<Integer> newDown = new ArrayList<>(gapsDown);
            float penalty = (newDown.size() == 0 || newDown.get(newDown.size() - 1) != (pos - 1)) // Продолжение делеции
                    ? d  //Открытие делеции
                    : e; // Продолжение делеции
            newDown.add(pos);
            return new CompareElement(pos + 1, gapsUp, newDown, score + penalty);
        }

        public CompareElement shiftDifference(float matchScore) {
            return new CompareElement(pos + 1, gapsUp, gapsDown, score + matchScore);
        }

        public CompareElement shiftEqual(float matchScore) {
            return new CompareElement(pos + 1, gapsUp, gapsDown, score + matchScore);
        }
        // ******************************* Comparator *******************************
        // Сравнение двух строк для выбора из очереди более приоритетной: той, у которой выше шансы по очкам оказаться
        // оптимальным выравниванием
        @Override
        public int compareTo(Object o) {
            CompareElement other = (CompareElement)o;
            if (this.equals(o)) return 0;
//            if (pos >= other.pos) {
//                if (score > other.score) {
//                    return -1; // Поставить приоритет выше у this
//                } else {
//                    return 1; // Поставить приоритет выше у other
//                }
//            } else {
//                if (score < other.score) {
//                    return -1;
//                } else {
//                    return 1;
//                }
//            }
//            if (pos >= other.pos) {
//                return Integer.compare(other.score + (pos - other.pos) * d, score);
//            } else {
//                return Integer.compare(other.score, score  + (other.pos - pos) * d);
//            }
            return Float.compare(other.score, this.score);
        }

        // Оба выравнивания экваивалентны, если на следующей итерации будут просматриваться буквы в одинаковых позициях,
        // то есть равны текущие позиции и количество gap-ов
        @Override
        public boolean equals(Object o) {
            if (!(o instanceof CompareElement)) return false;
            CompareElement other = (CompareElement) o;
            return (this.pos == other.pos)
                    && (this.score == other.score)
                    && (gapsDown.size() == other.gapsDown.size())
                    && (gapsUp.size() == other.gapsUp.size());


        }


        // ******************************* Getters *******************************


        public int gapsCounter() {
            return gapsUp.size() + gapsDown.size();
        }

        //Вернуть индекс обозреваемой буквы верхней строки с поправкой на добавленные gap'ы
        public int getUpperIndex() {
            return pos - gapsUp.size();
        }

        //Вернуть индекс обозреваемой буквы нижней строки с поправкой на добавленные gap'ы
        public int getLowerIndex() {
            return pos - gapsDown.size();
        }

        public int getPos() {
            return pos;
        }

        public int getGapsUpCounter() {
            return gapsUp.size();
        }

        public int getGapsDownCounter() {
            return gapsDown.size();
        }

        public ArrayList<Integer> getGapsUp() {
            return gapsUp;
        }

        public ArrayList<Integer> getGapsDown() {
            return gapsDown;
        }

        public float getScore() {
            return score;
        }

        @Override
        public String toString() {
            return "[" + pos + "] " + score
                    + "\n  " + getGapsUp()
                    + "\n  " + getGapsDown();
        }
    }

    public CompareElement produceWorstCase() {
        int sum_len = str1.length() + str2.length();
        ArrayList<Integer> worst = new ArrayList<>(sum_len);
        for (int i = 0; i < sum_len; ++i) {
            worst.add(i);
        }
        return new CompareElement(sum_len, worst, worst, Integer.MIN_VALUE); //Хуже быть не может
    }

    String str1, str2;
    String name1, name2;
    String aligned1, aligned2;
    PriorityQueue<CompareElement> queue;
    CompareElement     bestSolution;

    public Naive(String str1, String str2) {
        super();
        this.str1 = str1;
        this.str2 = str2;
        queue = new PriorityQueue<>(Integer.max(str1.length(), str2.length()));
        bestSolution = null;
    }

    public Naive(Line A, Line B, Parameters parameters) {
        this(A.getLine(), B.getLine());
        name1 = A.getName();
        name2 = B.getName();
        d = parameters.getGapPenalty();
        e = parameters.getExtGapPenalty();
        s = parameters.getMismatchPenalty();
        m = parameters.getMatchReward();
        comparator = parameters.getComparator();

        aligned1 = aligned2 = null;
    }

    @Override
    public void align() {
        if (!isScoreCounted) {
            countScore();
        }
        aligned1 = buildString(str1, bestSolution.getGapsUp(),   bestSolution.pos);
        aligned2 = buildString(str2, bestSolution.getGapsDown(), bestSolution.pos);
        super.align();
    }

    private String buildString(String src, ArrayList<Integer> gaps, int pos) {
        StringBuilder aligned = new StringBuilder(pos);
        int j = 0;
        for (int i = 0; i < pos; ++i) {
            if (j < gaps.size() && gaps.get(j) == i) {
                aligned.append(GAP);
                ++j;
            } else {
                aligned.append(src.charAt(i - j));
            }
        }
        return aligned.toString();
    }

    @Override
    public void setLines(Line... lines) throws IllegalArgumentException {
        if (lines == null || lines.length < 2) {
            throw new IllegalArgumentException("Naive.setLines: min 2 arguments needed");
        }
        name1 = lines[0].getName();
        name2 = lines[1].getName();
        str1  = lines[0].getLine();
        str2  = lines[1].getLine();
        isScoreCounted = isAligned = false;
        aligned1 = aligned2 = null;
    }

    @Override
    public ArrayList<Line> getAlignment() {
        if (!isAligned) {
            align();
        }
        ArrayList<Line> result = new ArrayList<>();
        result.add(new Line(name1, aligned1));
        result.add(new Line(name2, aligned2));
        return result;
    }

    @Override
    public float getScore() {
        if (!isScoreCounted) {
            countScore();
        }
        return bestSolution.getScore();
    }

    @Override
    protected void countScore() {
        queue.clear();
        aligned1 = aligned2 = null;
        bestSolution = produceWorstCase();
        queue.add(new CompareElement());
        while (!queue.isEmpty()) {
            //Взять последовательность с наименьшим количеством ошибок
            CompareElement elem = queue.poll();
            int pos1 = elem.getUpperIndex(); // Индексы обозреваемых букв в верхней/нижней строке
            int pos2 = elem.getLowerIndex();
//            if (bestSolution.score >= elem.score + m * (bestSolution.pos - elem.pos)) {
//                continue;
//                // Так как нет смысла дальше строить выравнивание строки,у которой score будет меньше наилучшего,
//                // даже если все остальные символы до конца строки совпадут
//            }
            // Сверка текущего состояния с последним наилучшим результатом
            if (pos1 == str1.length() && pos2 == str2.length()) {
                if (bestSolution.score < elem.score) {
                    bestSolution = elem;
                }
//                continue; // <-- Выполнять continue для получения оптимального выравнивания, показательная сложность
                break; // <-- Выполнять break для получения просто хорошего выравнивания, линейное время для равных строк
            }
            //Доставить gap-ы в более короткую строку
            if (pos1 == str1.length()) {
                queue.add(elem.shiftUp());
                continue;
            }
            if (pos2 == str2.length()) {
                queue.add(elem.shiftDown());
                continue;
            }
            //Положить на стек для дальнейшего рассмотрения все 3 возможных варианта:
            // без делеций, делеция в str1, делеция в str2
            char a = str1.charAt(pos1);
            char b = str2.charAt(pos2);
            if (a == b) {
                queue.add(elem.shiftEqual(comparator.compare(a, b)));
            } else {
                queue.add(elem.shiftDifference(comparator.compare(a, b)));
            }
            queue.add(elem.shiftUp());
            queue.add(elem.shiftDown());
        }
        super.countScore();
    }

}

