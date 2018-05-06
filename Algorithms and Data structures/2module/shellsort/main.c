// valgrind /home/nastasia/Qt/2module/shellsort-Debug/shellsort

//Метод Шелла является модификацией сортировки вставками, в которой переменная
//loc на каждой итерции внутреннего цикла уменьшается на некоторое число d ≥ 1.
//При этом фактически сортировка выполняется несколько раз для всё меньших и
//меньших значений d до тех пор, пока d не станет равно 1. Тем самым, сначала
//выполняется серия «грубых» сортировок, которые не дают точного ответа, но
//делают последовательность более упорядоченной, обеспечивая более быстрое
//выполнение финальной точной сортировки при d = 1.

//nel – количество элементов в последовательности; compare – указатель на
//функцию сравнения, которая возвращает -1, если i-тый элемент меньше j-того, 0
//– в случае, если i-тый элемент равен j-тому, и 1 – в случае, если i-тый
//элемент больше j-того; swap – указатель на функцию обмена i-того и j-того
//элементов последовательности. Значения расстояния d в ходе работы функции
//должны образовывать последовательность Фибоначчи (естественно, записанную
//задом наперёд). Первое значение в этой последовательности должно быть
//максимальным числом Фибоначчи, которое меньше значения параметра nel.

//(loc = i; loc >= d && (compare(loc-d, loc) == 1); loc--) нафига нам вложенные циклы? и точно ли для
//всех пар до начала можно использовать сразу, что до после какого-то значения они уже упорядочены?
#include <stdlib.h>
#include <stdio.h>

//#define DEBUG

#ifdef DEBUG
#define out printf
#else
#define out
#endif

int *array;

int compare(unsigned long i, unsigned long j)
{
        if (i <= j) {
                printf("COMPARE␣%d␣%d\n", i, j);
        } else {
                printf("COMPARE␣%d␣%d\n", j, i);
        }

        if (array[i] == array[j]) return 0;
        return array[i] < array[j] ? -1 : 1;
}

void swap(unsigned long i, unsigned long j)
{
        if (i <= j) {
                printf("SWAP␣%d␣%d\n", i, j);
        } else {
                printf("SWAP␣%d␣%d\n", j, i);
        }

        int t = array[i];
        array[i] = array[j];
        array[j] = t;
}

void shellsort(unsigned long,
        int (*)(unsigned long, unsigned long),
        void (*)(unsigned long, unsigned long));

int main()
{
        int i, n;
        scanf("%d", &n);

        array = (int*)malloc(n * sizeof(int));
        for (i = 0; i < n; i++) scanf("%d", array+i);

        shellsort(n, compare, swap);
        for (i = 0; i < n; i++) printf("%d␣", array[i]);
        printf("\n");

        free(array);
        return 0;
}


unsigned long *fib; //массив с числами фибоначчи
unsigned long make_fib_array(unsigned long nel);


void shellsort(unsigned long nel, int (*compare)(unsigned long i, unsigned long j),
               void (*swap)(unsigned long i, unsigned long j)){

    unsigned long i, d, loc;
    unsigned long j;                //Индекс числа в фибоначчиевой строке
    j = make_fib_array(nel);        //Заполнить массив числами Фибоначчи и установить счетчик на последний элемент
    for(;j; --j){                     //fib[] = {0, 1 ...} => на последней итерации fib[1] = 1. Двигаемся до конца.
        d = fib[j-1];                 //Взятие ширины шага из массива с числами Фибоначчи
        out("fib[%lu-1] = %lu\n", j, d);
        for(i = d; i < nel; ++i){
            for(loc = i-d; (compare(loc+d, loc) == -1); loc-=d){    //Для каждой пары чисел, отстоящих друг от друга на расстояние d,
                swap(loc+d, loc);
                if(loc < d) break;
            }
        }
    }
    free(fib);
}

unsigned long len_fib_array(unsigned long nel)
{
    unsigned long fib1 = 1, fib2 = 2, temp, i;
    if (nel < 4) return nel;
    for (i = 1; fib2 < nel; ++i){
        temp = fib2;
        fib2 += fib1;
        fib1 = temp;
    }
    return i;
}
unsigned long make_fib_array(unsigned long nel)
{
    unsigned long i;
    unsigned long len = len_fib_array(nel);
    fib = (unsigned long *)malloc((len+2)*sizeof(unsigned long));
    fib[0] = 1;     //Ввод первых чисел.
    fib[1] = 2;
    for(i = 1; i < len-1; ++i){
        fib[i+1] = fib[i] + fib[i-1];
    }/*
    out("fibstr len = %lu\nFib-elems:", len);
    for(i = 0; i < len; ++i){
        out("%lu ", fib[i]);
    }
    putchar('\n');*/
    return len; //Вернуть индекс наибольшего элемента
}


/*5
2 5 -1 -1 8

COMPARE 0 3
SWAP 0 3
COMPARE 1 4
COMPARE 0 2
COMPARE 1 3
SWAP 1 3
COMPARE 2 4
COMPARE 0 1
COMPARE 1 2
SWAP 1 2
COMPARE 0 1
COMPARE 2 3
COMPARE 3 4
-1 -1 2 5 8
*/
