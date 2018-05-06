/*
В классической сортировке пузырьком проход по сортируемой последовательности
осуществляется всегда в одном направлении. Модифицируйте алгоритм сортировки
пузырьком, чтобы в нём чередовались проходы по последовательности слева направо
и справа налево.

сортировка перемешиванием, двунаправленная пузырьковая сортировка, шейкерная
сортировка, пульсирующая сортировка (ripple sort), трансфертная сортировка
(shuttle sort), и даже сортировка «счастливый час» (happy hour sort).
*/
#include <stdlib.h>
#include <stdio.h>

int *array;

int compare(unsigned long i, unsigned long j)
{
        if (i <= j) {
            printf("COMPARE [%lu] [%lu] %d %d\n", i, j, array[i], array[j]);
        } else {
                printf("COMPARE [%lu] [%lu] %d %d\n", j, i, array[j], array[i]);
        }

        if (array[i] == array[j]) return 0;
        return array[i] < array[j] ? -1 : 1;
}

void swap(unsigned long i, unsigned long j)
{
        if (i <= j) {
                printf("SWAP [%lu] [%lu] %d %d\n", i, j, array[i], array[j]);
        } else {
                printf("SWAP [%lu] [%lu] %d %d\n", j, i, array[j], array[i]);
        }

        int t = array[i];
        array[i] = array[j];
        array[j] = t;
}

void bubblesort(unsigned long,
        int (*)(unsigned long, unsigned long),
        void (*)(unsigned long, unsigned long));

int main(void)
{
        int i, n;
        scanf("%d", &n);

        array = (int*)malloc(n * sizeof(int));
        for (i = 0; i < n; i++) scanf("%d", array+i);

        bubblesort(n, compare, swap);
        for (i = 0; i < n; i++) printf("%d ", array[i]);
        printf("\n");

        free(array);
        return 0;
}
void bubblesort(unsigned long nel,
                int (*compare)(unsigned long i, unsigned long j),
                void (*swap)(unsigned long i, unsigned long j))
{
    unsigned long i;
    unsigned long t;
    unsigned long bound_high = nel-1; //Верхняя граница неотсортированной части
    unsigned long bound_low = 0;      //Нижняя граница

    while(bound_high > bound_low){
        // Начиная от нижней границы, сравниваем соседние элементы
        for(t = i = bound_low; i < bound_high; i++){
            if(compare(i, i+1) == 1){
                swap(i, i+1);
                t = i;          //На выходе из цикла в t будет хранится индекс последней перемещенной переменной =>
            }                   // последовательность за t уже отсортирована
        }
        bound_high = t;

        // Проход "в другую сторону" начиная от верхней границы
        for(t = i = bound_high; i > bound_low; i--){
            if(compare(i, i-1) == -1){
                swap(i, i-1);
                t = i;
            }
        }
        bound_low = t;
    }
}

