#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

#ifdef DEBUG
//#include "read-sentence.c"

int main(void)
{
    return 0;
}

#else
#define MAX_LENGTH 1000 //Максимальная длина вводимого предложения, включая нуль-терминатор

void countsort(char *src, char *dest);

int main(void)
{
    char *source = (char *)calloc(MAX_LENGTH, 1); //Чтобы по окончании предложения за ним шли нули
    char *dest = (char *)calloc(MAX_LENGTH, 1);
//  Запись слов в предложение: каждое слово отделяется от другого нуль-терминатором,
//  пробелы пропускаются. Следующее предложение записывается в ячейку, следующую за
//  последним символом '\0'
    int letter;
    int i = 0;
        while((letter = getchar()) != '\n'){
    //        if(letter == ' ') continue;
            ungetc(letter, stdin);
            scanf(" %s", source+i);
            i += strlen(source+i)+1;
        }

//    countsort(source, dest);
    //Первое слово выводим без пробела
    memcpy(dest, source, MAX_LENGTH);
    printf(dest);
    for(i = strlen(dest) + 1; dest[i]; i+=strlen(dest+i)+1)
        printf (" %s", dest+i);               //Ставить пробел перед каждым следующим словом.

    putchar('\n');
    free(source);
    free(dest);
    return 0;
}

int sentence_len(char *src);

void countsort(char *src, char *dest)
{
    int nel = MAX_LENGTH;           //Или можно было nel = sentence_len, если бы MAX_LENGTH не была задана глобально
    int *count = (int *)calloc(nel, sizeof(int));//Хранилище итоговых индексов первых букв строки
    int i, j;                               //Индексы первых букв сравниваемых строк
    int len_j = strlen(src) + 1;            //Длины соответствующих строк, считая '\0'
    int len_i;
    //Для каждой j-той строки подсчитываем, сколько строк короче нее
    for(j = 0; src[j] && j < nel-len_j; j += len_j){
        len_j = strlen(src + j) + 1;
        i = j + len_j;                       //i указывает на следующее за j слово
        for(; src[i] && i < nel; i += len_i){
            len_i = strlen(src + i) + 1;
            if(len_j > len_i)
                count[j] += len_i;          //Итоговый индекс элемента увеличивается на длину i-й
            else count[i]+= len_j;          //(j-й) строки, которая будет стоять перед ним
        }
    }
    //Теперь i-й элемент в массиве count содержит итоговый индекс элемента,
    //т.е. позицию, на которой он должен стоять.
    //Далее инверсия перестановки count: i-й элемент начального массива записывается
    //в итоговый массив по индексу, хранящемуся в соответвующей ему ячейке count
//    i = 0;
//    len_i = strlen(src)+1;
    for(i = 0; src[i] < nel && i < nel; i += len_i){
        strcpy(dest+count[i], src+i);      //dest[count[i]] = scr[i]
        len_i = strlen(src + i) + 1;
    }
    free(count);
}

int sentence_len(char *src){
    int len = 0;
    while(src[len] || src[++len]); //Когда идут 2 подряд '\0' - остановка
    return len;
}
#endif

/* Обыкновенная реализация сsort для целых чисел
 *void countsort(int *base, int *dest, int nel, int width, int (*compare)(int i, int j));
int compare(int i, int j);

int compare(int i, int j)
{
    if(i == j) return 0;
    return i > j ? 1 : -1;
}

//Сначала выполняется отображение scr->count eq.
//P[i] -> i';
//потом scr->dest с индексами из count
//P[i] -> T[i'];
void countsort(int *src, int *dest, int nel, int width, int (*compare)(int, int))
{
    int *count = (int *)calloc(nel, width);
    int i, j;
    //Для каждого j-того элемента подсчитываем, сколько элементов меньше него
    for(j = 0; j < nel-1; j++){
        for(i = j+1; i < nel; i++){
            if(compare(src[i], src[j]) == -1)
                count[j]++;
            else count[i]++;
        }
    }
    //Теперь i-й элемент в массиве count содержит итоговый индекс элемента,
    //т.е. позицию, на которой он должен стоять.
    //Далее инверсия перестановки count: i-й элемент начального массива записывается
    //в итоговый массив по индексу, хранящемуся в соответвующей ему ячейке count
    for(i = 0; i < nel; i++)
        dest[count[i]] = src[i];
    free(count);
}
*/
/*В качестве параметров функция csort принимает указатель на исходное предложение src и указатель на пустой буфер dest подходящего размера. В результате работы функции в буфер dest записывается новое предложение, состоящее из слов, взятых из исходного предложения и отсортированных в порядке возрастания их длин. При этом слова в новом предложении разделяются одним пробелом.

Рассмотрим пример работы функции csort. Пусть исходное предложение выглядит как

    qqq  www  t  aa rrr  bb  x y zz

Тогда в выходной буфер должно быть записано предложение

    t x y aa bb zz qqq www rrr


valgrind ~/Qt/2module/countsort-Debug/countsort

*/
