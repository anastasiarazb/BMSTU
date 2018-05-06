#ifndef OLD_VERSIONS_H
#define OLD_VERSIONS_H

#include <stdio.h>

int array[] = { 1, 2, 3, 4, 5, 3, 2, 5};

int less(unsigned long i, unsigned long j){
    return array[i] < array[j];
}
unsigned long peak(unsigned long, int (*)(unsigned long, unsigned long));

int main(void)
{
    unsigned long i = peak(8, less);
    printf("%lu\n", i);
    if ((i == 0 || array [i] >= array[i-1]) &&
            (i == 7 || array[i] >= array [i+1]))
        printf ("CORRECT\n");
    else printf ("WRONG\n");
    return 0;
}

unsigned long peak(unsigned long nel,
                   int (*less)(unsigned long i, unsigned long j))
{
    unsigned long i;
    if (less(1, 0)) return 0;
    if (less(nel-2, nel-1)) return nel-1;
    for(i = 1; i < nel - 1; i++) {
        if (less(i-1, i) && less(i+1, i)) return i;
    }
    return 0; //Сюда не должно быть передано управление, это лишь для компилятора
}
#endif // OLD_VERSIONS_H

