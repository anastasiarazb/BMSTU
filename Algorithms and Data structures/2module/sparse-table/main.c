#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#define DEBUG

#ifdef DEBUG
#define out printf
#define newline putchar('\n');
#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
#define PRINTF_ARR(arr, nel, spec) {int i; for(i = 0; i < (nel); ++i) printf(spec, (arr)[i]); putchar('\n');}
#define PRINT_MATRIX(a, m, n, spec) {\
    int i, j; \
    for(i = 0; i < (m); ++i) { \
        for(j = 0; j < (n); ++j) { \
            printf(spec, (a)[i][j]); \
        } \
        putchar('\n'); \
    } \
    putchar('\n'); \
  }
#else
#define out(...)
#define newline
#define SHOW_RECURSION_DEPTH(i)
#define PRINTF_ARR(arr, nel, spec)
#define PRINT_MATRIX(a, m, n, spec)
#endif

#define MATRIX_MALLOC(arr, M, N, type) { \
    arr = (type **)calloc((M)*sizeof(type *) + (M)*(N)*sizeof(type), 1); \
    arr[0] = (type *)(arr + M); /*Переходим на ту область, где начинается массив строк*/\
    int i;                      \
    for(i = 1; i < (M); ++i){   \
       arr[i] = arr[i-1]+N;     /*Отступаем от начала предыдущей строки на ее длину*/\
    } \
 }

#define POW2(i) (1 << (i))
#define OP gcd   //Чтобы не случилось такого, что в программе определено несколько, и в build и query переданы разные

int *lg;
int **table;

void compute_logs(int m)  //m - max длина отрезка
{
    lg = (int *)malloc((POW2(m)+1)*sizeof(int));
    int i, j = 0;
    for(i = 1; i <= m; ++i){
        while(j <= POW2(i)){
            lg[j++] = i-1;
        }
    }
    PRINTF_ARR(lg, m, "%d ");
}

typedef int my_type;
typedef my_type (*bin_op)(my_type, my_type);

my_type **sparse_table_build(my_type *arr, int nel, int *lg, bin_op op);
my_type sparse_table_query(my_type **table, int l, int r, int *lg, bin_op op);


// /*-------------------------Бинарные операции--------------------------------*/
int min(int a, int b){
    return (a > b) ? b : a;
}

int gcd(int x, int y)
{
    int a, b;
    x = abs(x);
    y = abs(y);
    if(x > y){
        a = x;
        b = y;
    } else {
        a = y;
        b = x;
    }
    if(!b) return a;
    int r1 = a % b, r2 = b;
    while(r1){
        a = b;
        b = r2 = r1;
        r1 = a % b;
    }
    return r2;
}
// /*------------------------------------------------------------------*/

int main(void)
{
    int nel;
    scanf("%d", &nel);
    int *v = (int *)malloc(nel*sizeof(int));
    int i;
    for(i = 0; i < nel; ++i){
        scanf("%d", v+i);
    }
//    compute_logs(nel);
    table = sparse_table_build(v, nel, lg, OP);
    int l, r;
    scanf("%d", &nel);
    for(i = 0; i < nel; ++i){
        scanf("%d %d", &l, &r);
        printf("%d\n", sparse_table_query(table, l, r, lg, OP));
    }
    putchar('\n');
    free(lg);
    free(table);
    free(v);
    return 0;
}

typedef int my_type;
typedef my_type (*bin_op)(my_type, my_type);

my_type **sparse_table_build(my_type *arr, int nel, int *lg, bin_op op)
{
    int m = log2(nel)+1;
    out("m = %d\n", m);
    my_type **table;
    MATRIX_MALLOC(table, nel, m, my_type);
    int i;
    for(i = 0; i < nel; ++i){ //Заполнение первого столбца элентами массива.
        table[i][0] = arr[i]; //Можно было бы поменять в алгоритме местами строки и столбцы
    }                         //и использовать вместо первого цикла memcpy.
    int j;
    for(j = 1; j < m; ++j){
        for(i = 0; i <= nel-POW2(j); ++i){ //Т.к. каждый столбец короче nel на 2^j
            table[i][j] = op(table[i][j-1], table[i+POW2(j-1)][j-1]);
        }
    }
    PRINT_MATRIX(table, nel, m, "%d ");
    return table;
}

my_type sparse_table_query(my_type **table, int l, int r, int *lg, bin_op op)
{
    int j = log2(r-l+1);
    return op(table[l][j], table[r-POW2(j)+1][j]);
}

//valgrind ~/Qt/2module/sparse-table-Debug/sparse-table


/*---------------------Рабочие макросы-------------------------*/

//Выделение памяти под двумерный массив вида type arr[M][N];
//type **arr уже должен быть объявлен
//Работает и для calloc(..., 1);

/*---------------------Отладочные макросы-------------------------*/


/*Наибольший общий делитель подпоследовательности
Условие
Баллы: 3

Составьте программу rangegcd.c, вычисляющую наибольший общий делитель на различных интервалах последовательности целых чисел.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит размер последовательности n (0 < n ≤ 300000). Во второй строке перечислены элементы последовательности. Каждый элемент представляет собой целое число, находящееся в диапазоне от -109 до 109. Элементы разделяются пробелами.

Третья строка содержит общее количество запросов m (0 < m ≤ 1000000). Каждая из следующих m строк содержит запрос, который представляет собой два числа l и r, задающие границы интервала, на котором нужно вычислить наименьший общий делитель (0 ≤ l,r < n).

Формат результата работы программы

Для каждого запроса вывести в стандартный поток вывода наименьший общий делитель указанной подпоследовательности.

Пример работы программы


Входные данные


Выходные данные


10
-10 -2 5 60 80 100 77 65 33 45
6
0 9
0 1
2 5
3 5
6 8
8 9


1
2
5
20
1
3

2
0 10
3
0 0
1 1
0 1

0
10
10




28
7875 3575 -4389 -7975 -4785 -8349 -8855 -5225 -2530 -7714 -8265 4785 -4002 6279 9177 -7350 -9108 4002 5220 8892 -5577 -7308 -7569 6003 7650 8410 -4930 8190
28
11 26
22 27
11 18
23 27
12 12
4 17
23 25
8 18
2 16
22 27
9 16
19 24
0 0
11 24
0 6
16 23
1 12
27 27
21 27
18 19
14 22
7 14
14 15
20 20
15 15
3 12
8 14
27 27

1
1
3
1
4002
1
1
1
1
1
1
3
7875
3
1
3
1
8190
1
36
3
1
21
5577
7350
1
1
8190
ограничение -- 262144 Кб 268 435 456
*/
