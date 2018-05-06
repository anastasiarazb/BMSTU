#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 100 //Число взято произвольно

/*---Обработка множества, хранящегося в виде целого числа----*/

#define SET_BIT(i) (1 << (i))
#define GET_BIT(arr, index) (1 & ((arr) >> (index)))

#define INIT_ARR(m) (SET_BIT(m)-1) //Отметить все э-ты как принадлежащие множеству размера m
//Возвращает массив с удаленным элементом q
#define ERASE_q_ARR(arr, q) (GET_BIT(arr, q) ? ((arr) ^ SET_BIT(q)) : arr)

//#define DEBUG

#ifdef DEBUG
#define out printf
#define newline putchar('\n');
#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}

#else
#define out(...)
#define newline
#define SHOW_RECURSION_DEPTH(i)
#endif

// Выделение памяти под двумерный массив размера MxN;
//type **arr уже должен быть объявлен
#define MATRIX_MALLOC(arr, M, N, type) { \
    int i; \
    arr = (type **)malloc((M)*sizeof(type *) + (M)*(N)*sizeof(type)); \
    arr[0] = (type *)(arr + M);\
    for(i = 1; i < (M); ++i){ \
       arr[i] = arr[i-1]+N; \
    }\
  }
#define MATRIX_FREE(arr) free(arr); //Т.к. arr = (type **)place; => arr == place

typedef unsigned char byte;

byte pref_func1(char *str1);
byte pref_func2(char *str2, char *str1);
void init_matrix(int n);
void permut(short arr, int nel);
short init_arr(int m);
short erase_q_arr(short arr, int q);
int *add_q_cort(int *cort, int q, int i);
void show_cort(int *cort, int nel);

char **container; //Хранилище строк
byte **matrix;  //матрица смежности
byte *pref/*[MAX_LENGTH*2+1]*/;
byte *pi;

int main(void)
{
    int n, i;
    scanf("%d ", &n);
        // Выделение памяти под строки и считывание
    MATRIX_MALLOC(container, n, MAX_LENGTH, char);
    for (i = 0; i < n; ++i){
        scanf("%s", container[i]);
//        out("%s, ", container[i]);
    }
    MATRIX_MALLOC(matrix, n, n, byte);
    init_matrix(n);
    permut(INIT_ARR(n), n);
    // Освобождение выделенной ранее памяти
    MATRIX_FREE(container);
    MATRIX_FREE(matrix);
    return 0;
}


/*---------Построение префиксных функций и вычисление длины первой строки-----------
---------------за вычетом длины пересечения со сторой строкой----------------------*/

//Строим префиксную функцию для строки S, полученной конкатенацией
//двух строк. Наибольшая грань S = str1+str2 есть наибольшая
// длина пересечения строк str2#str1

// Сначала строим обыкновенную префиксную функцию для второй строки пересечения (str1)
// (она же первая подстрока строки S) в pref_func1, потом достраиваем pi для всей S.
// Так как нужно вычислять каждое пересечение str_x#str1, то имеет смысл заполнять матрицу
// построчно, сначала вычислив один раз префиксную функцию для str1, а потом достраивая
// для каждой другой строки.
// Длину строки, являющуюся последним вычисленным индексом i по завершении внешнего цикла
// также удобно хранить первым элементом массива, чтобы не тратить время на ее последующее
// вычисление (поэтому оставляем дополнительную ячейку в массиве
//  pref = {длина второй строки пересечения, собственно массив pi)


byte pref_func1(char *str1)
{
    out("PREF_FUNC_1 %s\n", str1);
    pi = pref + 1;
    byte i, t;
    pi[0] = t = 0;
    for (i = 1; str1[i]; ++i){
        while (t && str1[t] != str1[i]){
            t = pi[t-1];
        }
        if (str1[t] == str1[i]){
            ++t;
        }
        pi[i] = t;
    }
    out("t = %d, pi = %d", t, pi[i-1]);
    return pref[0] = i;
}

//Возвращает букву с текущим индексом из первого массива, если индекс меньше его длины
// и соответствующую букву из второго массива, если мы вышли за границу первого

#define STR1_t ((t < len1) ? str1[t] : str2[t - len1])

// Возвращает длину второй строки (которая при взятии пересечения идет первой),
// уменьшенную на длину пересечения
byte pref_func2(char *str2, char *str1)
{
//    pref_func1(str1);
//    out("PREF_FUNC str2 = %s, str1 = %s\n", str2, str1);
    out("PREF_FUNC_2 S1 = %s, S2 = %s\n", str1, str2);
    //Достраиваем префиксную функцию, используя вторую строку
    //как продолжение первой
    byte len1 = pref[0]; //Длина первого слова
    byte i, t = pi[len1-1];
    out("len1 = %d, pi[%d] = %d\n", len1, len1, t);
    for (i = len1; str2[i-len1]; ++i){
        while (t && STR1_t != str2[i-len1]){
            t = pi[t-1];
        }
        if (STR1_t == str2[i-len1]){
            ++t;
        }
        pi[i] = t;
    }
    byte len2 = i - len1;
    //Если при наложении строк друг на друга мы получили отрицательный сдвиг,
    //переходим к следующей по длине грани и т.д.
    while(t > len2){
//        out("pi[%d] = %d\n", t-1, pi[t-1]);
        t = pi[t-1];
    }
//    out("str2 = %s str1 = %s\nSUM_LENGTH = %d, last pi = %d\n", str2, str1, i, pi[i-1]);
    out("len1 = %d. len2 = %d\n", len1, len2);
    out("LEN_OF_SECOND = %d, LEN_WITOUT_CROSS = %d\n\n", len2, len2 - t);
    return len2-t;
}

