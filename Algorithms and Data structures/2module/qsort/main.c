#include <stdio.h>
#include <stdlib.h>

int partition(int *base, int low, int high, /*size_t width,*/
              int (*cmp)(const void *, const void *));
void quicksort(int *base, int nel, int m, int (*cmp)(const void *, const void *));
void selectsort(int *base, int nel,
                int (*cmp)(const void *, const void *));

int compare(const int *elem1, const int *elem2)
{
    if(*elem1 == *elem2) return 0;
    return (*elem1 > *elem2) ? 1 : -1;
}
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int main(void)
{
    int n, m;
    scanf("%d%d", &n, &m);
    int *arr = (int *)malloc(n*sizeof(int));
    int i;
    for (i = 0; i < n; ++i) scanf("%d", arr + i);
    quicksort(arr, n, m,
              (int (*)(const void*, const void *))compare);
    for (i = 0; i < n; ++i) printf("%d ", arr[i]);
    free(arr);
    return 0;
}

int partition(int *base, int low, int high, /*size_t width,*/
              int (*cmp)(const void *, const void *))
{
    int i, j;                    //i - кол-во элементов, меньших опорного base[high]
    for(j = i = low; j < high; ++j){
        if(cmp(base + j, base + high) == -1){
            swap(base+i, base+j);
            ++i;
        }
    }
    swap(base+i, base+high);
//    printf("PARTITION: ");
//    int counter;
//    for(counter = low;counter <=high; ++counter) printf("%d ", base[counter]);
//    printf("  (i = %d)", i);
//    putchar('\n');
    return i;
}

/*
void quicksort_rec(int *base, int low, int high, int m,
                   int (*compare)(const void *, const void *))
{
    while (high - low > m){                         //Улучшения: if заменен на while
//        printf("%d %d, length = %d\n", low, high, high-low+1);
        int q = partition(base, low, high, compare);
        quicksort_rec(base, low, q-1, m, compare);
        low = q + 1;                                //вместо распараллеленной рекурсии - вызов в цикле
//        printf("я в цикле\n");
    }                                               //А не quicksort_rec(base, q + 1, high, compare)
    selectsort(base + low, high-low+1, compare);
}*/

void quicksort_rec(int *base, int low, int high, int m,
                   int (*compare)(const void *, const void *))
{
    while (high - low > m){
        int q = partition(base, low, high, compare);
        int len1 = q - 1 - low, len2 =  high - low + 1;
        if (len2 > len1){
            quicksort_rec(base, low, q-1, m, compare);
            low = q + 1;
        } else {
            quicksort_rec(base, q+1, high, m, compare);
            high = q - 1;
        }
    }
    selectsort(base + low, high-low+1, compare);
}


void quicksort(int *base, int nel, int m, int (*cmp)(const void *, const void *))
{
    quicksort_rec(base, 0, nel-1, m, cmp);
}

void selectsort(int *base, int nel, int (*cmp)(const void *, const void *))
{
//    printf("SELECTSORT: nel = %d\n  before:", nel);
//    int counter = 0; while(counter < nel) printf("%d ", base[counter++]);
//    putchar('\n');
    if(nel == 0) return;
    int i, j;           //j - последний элемент неотсортированной подпоследовательности
    int maxelem;
    for(j = nel-1; j; --j){
        //Поиск максимального элемента в подпоследовательности
        for(maxelem = j, i = 0; i < j; ++i){
//            printf(" base + %d = %d base + %d = %d\n", i, base[i], maxelem, base[maxelem]);
            if(cmp(base + i, base + maxelem) == 1)
                maxelem = i;
        }
        //Установка перемещение максимального элемента в конец
        //неотсортированной подпоследовательности
        swap(base + maxelem, base + j);
    }
//    printf("\n  after:");
//    counter = 0; while(counter < nel) printf("%d ", base[counter++]);
//    putchar('\n');
}
/*
 *В программе должен быть реализован алгоритм быстрой сортировки, рекурсивную
 *функцию которого нужно модифицировать таким образом, чтобы, во-первых, для
 *последовательностей длиной меньше m выполнялась сортировка прямым выбором, а во-вторых,
 *глубина стека вызовов была равна O(lg n), где n – размер массива.
 *void *base, size_t nel, size_t width,
        int (*compare)(const void *a, const void *b)
  valgrind --leak-check=full ~/Qt/2module/qsort-Debug/qsort
*/
