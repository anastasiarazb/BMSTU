#ifndef OLD_H
#define OLD_H
/*
Составьте функцию hsort, выполняющую пирамидальную сортировку произвольного
массива. В качестве параметров функция hsort принимает указатель на начало
массива base, количество элементов массива nel, размер одного элемента width и
указатель на функцию сравнения compare.

Итоговая программа heapsort.c должна сортировать массив строк в порядке
возрастания количества букв a в строке. Программа должна считывать из
стандартного потока ввода размер и элементы массива, и выводить в стандартный
поток вывода результат сортировки.

valgrind --leak-check=full --show-reachable=yes --track-origins=yes ~/Qt/2module/heapsort-Debug/heapsort
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 100 //Максимальная длина вводимой строки, включая завершающий символ

int compare(const void *a, const void *b);
size_t howManyA(const char *str);
void swap(void *base, size_t i, size_t j, size_t width);
void hsort(void *base, size_t nel, size_t width,
        int (*compare)(const void *a, const void *b));
void buildHeap(void *base, size_t nel, size_t width,
               int (*compare)(const void *, const void *));
void heapify(void *base, size_t i, size_t nel, size_t width,
             int (*compare)(const void *, const void *));


int main(void)
{
    int number;                              //Количество строк
    scanf("%d", &number);

    char *superstr = (char *)malloc(number*MAX_LINES); //Хранилище строк
    // Считываем строки и записываем их в superstr как подстроки с индексами i,
    // отстоящие друг от друга на MAX_LINES char'ов
    int i;
    for (i = 0; i < number; i++)
        scanf("%s", superstr+i*MAX_LINES);

    hsort(superstr, number, MAX_LINES, compare);

    for (i = 0; i < number; i++)
        printf("%s\n", superstr+i*MAX_LINES);

    free(superstr);
    return 0;
}

void hsort(void *base, size_t nel, size_t width,
           int (*compare)(const void *, const void *))
{
    buildHeap(base, nel, width, compare);
    size_t i;
    for (i = nel-1; i ; i--){
        swap(base, 0, i, width);              //Нулевой элемент пирамиды максимальный - ставим его в конец
        heapify(base, 0, i, width, compare);  //и восстанавливаем свойство пирамиды от начала и до конца
    }                                         //неотсортированной части
}


//Построение массива - начинаем с i = nel/2, так как для элементов правее не существует сыновей
//с индексами 2i+1 и 2i+2 => они листья и удовлетворяют условию дерева.
//С каждой следующей итерацией сдвигаемся влево, а справа - уже упорядоченная пирамида, т.е работает
//heapify

void buildHeap(void *base, size_t nel, size_t width,
               int (*compare)(const void *, const void *))
{
    size_t i = (nel > 1) ? (nel/2-1) : 0;
//    size_t i = nel/2 - 1;
    for(; i; --i) {
        heapify(base, i, nel, width, compare);
    }
//    size_t k;
//    for (k = 0; k < nel; k++)
//        printf("%s\n", (char *)base+k*width);
}

//Восстановление свойств пирамиды.
//Переупорядочивает массив так, чтобы i-й элемент являлся корнем пирамиды
//Работает только если элементы 2i+1 и 2i+2 уже являются корнями пирамид
void heapify(void *base, size_t i, size_t nel, size_t width,
             int (*compare)(const void *, const void *))
{
    size_t left, right, old_root;
    while(1){
        left = 2*i + 1;       //l = 2i+1  /i-й элемент в нашем случае - это i-я подпоследовательность
        right = left + 1;     //r = 2i+2  /т.е. его реальный индекс в width раз больше,
        old_root = i;                           //поэтому base[i] = *(base+left*width)
        //Ищем наибольший элемент среди данного элемента и его потомков
        if (left < nel && (compare(base+i*width, base+left*width) == 1))
            i = left;
        if (right < nel && (compare(base+i*width, base+right*width) == 1))
            i = right;
        if (i == old_root)     //Текущий элемент и так оказался наибольшим -
            break;      //изменения не требуются
        //Иначе ставим в корень наибольшего из потомков
        swap(base, i, old_root, width);
        //i-й элемент опустился и стал корневым для уровня ниже
    }
}

//Возвращает 0, если количество вхождений 'a' в a == b, 1, если a < b и -1, если a > b
int compare(const void *a, const void *b)
{
    const char *str_a = a;
    const char *str_b = b;
    size_t counter_a = howManyA(str_a);
    size_t counter_b = howManyA(str_b);

//    printf("COMPARE %s %s (%zd) (%zd)\n", str_a, str_b, counter_a, counter_b);

    if(counter_a == counter_b) return 0;
    return (counter_a < counter_b) ? 1 : -1;
}

//Считает количество вхождений символа 'a' в строку str
size_t howManyA(const char *str)
{
    size_t counter = 0;
    size_t i = 0;
//    printf("%s", str);
    while(str[i]){
        if(str[i++] == 'a'){
            counter++;
        }
    }
//    printf("%zd\n", counter);
    return counter;
}

void swap(void *base, size_t i, size_t j, size_t width)
{
//    printf("SWAP %s %s\n", (char*)(base+i*width), (char*)(base+j*width));
    void *buff = malloc(width);
    memcpy(buff, base+i*width, width);
    memcpy(base+i*width, base+j*width, width);
    memcpy(base+j*width, buff, width);
    free(buff);
}



#endif // OLD_H
