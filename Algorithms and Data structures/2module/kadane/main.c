#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG

#ifdef DEBUG
#define out printf
#define newline putchar('\n');
#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
#define PRINTF_ARR(arr, nel, spec) {int i; for(i = 0; i < (nel); ++i) printf(spec, (arr)[i]); putchar('\n');}

#else
#define out(...)
#define newline
#define SHOW_RECURSION_DEPTH(i)
#define PRINTF_ARR(arr, nel, spec)
#endif

#define alloc(n, type) (type *)malloc(n*sizeof(type))
typedef double my_type;
void kadane1(my_type *arr, int nel /*,int *left, int right*/);
void kadane(my_type *arr, int nel /*,int *left, int right*/);
int main(void)
{
    int n;
    scanf("%d", &n);
    my_type *logs = alloc(n, my_type);
    int i, a, b;
    for(i = 0; i < n; ++i){
        scanf("%d/%d", &a, &b);
        logs[i] = log(a)-log(b);
    }
    kadane1(logs, n);
    kadane(logs, n);
    free(logs);
    return 0;
}


void kadane(my_type *arr, int nel /*,int *left, int right*/)
{
    printf("KADANE\n");
    int left, right, start, i;
    my_type sum = 0, maxsum = arr[0];
    sum = left = right = i = start = 0;
    while(i < nel){
        sum += arr[i];
        printf("sum > maxsum? %d  |", sum > maxsum);
        if(sum > maxsum){
            maxsum = sum;
            left = start;
            right = i;
        }
        printf("arr[%d] = %lf, ", i, arr[i]);
        ++i;
        if(sum < 0){
            sum = 0;
            start = i;
        }
        printf("sum = %lf, maxsum = %lf, l = %d, r = %d\n", sum, maxsum, left, right);
    }
    printf("%d %d\n", left, right);
}

void kadane1(my_type *arr, int nel /*,int *left, int right*/)
{
    out("KADANE reverse order\n");
    int left, right, start, i;
    my_type sum = 0, maxsum = arr[nel-1];
    sum = 0;
    left = right = i = start = nel-1;
    while(i >= 0){
        sum += arr[i];
        printf("sum > maxsum? %d  |", sum > maxsum);
        if(sum >= maxsum){
            maxsum = sum;
            left = i;
            right = start;
        }
        out("arr[%d] = %lf, ", i, arr[i]);
        --i;
        if(sum < 0){
            sum = 0;
            start = i;
        }
        out("sum = %lf, maxsum = %lf, l = %d, r = %d\n", sum, maxsum, left, right);
    }
    printf("%d %d\n", left, right);
}

/*Составьте программу maxprod.c, выполняющую поиск отрезка последовательности простых дробей {v }n-1 i 0, на котором произведение дробей максимально.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит размер последовательности n (0 < n ≤ 1000000). Во второй строке перечислены элементы последовательности. Каждый элемент записывается в виде a∕b, где a и b – неотрицательные целые числа (0 ≤ a ≤ 1000000, 0 < b ≤ 1000000). Элементы разделяются пробелами.

Формат результата работы программы

Программа должна вывести в стандартный поток вывода два числа l и r такие, что произведение ∏r vi i=l – максимально. Если возможно несколько решений, следует выбрать решение с минимальным l.

Пример работы программы


Входные данные


Выходные данные


10
4/3 2/1 8/3 0/1 10/3 8/3 1/10 1/5 50/7 1/8


4 5

valgrind ~/Qt/2module/kadane-Debug/kadane
10
4/3 2/1 8/3 0/1 10/3 8/3 1/10 1/5 50/7 1/8

7
1/8 2/1 3/1 4/1 0/1 12/1 2/1

1 3 - если делать double, то на последнем участке будет, что
sum > maxsum? 1  |arr[6] = 0.693147, sum = 3.178054, maxsum = 3.178054, l = 5, r = 6
хотя в обратном порядке на последней итерации считает правильно
sum > maxsum? 1  |arr[6] = 0.693147, sum = 3.178054, maxsum = 3.178054, l = 5, r = 6
для float оба раза считает правильно


*/
