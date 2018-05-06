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

//------------Реализация очереди с приоритетами------------

#define alloc_queue(n) (struct Priority_queue *)malloc(sizeof(struct Priority_queue) + (n)*sizeof(pq_type))

typedef char boolean;
typedef int pq_type;
typedef int time;

typedef struct
{
    time prepare;    //Время, за которое произойдет обработка данных
    time working;    //Предполагаемое время, за которое отработает задача
} pair;

typedef struct Priority_queue
{
    pq_type *heap;
    int cap;          //Размер массива heap
    int current_time; //Общее время работы
    int (*cmp)(struct Priority_queue *, int, int);
} *p_queue;

p_queue init_priority_queue(int nel, int (*compare)(struct Priority_queue *, int, int))
{
    p_queue q = alloc_queue(nel);
    q->heap = (pq_type *)(q+1);
//    p_queue q = (p_queue)malloc(sizeof(struct Priority_queue));
//    q->heap = (pq_type *)calloc(nel, sizeof(pq_type));
    memset(q->heap, 0, nel);
    q->cap = nel;
    q->current_time = 0;
    q->cmp = compare;
    return q;

}

void delete_priority_queue(p_queue q)
{
    free(q->heap);
    free(q);
    q = NULL;

}

boolean empty_queue(p_queue q)
{
    return !q->current_time;
}

void swap(p_queue q, int a, int b)
{
    pq_type temp;
    temp = q->heap[a];
    q->heap[a] = q->heap[b];
    q->heap[b] = temp;
}

int compare_min(p_queue q, int a, int b) //Сравнить элементы с индексами a и b
{
    return q->heap[a] > q->heap[b];
}

void heapify(p_queue q)
{
    int left, right, old_root;
    int i = 0, nel = q->cap;
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
#define update_current_time(i) \
    if(q->current_time < q->heap[i]){ \
        q->current_time = q->heap[i]; \
}

void put_on_claster(p_queue q, pair x)
{
    if(x.prepare <= q->heap[0]){  //Данные были подготовлены раньше, чем отработал кластер
        q->heap[0] += x.working;
    } else {
        q->heap[0] += x.prepare - q->heap[0] + x.working;
    }
    update_current_time(0);
    heapify(q);
}


//pair extract_min(p_queue q)
//{
//    if(!q->count){
//        printf("Error: queue is empty\n");
//        exit(1);
//    }
//    pair temp = q->heap[0];
//    --q->count;
//    if(q->count){
//        q->heap[0] = q->heap[q->count];
//        heapify(q);
//    }
//    return temp;
//}

int emul_cluster(p_queue q, pair *arr, int nel)
{
    int i;
    for(i = 0; i < q->cap; ++i){
        q->heap[i] = arr[i].prepare + arr[i].working;
        update_current_time(i);
    }
    heapify(q);
    for(; i < nel; ++i){
        put_on_claster(q, arr[i]);
    }
    return q->current_time;
}

int main(void)
{
    int n, m;//Количество узлов кластера и выполняемых задач
    scanf("%d %d", &n, &m);
    if(n > m){
        n = m; //Очередь, эмулирующая кластеры, не длиннее, чем число задач
    }
    p_queue Q = init_priority_queue(n, compare_min);
    pair *tasks = (pair *)malloc(m*sizeof(pair));
    for(n = 0; n < m; ++n){
        scanf("%d %d", &tasks[n].prepare, &tasks[n].working);
    }
    printf("%d\n", emul_cluster(Q, tasks, m));
//    delete_priority_queue(Q);
    free(Q);
    free(tasks);

    return 0;
}


/*Моделирование работы вычислительного кластера
Условие
Баллы: 2

Имеется вычислительный кластер, состоящий из N однопроцессорных узлов. На кластере нужно выполнить M задач. Каждая задача описывается парой ⟨t ,t ⟩ 1 2, где t1 – время в микросекундах от включения кластера, начиная с которого задачу можно посылать на выполнение (до этого времени входные данные для задачи неготовы); t2 – прогнозируемое время выполнения задачи в микросекундах.

Для выполнения каждой задачи задействуется один узел кластера, то есть задачи невозможно распараллеливать. Кроме того, нельзя менять порядок выполнения задач: если данные для задачи A оказываются подготовлены раньше, чем данные для задачи B, то задача A не может быть запущена позже задачи B.

Необходимо составить программу cluster.c, вычисляющую минимальное время в микросекундах от начала работы кластера, когда все задачи будут выполнены. В программе нужно использовать очередь с приоритетами для хранения времен окончания задач, запущенных на кластере.

Формат входных данных программы должен быть такой: первая строка, считываемая со стандартного потока ввода, содержит количество N узлов кластера, вторая строка содержит число M задач, каждая из следующих M строк содержит пару целых чисел ⟨t1,t2⟩, описывающих задачу. Пары чисел отсортированы в порядке возрастания t1, и, более того, в каждой паре t1 уникально.

Программа должна вывести в стандартный поток вывода целое число, выражающее время в микросекундах, прошедшее от включения кластера до момента, когда все задачи будут выполнены.

Примеры работы программы:


Входные данные
    Ответ

Входные данные
    Ответ



1
3
2 4
4 5
5 6
    17

2
3
1 3
6 4
13 5
    18


2
4
3 5
4 5
6 2
7 7
    16

3
4
2 5
3 5
4 5
5 3
    10


3
3
2 6
3 5
4 4
    8

3
5
1 5
2 5
3 5
4 5
9 1
    11


4
9
10000000 50000000
20000000 20000000
30000000 50000000
40000000 70000000
50000000 60000000
60000000 30000000
70000000 60000000
80000000 10000000
90000000 50000000
    150000000


http://pastebin.com/PeDs56N8

valgrind ~/Qt/3-module/6-cluster-priority-queue-Debug/6-cluster-priority-queue

*/
