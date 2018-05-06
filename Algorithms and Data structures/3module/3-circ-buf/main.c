#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------Реализация очереди через кольцевой буфер-------------
#define alloc(n, type) (type *)malloc(n*sizeof(type))
typedef int q_type;
typedef char boolean;

typedef struct CB_Queue
{
    q_type *data;
    int cap;   //Размер массива
    int count; //Количество элементов в очереди
    int head;  //Откуда брать элемент для удаления
    int tail;  //Куда класть элемент при добавлении
} cb_queue;

void init_queue(cb_queue *q, int nel)
{
    q->data = alloc(nel, q_type);
    q->cap = nel;
    q->count = q->head = q->tail = 0;
}

void delete_queue(cb_queue *q)
{
    free(q->data);
    q->cap = q->count = q->head = q->tail = 0;
}

boolean queue_empty(cb_queue *q)
{
    return !q->count;
}

void enqueue(cb_queue *q, q_type x)
{
    if(q->count == q->cap){
        int old_cap = q->cap;
        q->cap *= 2;
        q->data = (q_type *)realloc(q->data, q->cap*sizeof(q_type));
        if(q->head){ //Голова сдвинута относительно начала массива - надо сдвигать эту часть
            memcpy(q->data+q->head+old_cap, q->data+q->head, (old_cap-q->head)*sizeof(q_type));
            q->head += old_cap;
        } else { //Данные расположены в начале массива - достаточно переместить хвост в конец
            q->tail += old_cap;
        }

    }
    q->data[q->tail] = x;
    ++q->tail;
    if(q->tail == q->cap){
        q->tail = 0;
    }
    ++q->count;
}

//void enqueue(cb_queue *q, q_type x)
//{
//    if(q->count == q->cap){
//        q->cap *= 2;
//        q->data = realloc(q->data, q->cap*sizeof(q_type));
//        if(q->head){
//            int right_part_len = q->count-q->head;
//            memcpy(q->data+q->head, q->data+q->cap-right_part_len, right_part_len * sizeof(q_type));
//            q->head = q->cap-right_part_len;
//        }

//    }
//    q->data[q->tail] = x;
//    ++q->tail;
//    if(q->tail == q->cap){
//        q->tail = 0;
//    }
//    ++q->count;
//}

q_type dequeue(cb_queue *q)
{
    if(queue_empty(q)){
        delete_queue(q);
        printf("ERROR: Опустошение\n");
        exit(1);
    }
    q_type result = q->data[q->head];
    ++q->head;
    if(q->head == q->cap){
        q->head = 0;
    }
    --q->count;
    return result;
}

//-----------------------------------------
#define NAME_LEN 100 //Длина имени команды
#define COMMAND(str) if(!strcmp(command, #str))
void menu(cb_queue *q)
{
    int  i, m;
    char command[NAME_LEN];
    scanf("%d", &m);
    int arg;
    for(i = 0; i < m; ++i){
        scanf("%s", command);
        COMMAND(ENQ){
            scanf("%d", &arg);
            enqueue(q, arg);
            printf("ENQ: HEAD = %d, q->data[q->head] = %d, TAIL = %d, CAP = %d, COUNT = %d\n", q->head, q->data[q->head], q->tail, q->cap, q->count);
        }
        COMMAND(DEQ){
            printf("HEAD = %d, TAIL = %d, CAP = %d\n", q->head, q->tail, q->cap);
            printf("%d\n", dequeue(q));
        }
        COMMAND(EMPTY){
            printf(queue_empty(q) ? "true\n" : "false\n");
        }
    }
}

int main(void)
{
    cb_queue Q;
    int start_size = 4;
    init_queue(&Q, start_size);
    menu(&Q);
    delete_queue(&Q);
    return 0;
}

/*Кольцевой буфер
Условие
Баллы: 2

Реализуйте операции InitQueue, QueueEmpty, Enqueue и Dequeue для очереди целых чисел, представленной в виде кольцевого буфера. Начальный размер буфера – 4. В случае переполнения размер буфера должен увеличиваться в два раза.

Составьте программу circbuf.c, демонстрирующую работоспособность реализованных операций.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит общее количество выполняемых операций n (0 < n ≤ 100000). Каждая из следующих n строк содержит описание операции.

Операция либо имеет форму ENQ x (добавить число x в хвост очереди, -2000000000 < x < 2000000000), либо форму DEQ (удалить головной элемент из очереди), либо форму EMPTY (проверить пустоту очереди).

Можно считать, что последовательность операций составлена правильно, то есть перед каждой операцией DEQ очередь не пуста.

Формат результата работы программы

Для каждой операции DEQ вывести в стандартный поток вывода значение удаляемого головного элемента очереди. Для каждой операции EMPTY вывести в стандартный поток вывода «true» или «false» в зависимости от того, пуста очередь или нет.

Пример работы программы


Входные данные


Выходные данные


9
EMPTY
ENQ 1
ENQ 2
DEQ
EMPTY
ENQ 3
DEQ
DEQ
EMPTY


true
1
false
2
3
true


5
EMPTY
ENQ -20893039
ENQ 1345905379
DEQ
DEQ

14
ENQ 1
ENQ 2
ENQ 3
ENQ 4
DEQ
DEQ
DEQ
ENQ 5
ENQ 6
ENQ 7
ENQ 8
DEQ
DEQ
DEQ


ENQ count = 511, tail = 220, cap = 512,
HEAD = 221, q->data[q->head] = -727076666, TAIL = 220, CAP = 512
ENQ count = 512, tail = 221, cap = 1024,
HEAD = 221, q->data[q->head] = -727076666, TAIL = 221, CAP = 1024
ENQ count = 513, tail = 222, cap = 1024,
HEAD = 221, q->data[q->head] = -1641599467, TAIL = 222, CAP = 1024
ENQ count = 514, tail = 223, cap = 1024,

1908

http://pastebin.com/RVsae1GC

valgrind ~/Qt/3-module/3-circ-buf-Debug/3-circ-buf

< /home/nastasia/Qt/3-module/3-circ-buf/test10.txt > /home/nastasia/Qt/3-module/3-circ-buf/out.txt
*/