/*--------------Заполнение матрицы смежности ----------*/
//Построчно, чтобы не вычислять n раз префиксную функцию для одной и той же строки

void init_matrix(int n)
{
    int i, j;
    pref = (byte *)malloc(2*MAX_LENGTH+1);
    for(j = 0; j < n; ++j){
//        printf("PI1 = %s\n", container[j]);
        pref_func1(container[j]);
        for(i = 0; i < n; ++i){
            if(i != j){
//                printf("i = %d, j = %d, P2(i, j) - %s, %s\n", i, j, container[i], container[j]);
                matrix[i][j] = pref_func2(container[i], container[j]);
            } else {
                matrix[i][j] = 0;
            }
        }
    }
    free(pref);
}

/*------------Вычисление перестановок и длины кратчайшей суперстроки---------*/

int shortest; //Длина текущей кратчайшей суперстроки


//Подсчет длины суперстроки, получающейся из текущей перестановки строк
void count_least(int *cort, int nel)
{
    int i, current;
    for(i = current = 0; i < nel-1; ++i){
        current += matrix[cort[i]][cort[i+1]];
    }
    current += strlen(container[cort[i]]);
    out("CURRENT_LEN = %d", current);
    if(current < shortest){
        shortest = current;
    }
}

//Размещения без повторений из m элементов по nel-элементому множеству
void permut_rec(short arr, int nel, int counter, int last, int sum)
{
    if(counter == nel){
//        SHOW_RECURSION_DEPTH(counter);
//        printf("sum = %d, last = %d, strlen(container[last]) = %d, %s  ",
//               sum, last, (int)strlen(container[last]), container[last]);
        sum += strlen(container[last]);
//        printf("sum = %d\n", sum);
        if(sum < shortest){
            shortest = sum;
        }
//        newline;
    } else {
        int i, new_sum;
        for(i = 0; i < nel; ++i){
            if(!counter){
                last = i;
                new_sum = sum;
            }
            if(GET_BIT(arr, i)){

//                SHOW_RECURSION_DEPTH(counter);
//                printf("[%d:%d], matrix[last][i] = %d, %s %s\n",
//                      last, i, matrix[last][i], container[last], container[i]);
                new_sum = sum + matrix[last][i];
                permut_rec(ERASE_q_ARR(arr, i), nel, counter + 1, i, new_sum);
            }
        }
    }
}

void permut(short arr, int nel)
{
//    int *cort = (int *)calloc(nel, sizeof(int));
    shortest = 10*MAX_LENGTH;
    permut_rec(arr, nel, 0, 0, 0);
    printf("SHORTEST: %d\n", shortest);
//    free(cort);
}

//-------------Функции обработки множества, хранящегося в цeлом числе и кортежа, хрян. в массиве----------

//Отметить все элементы, как принадлежащие множеству
short init_arr(int m)
{
    return SET_BIT(m)-1;
}

//Возвращает массив с удаленным элементом q
short erase_q_arr(short arr, int q)
{
    return GET_BIT(arr, q) ? (arr ^ SET_BIT(q)) : arr;
}

int *add_q_cort(int *cort, int q, int i)
{
    cort[i] = q;
    return cort;
}

void show_cort(int *cort, int nel)
{
    int i;
    for(i = 0; i < nel; ++i){
        printf("[%d]%s ", cort[i], container[cort[i]]);
    }
    newline;
}

//valgrind --leak-check=full --show-reachable=yes --track-origins=yes ~/Qt/superstr-Release/superstr < /home/nastasia/Qt/superstr/superstr-without-optimizations/test.txt
//time ~/Qt/superstr-Debug/superstr < /home/nastasia/Qt/superstr/superstr-without-optimizations/test.txt
//time ~/Qt/superstr-Release/superstr < /home/nastasia/Qt/superstr/superstr-without-optimizations/test.txt

//time /home/nastasia/Qt/superstr/superstr-without-optimizations-Release/superstr-without-optimizations < ~/Qt/superstr/superstr-without-optimizations/test.txt
/*
10
acbabcaac
cbca
bbaa
bcacaab
acbcccab
bcbcbab
bbbccbabca
accccabba
cbaca
accaabaaab

7 -- 40

6
acbabcaac
cbca
bbaa
bcacaab
acbcccab
bcbcbab 31

5
acbabcaac
cbca
bbaa
bcacaab
acbcccab 25

4
acbabcaac
cbca
bbaa
bcacaab 19 и 19

4
acbabcaac
cbca
bbaa
acbcccab  21 22

3
acbabcaac
cbca
acbcccab 18 19

2 cbca
acbcccab
*/
