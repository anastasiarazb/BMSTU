package ru.bmstu.nastasia.comparator;

public class NkMatrix {

    public static CharComparator comparator = new CharComparator() {
        @Override
        public float compare(char a, char b) {
            return (float)nk[idNK(a)][idNK(b)];
        };
    };

    public static final int nk[][] = {
            { 5, -4, -4, -4},
            {-4,  5, -4, -4},
            {-4, -4,  5, -4},
            {-4, -4, -4,  5}};

    public static int idNK(char a) {
        switch(a) {
            case 'A': return 0;
            case 'G': return 1;
            case 'C': return 2;
            case 'T': return 3;
        }
        return 0;
    }

}
