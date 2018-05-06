#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

#ifdef DEBUG
#define out printf
#define SHOW_HEAP(q) {int j; for(j = 0; j < (q)->count; ++j){ printf("%d ", (q)->heap[j].val);} putchar('\n');}
#else
#define out(...)
#define SHOW_HEAP(q)
#endif

#define alloc(n, type) (type *)malloc(n*sizeof(type))

//------------Реализация очереди с приоритетами------------

typedef char boolean;
typedef int pq_type;
typedef struct
{
    int val;    //Число
    int num;    //Номер строки, из которой взят элемент
} pair;

typedef struct Priority_queue
{
    pair *heap;
    int cap;   //Размер массива heap
    int count; //Количество элементов в очереди
    int (*cmp)(struct Priority_queue *, int, int);
} p_queue;

p_queue *init_priority_queue(int nel, int (*compare)(struct Priority_queue *, int, int))
{
    p_queue *q = alloc(1, p_queue);
    q->heap = alloc(nel, pair);
    q->cap = nel;
    q->count = 0;
    q->cmp = compare;
    return q;

}

void delete_priority_queue(p_queue *q)
{
    free(q->heap);
    q->cap = q->count = 0;
    free(q);

}

pq_type maximum(p_queue *q)
{
    if(!q->count){
        printf("Error: queue is empty.\n");
        exit(1);
    }
    return q->heap[0].val;
}

boolean empty_queue(p_queue *q)
{
    return !q->count;
}

void swap(p_queue *q, int a, int b)
{
    pair temp;
    temp = q->heap[a];
    q->heap[a] = q->heap[b];
    q->heap[b] = temp;
}

int compare_min(p_queue *q, int a, int b) //Сравнить элементы с индексами a и b
{
    return q->heap[a].val > q->heap[b].val;
}

void insert_p_queue(p_queue *q, pair x)
{
    int i = q->count;
    if(i == q->cap){
        q->cap *= 2;
        q->heap = (pair *)realloc(q->heap, q->cap*sizeof(pair));
    }
    ++q->count;
    q->heap[i] = x;
    while(i && q->cmp(q, (i-1)/2, i))
    {
        swap(q, (i-1)/2, i);
        i = (i-1)/2;
    }
}

//Восстановление свойств пирамиды.
void heapify(p_queue *q)
{
    int left, right, old_root;
    int i = 0, nel = q->count;
    while(1){
        left = 2*i + 1;       //l = 2i+1
        right = left + 1;     //r = 2i+2
        old_root = i;
        //Ищем наибольший элемент среди данного элемента и его потомков
        if (left < nel && q->cmp(q, i, left)){
            i = left;
        }
        if (right < nel && q->cmp(q, i, right)){
            i = right;
        }
        if (i == old_root){     //Текущий элемент и так оказался наибольшим -
            break;      //изменения не требуются
        }
        //Иначе ставим в корень наибольшего из потомков
        swap(q, i, old_root);
        //i-й элемент опустился и стал корневым для уровня ниже
    }
}

pair extract_min(p_queue *q)
{
    if(!q->count){
        printf("Error: queue is empty\n");
        exit(1);
    }
    pair temp = q->heap[0];
    --q->count;
    if(q->count){
        q->heap[0] = q->heap[q->count];
        heapify(q);
    }
    return temp;
}

void merge(p_queue *q, int *lens, int **place)
{
    pair temp = extract_min(q);
    printf("%d ", temp.val);
    --lens[temp.num];
    out("val = %d num = %d len = %d\n", temp.val, temp.num, lens[temp.num]);
    if(lens[temp.num]){    //Остаток подстроки не пуст
        ++place[temp.num]; //Передвинуть начало подстроки на один элемент
        temp.val = *place[temp.num];
        out("   push val = %d place[i][0] = %d, num = %d, len = %d\n", temp.val, place[temp.num][0], temp.num, lens[temp.num]);
        insert_p_queue(q, temp);
    }
    SHOW_HEAP(q);
}

int main(void)
{

    int n;
    scanf("%d", &n);
    int *lens = alloc(n, int); //Массив, содержащий длины подследовательностей
    int i, cur_len, sum_len = 0;
    for(i = 0; i < n; ++i){
        scanf("%d", &cur_len);
        if(cur_len){      //Если подстрока не пуста
            lens[i] = cur_len;
            sum_len += cur_len;
        } else {   //Если пуста, пропустить ее
            --n;
            --i;
        }
    }
    int **place = (int **)malloc(n*sizeof(int *) + sum_len*sizeof(int));
    place[0] = (int *)(place + n);        /*Переходим на ту область, где начинается массив строк*/
    int j;
    cur_len = lens[0];
    for(j = 0; j < cur_len; ++j){
        scanf("%d", place[0]+j);
    }
    for(i = 1; i < n; ++i){
        place[i] = place[i-1]+cur_len;     /*Отступаем от начала предыдущей строки на ее длину*/
        cur_len = lens[i];
        for(j = 0; j < cur_len; ++j){
            scanf("%d", place[i]+j);
        }
    }

    p_queue *Q = init_priority_queue(n, compare_min);
    pair new_pair;
//    Кладем в очередь первые (наименьшие) элементы каждого из подмассивов
    for(i = 0; i < n; ++i){
        new_pair.val = place[i][0];
        new_pair.num = i;
        insert_p_queue(Q, new_pair);
    }
    for(i = 0; i < sum_len; ++i){
        merge(Q, lens, place);
    }
    delete_priority_queue(Q);
    free(lens);
    free(place);
    return 0;
}


/*Слияние последовательностей
Условие
Баллы: 2

Составьте программу merge.c, объединяющую k отсортированных по возрастанию массивов целых чисел в один отсортированный массив за время O(n lg k), где n – общее количество элементов во всех входных массивах. Для слияния массивов воспользуйтесь очередью с приоритетами размера k.

Формат входных данных программы должен быть такой: первая строка, считываемая со стандартного потока ввода, содержит количество k массивов, вторая строка содержит последовательность целых чисел n1, n2, ..., nk, разделённых пробелами и задающих размеры массивов, каждая из следующих k строк описывает соответствующий массив, то есть i-тая строка содержит ni целых чисел, отсортированных по возрастанию и разделённых пробелами.

Программа должна выводить в стандартный поток вывода отсортированную по возрастанию последовательность целых чисел, полученную путём слияния массивов. Целые числа должны разделяться пробелами или символами перевода строки.

Например, рассмотрим следующие входные данные:

    4

    3 5 1 2

    10 12 20

    15 16 17 19 25

    20

    11 12

Для этих данных программа должна вывести

    10 11 12 12 15 16 17 19 20 20 25

valgrind ~/Qt/3-module/5-merge-priority-queue-Debug/5-merge-priority-queue

http://pastebin.com/pxafpjwh

*/
