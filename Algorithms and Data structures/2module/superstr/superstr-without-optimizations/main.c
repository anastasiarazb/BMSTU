#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 1000 //Число взято произвольно

#define SET_BIT(i) (1 << (i))
#define GET_BIT(arr, index) (1 & ((arr) >> (index)))

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

//char *place type **arr уже должны быть объявлены
#define MATRIX_MALLOC(place, arr, M, N, type) { \
    int i; \
    int col_size = (M)*sizeof(type *); \
    int str_size = (N)*sizeof(type); \
    place = (char *)malloc(col_size + (M)*str_size); \
    arr = (type **)(place); \
    for(i = 0; i < (M); ++i){ \
        arr[i] = (type *)((place) + col_size + i*str_size); \
    }\
  }

int pref_func2(char *str2, char *str1);
void permut(short arr, int nel);
short init_arr(int m);
short erase_q_arr(short arr, int q);
int *add_q_cort(int *cort, int q, int i);
void show_cort(int *cort, int nel);

char **container; //Хранилище строк
int **matrix;  //матрица смежности
int pi[MAX_LENGTH*2];

int main(void)
{
    int n, i;
    scanf("%d ", &n);
        // Выделение памяти под строки и считывание
    char *place1;
    MATRIX_MALLOC(place1, container, n, MAX_LENGTH, char);
    for (i = 0; i < n; ++i){
        scanf("%s", container[i]);
//        out("%s, ", container[i]);
    }
    char *place2;
    MATRIX_MALLOC(place2, matrix, n, n, int);
    int j;
    for(i = 0; i < n; ++i){
        for(j = 0; j < n; ++j){
            if(i == j){
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = pref_func2(container[i], container[j]);
            }

        }
    }
    permut(init_arr(n), n);
    // Освобождение выделенной ранее памяти   2 caacccca accaacccc
    free(place1);
    free(place2);
    return 0;
}
//Возвращает букву с текущим индексом из первого массива, если индекс меньше его длины
// и соответствующую букву из второго массива, если мы вышли за границу первого

#define STR1_t ((t < len1) ? str1[t] : str2[t - len1])

//Строим префиксную функцию для строки S, полученной конкатенацией
//двух строк. Наибольшая грань S = str2+str1 есть наибольшая
// длина пересечения строк str2 и str1
// Возвращает длину второй строки (которая при взятии пересечения идет первой),
// уменьшенную на длину пересечения
int pref_func2(char *str2, char *str1)
{
    //Сначала строим префиксную функцию только для первой строки
    int i, t;
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
    out("t = %d, pi[len-1] = %d\n", t, pi[i-1]);
    //Достраиваем префиксную функцию, используя вторую строку
    //как продолжение первой
    int len1 = i; //Длина первого слова
    for (; str2[i-len1]; ++i){
        while (t && STR1_t != str2[i-len1]){
            t = pi[t-1];
        }
        if (STR1_t == str2[i-len1]){
            ++t;
        }
        pi[i] = t;
    }
    int len2 = i - len1;
    //Если при наложении строк друг на друга мы получили отрицательный сдвиг,
    //переходим к следующей по длине грани и т.д.
    while(t > len2){
        out("pi[%d] = %d, ", t-1, pi[t-1]);
        t = pi[t-1];
    }
    out("str2 = %s str1 = %s\nSUM_LENGTH = %d, last pi = %d\n", str2, str1, i, pi[i-1]);
    out("LEN_OF_SECOND = %d, t = %d,  LEN_WITOUT_CROSS = %d\n\n", i - len1, t, len2 - t);
    return len2-t;
}

//Суммарная длина двух строк минус длина их пересечения
int cross_free(int str1, int str2)
{
    int cross = pref_func2(container[str2], container[str1]);
    out("str1 = %s, str2 = %s cross = %d\n", container[str1], container[str2], cross);
    return strlen(container[str1]) - cross;
}

int shortest; //Длина текущей кратчайшей суперстроки


//Подсчет длины суперстроки, получающейся из текущей перестановки строк
void count_least(int *cort, int nel)
{
    int i, current;
    for(i = current = 0; i < nel-1; ++i){
        current += matrix[cort[i]][cort[i+1]];
        printf("[%d:%d] matr = %d, %s %s\n", cort[i], cort[i+1], matrix[cort[i]][cort[i+1]], container[cort[i]], container[cort[i+1]]);
    }
    current += strlen(container[cort[i]]);
    printf("[%d] %s \nCURRENT_LEN = %d\n\n", cort[i], container[cort[i]], current);
    if(current < shortest){
        shortest = current;
    }
}

//Размещения без повторений из m элементов по nel-элементому множеству
void permut_rec(short arr, int nel, int counter, int *cort)
{
    if(counter == nel){
//        show_cort(cort, counter);
        count_least(cort, nel);
        newline;
    } else {
        int i;
        for(i = 0; i < nel; ++i){
            if(GET_BIT(arr, i)){
                permut_rec(erase_q_arr(arr, i), nel, counter + 1, add_q_cort(cort, i, counter));
            }
        }
    }
}

void permut(short arr, int nel)
{
    int *cort = (int *)calloc(nel, sizeof(int));
//    init_shortest(nel);
    shortest = 10*MAX_LENGTH;
    permut_rec(arr, nel, 0, cort);
    free(cort);
    out("SHORTEST = ");
    printf("%d\n", shortest);
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
//    printf("erase q = %c; ", massive[q]);
//    show_arr(arr - SET_BIT(q), massive, 4);
    return GET_BIT(arr, q) ? (arr ^ SET_BIT(q)) : arr;
}

int *add_q_cort(int *cort, int q, int i)
{
//    printf("ADD: size = %d, last = %c\n", cort[0], massive[q]);
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

//valgrind --leak-check=full --show-reachable=yes --track-origins=yes ~/Qt/superstr/superstr-without-optimizations-Debug/superstr-without-optimizations
//valgrind ~/Настя/Clang_2_module/1-superstr-Debug/1-superstr
//time /home/nastasia/Qt/superstr/superstr-without-optimizations-Debug/superstr-without-optimizations < ~/Qt/superstr/superstr-without-optimizations/test.txt
//time /home/nastasia/Qt/superstr/superstr-without-optimizations-Release/superstr-without-optimizations < ~/Qt/superstr/superstr-without-optimizations/test.txt
/*
8
caababb
cabbcc
bacccccbc
bacabbabbc
abbcca
bcaac
cbbcab
bccb

36


9
aabbcbba
cbbbcbbb
bbab
accaa
cccbb
acacccbaa
bcacaba
bbcbc
acabbcc

42*/

