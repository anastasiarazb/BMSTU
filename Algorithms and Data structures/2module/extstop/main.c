#include <stdio.h>
#include <stdlib.h>

// Выделение памяти под двумерный массив размером MxN;
//type **arr уже должен быть объявлен
#define MATRIX_MALLOC(arr, M, N, type) { \
    int i; \
    int col_size = (M)*sizeof(type *); \
    int str_size = (N)*sizeof(type); \
    char *place = (char *)malloc(col_size + (M)*str_size); \
    arr = (type **)place; \
    char *strings = place + col_size; \
    for(i = 0; i < (M); ++i){ \
        arr[i] = (type *)(strings + i*str_size); \
    }\
  }
#define MATRIX_FREE(arr) free((char *)arr); //Т.к. arr = (type **)place;

#define newline {putchar('\n');}

#define MAXLEN 100
#define SIZE 26         //Alphabeth size
#define LEN(l) for((l) = 0; S[l]; ++(l));
#define KEY(c, str) (str[c]-'a')

char *T;
char *S;
int **delta1; //Таблица стоп-символов
int lenS;
int lenT;

void delta1_func(void);
int bmall(void);

int main(int argc, char **argv)
{
    char *place1;
    if (argc == 3) {
        S = argv[1];
        T = argv[2];
    } else {
        place1 = (char *)malloc(MAXLEN*2);
        S = place1;
        T = place1 + MAXLEN;
        scanf("%s%s", S, T);
    }
    LEN(lenS);
    LEN(lenT);
    for(lenT = 0; T[lenT]; ++lenT);
    MATRIX_MALLOC(delta1, lenS, SIZE, int);
    printf("%d\n", bmall());
    if (argc != 3){
        free(place1);
    }
    MATRIX_FREE(delta1);
    return 0;
}

void delta1_func(void)
{
    int a, i;
    for(i = 0; i < lenS; ++i){
        for(a = 0; a < SIZE; ++a)
            delta1[i][a] = lenS;
    }
    for(i = 0; i < lenS; ++i){
        for(a = 0; a <= i; ++a){
          delta1[i][KEY(a, S)] = lenS - 1 - a;
        }
    }

//    printf("i|");
//    for (a = 0; a < SIZE; ++a)
//        printf("  %c", 'a'+ a);
//    newline;
//    for(i = 0; i < lenS; ++i){
//        printf("%d|", i);
//        for (a = 0; a < SIZE; ++a){
//            printf(" %2d", delta1[i][a]);
//        }
//        newline;
//    }
}


int bmall(void)
{
    delta1_func();
    int k, i;
    for(k = lenS - 1 ; k < lenT; k += delta1[i][KEY(k, T)]) {
        for(i = lenS - 1; T[k] == S[i]; --i, --k)
            if(i == 0) {;
                return k;
            }
    }
    return lenT;
}
/*
Существует модификация алгоритма Бойера-Мура, в которой эвристика стоп-символа расширена следующим образом:

Расширенная эвристика стоп-символа. Встретив в строке T символ x = T[k] такой,
что x≠S[i], мы можем расположить строку S относительно строки T так, что
последнее вхождение x в S, расположенное левее S[i], окажется напротив T[k].

Таблица δ1 для эффективной реализации расширенной эвристики стоп-символа должна
представлять собой матрицу размера len(S) × size, где size – размер алфавита.
При неудачном сравнении символов S[i] и T[k] алгоритм Бойера-Мура должен
прочитать смещение для переменной k из δ1[i,T [k]].

Составьте программу extstop.c, осуществляющую поиск первого вхождения подстроки
S в строку T. В программе должен быть реализован вариант алгоритма Бойера-Мура,
в котором не используется эвристика совпавшего суффикса, а эвристика
стоп-символа расширена приведённым выше способом.

Строки S и T должны передаваться в программу через аргументы командной строки.
Программа должна вывести в стандартный поток вывода индекс первого символа
первого вхождения S в T. Если такого вхождения нет, программа должна вывести
len(T).
abadba
valgrind ~/Qt/2module/extstop-Debug/extstop
*/
