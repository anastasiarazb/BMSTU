#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

#ifdef DEBUG
#define out printf
#define show(low, high) {int i; for(i = low; i <= high; ++i) printf("%d ", base[i]); putchar('\n');}
#else
#define out
#define show(a, b)
#endif

void merge(int *base, int low, int med, int high);
void mergesort_rec(int *base, int low, int high);
void insertion_sort(int *base, int low, int high);
void mergesort(int *base, int nel);
int compare(int a1, int a2);

int main(void)
{
    int i, n;
    scanf("%d", &n);
    int *arr = (int *)malloc(n*sizeof(int));
    for(i = 0; i < n; ++i) scanf("%d", arr+i);
    mergesort(arr, n);
    for(i = 0; i < n; ++i) printf("%d ", arr[i]);
    free(arr);
    return 0;
}
int compare(int a1, int a2)
{
    return (abs(a1) > abs(a2));
}

//Аргументы: base - массив, low - индекс первого элемента, high - индекс последнего
void insertion_sort(int *base, int low, int high)
{
    out("low = %d, high = %d\n", low, high);
    int elem;            //Хранилище сравниваемого элемента
    int i, loc;          //Индексы. i - начало неотсортированной части, loc - конец отсортированной
    for(i = low+1; i <= high; ++i){
        for(elem = base[i], loc = i - 1; loc >= low && compare(base[loc], elem); --loc){
            base[loc+1] = base[loc];   //Сдвигаем все элементы на один вправо, пока они больше elem
        }
        base[loc+1] = elem;            //По окончании вставляем элемент на свое место
    }
}
//first - индекс начала первого подмассива, second - начало второго, end - последний элемент
void merge(int *base, int low, int med, int high)
{
    out("low  %d, med = %d, high = %d\n", low, med, high);
    int count_1 = low;           //Cчетчик по первому подмассиву [first...second]
    int count_2 = med+1;          //Cчетчик по второму подмассиву [second+1...high]
    int count_t = 0;               //Счетчик по доп. массиву
    show(count_1, med);
    show(count_2, high);
    int *temp = (int *)malloc((high-low+1)*sizeof(int));
    while(count_t < high-low+1){
        //Если мы еще в границах 1-го подмассива и вышли за второй массив
        // или мы в границах обоих подмассивов, и элемент из 1-го меньше,
        // записываем его в temp
        out("count1 = %d count2 = %d, > %d\n", count_1, count_2, compare(base[count_2], base[count_1]));
        if(count_1 <= med && (count_2 > high || !compare(base[count_1], base[count_2]))){
            temp[count_t++] = base[count_1++];
        } else {
            temp[count_t++] = base[count_2++];
        }
    }
    memcpy(base + low, temp, (high-low+1)*sizeof(int));
    free(temp);
}


void mergesort_rec(int *base, int low, int high)
{
    if(high - low > 4) {
        int med = (low+high)/2;
        mergesort_rec(base, low, med);
        mergesort_rec(base, med+1, high);
        merge(base, low, med, high);
    } else {
        insertion_sort(base, low, high);
    }
}
void mergesort(int *base, int nel)
{
    mergesort_rec(base, 0, nel-1);
}

/*
 Сортировка слиянием+вставками

В программе должен быть реализован алгоритм сортировки слиянием, рекурсивную
функцию которого нужно модифицировать таким образом, чтобы для
последовательностей длиной меньше 5 выполнялась сортировка вставками.
valgrind /home/nastasia/Qt/2module/mergesort-Debug/mergesort

6
7 -3 6 -4 8 0

0 -3 -4 6 7 8

7
5 -5 5 -5 5 -5 5


*/
