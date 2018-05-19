#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

#ifdef DEBUG
#define out printf
#else
#define out(...)
#endif

#define max(a, b) ((a) > (b) ? a : b)
#define MINVALUE -2000000000
//------------Реализация очереди через двойной стек-------------
#define alloc(n, type) (type *)malloc(n*sizeof(type))
typedef int q_type;

typedef char boolean;

typedef struct DS_Queue
{
    q_type *data;
    int cap;   //Размер массива
    int top1;  //Куда класть элементы при добавлении (сколько элементов в 1-м стеке)
    int top2;  //За этим индексом начинается второй стек
    int max_left; //Максимальный элемент в первом, необработанном стеке
} ds_queue;

void init_queue(ds_queue *q, int nel)
{
    q->data = alloc(nel, q_type);
    q->cap = nel;
    q->top1 = 0;
    q->top2 = nel-1;
}

void delete_queue(ds_queue *q)
{
    free(q->data);
    q->cap = 0;
    q->top1 = q->top2 = 0;
}

boolean queue_empty(ds_queue *q)
{
    return (q->top1 == 0) && (q->top2 == q->cap - 1); //StackEmpty1 && StackEmpty2
}

boolean stack_empty1(ds_queue *q)
{
    return q->top1 == 0;
}

boolean stack_empty2(ds_queue *q)
{
    return q->top2 == q->cap - 1;
}

//Увеличить размер очереди
void realloc_ds_queue(ds_queue *q)
{
        int old_cap = q->cap;
        q->cap *= 2;
        q->data = (q_type *)realloc(q->data, q->cap * sizeof(q_type));
        if(q->top2 == old_cap-1){ //Второй стек пуст, не надо ничего двигать
            q->top2 = q->cap - 1;
            out("new-top2 = %d, new-cap = %d\n", q->top2 , q->cap);
        } else {
            out("from:  %d, to : %d, number: %d\n", q->top2 + 1 , q->top2 + 1 + old_cap, (old_cap - q->top2 - 1));
            memcpy(q->data + q->top2 + 1 + old_cap, q->data + q->top2 + 1, (old_cap - q->top2 - 1) * sizeof(q_type));
            q->top2 = q->cap - (old_cap - q->top2);
            out("new top2 = %d\n", q->top2);

        }
    }

void enqueue(ds_queue *q, q_type x) //Push1(s, x)
{
    if(q->top2 < q->top1){
        realloc_ds_queue(q);
    }
    q->data[q->top1] = x;
    ++q->top1;
}

q_type dequeue(ds_queue *q)
{
    if(stack_empty2(q)){
        if(q->top1 < q->cap){
        //Тут старый первый стек и новый второй могут перекрываться, поэтому memcpy выдает ошибку
        memmove(q->data+q->cap-q->top1, q->data, q->top1 * sizeof(q_type));
        q->top2 = q->cap-q->top1;
        q->top1 = 0;
        return q->data[q->top2];
        } else {    //Первый стек занимает весь буфер, не надо ничего двигать
            q->top2 = q->top1 = 0;
            return q->data[q->top2];
        }
    }
    //x = pop2(s)
    ++q->top2;
    return q->data[q->top2];
}
//----------------Специфические операции для быстрой работы функции maximum-------------

//Переместить все элементы из первого стека во второй,
//вычисляя сразу максимум между текущим элементом и старым стеком
//и записывая его сразу за текущим элементом
//(стек состоит из пар <current_max, x>, эмуляция массива структур).
void while_not_empty1_push2(ds_queue *q)
{
    while(q->top1){
        //Забрать элемент из первого стека и положить во второй
        --q->top1;
        q->data[q->top2] = q->data[q->top1];
        --q->top2;
        //Положить во второй стек максимум между последним значением и прошлым максимумом
        q->data[q->top2] = q->top2 + 2 < q->cap ?
                    max(q->data[q->top2 + 1], q->data[q->top2 + 2]) :
                    q->data[q->top2 + 1];
        --q->top2;
    }
}

q_type max_dequeue(ds_queue *q)
{
    if(stack_empty2(q)){
        if(q->cap < 2*q->top1){ //Если размера очереди не хватит на длину удвоенного первого стека
            out("REALLOC: cap = %d, top = %d\n", q->cap, q->top1);
            realloc_ds_queue(q);
        }
        while_not_empty1_push2(q);
    }
    //x = pop2(s)
    q->top2 += 2; //Подняться на 2 позиции, так как во втором стеке лежит пара <current_max, el>
    return q->data[q->top2];
}

void max_enqueue(ds_queue *q, q_type x)
{
    if(q->top2 < q->top1){
        out("REALLOC: cap = %d, top = %d\n", q->cap, q->top1);
        realloc_ds_queue(q);
    }
    q->data[q->top1] = x;
    ++q->top1;
    if(q->top1 == 1){ //В первом стеке находится единственный элемент
        q->max_left = x;
    } else {
        q->max_left = max(q->max_left, x);
    }
}

q_type return_max(ds_queue *q)
{
    if(!stack_empty1(q)){
        if(!stack_empty2(q)){
            //Вернуть максимум между максимумами первого и второго стеков
            return max(q->max_left, q->data[q->top2+1]);
        } else {
            return q->max_left;
        }
    } else {
        if(!stack_empty2(q)){
            //Первый стек пуст, вернуть максимум из второго стека
            return q->data[q->top2+1];
        } else {
            printf("Error: no max, queue is empty.\n");
            return MINVALUE;
        }
    }
}

//-----------------------------------------
#define NAME_LEN 100 //Длина имени команды
#define COMMAND(str) if(!strcmp(command, #str))


void menu(ds_queue *q)
{
    int  i, m;
    char command[NAME_LEN];
    scanf("%d", &m);
    int arg;
    for(i = 0; i < m; ++i){
        scanf("%s", command);
        COMMAND(ENQ){
            scanf("%d", &arg);
            max_enqueue(q, arg);
        }
        COMMAND(DEQ){
            printf("%d\n", max_dequeue(q));
        }
        COMMAND(EMPTY){
            printf(queue_empty(q) ? "true\n" : "false\n");
        }
        COMMAND(MAX){
            printf("%d\n", return_max(q));
        }
    }
}

int main(void)
{
    ds_queue Q;
    int start_size = 4;
    init_queue(&Q, start_size);
    menu(&Q);
    delete_queue(&Q);
    return 0;
}


/*Очередь с операцией Maximum
Условие
Баллы: 2

Реализуйте через двойной стек набор операций InitQueue, Enqueue, Dequeue, QueueEmpty и Maximum для работы с очередью целых чисел. Операция Maximum возвращает максимальное целое число, в данный момент времени находящееся в очереди. Операции Enqueue, QueueEmpty и Maximum должны работать за константное время, а операция Dequeue – за амортизированное константное время.

Составьте программу qmax.c, демонстрирующую работоспособность реализованных операций.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит общее количество выполняемых операций n (0 < n ≤ 100000). Каждая из следующих n строк содержит описание операции.

Операция либо имеет форму ENQ x (добавить число x в хвост очереди, -2000000000 < x < 2000000000), либо форму DEQ (удалить головной элемент из очереди), либо форму MAX (показать текущее максимальное число), либо форму EMPTY (проверить пустоту очереди).

Можно считать, что последовательность операций составлена правильно, то есть перед каждой операцией DEQ очередь не пуста.

Формат результата работы программы

Для каждой операции DEQ вывести в стандартный поток вывода значение удаляемого головного элемента очереди. Для каждой операции MAX вывести в стандартный поток вывода текущее максимальное число. Для каждой операции EMPTY вывести в стандартный поток вывода «true» или «false» в зависимости от того, пуста очередь или нет.

Пример работы программы


Входные данные


Выходные данные


7
ENQ 4
ENQ 2
ENQ 3
MAX
DEQ
MAX
EMPTY


4
4
3
false


6
EMPTY
ENQ 1
ENQ -2
ENQ 8
MAX
DEQ

true
8
1




14
EMPTY
ENQ 5
MAX
DEQ
EMPTY
ENQ -5
ENQ -5
MAX
DEQ
DEQ
EMPTY
ENQ -5
ENQ -7
ENQ 8

true
5
5
true
-5
-5
-5
true


http://pastebin.com/UhidDfTy

valgrind ~/Qt/3-module/4-queue-max-doublestack-Debug/4-queue-max-doublestack

16
ENQ 1
ENQ 2
ENQ 3
ENQ 4
DEQ
DEQ
DEQ
EMPTY
ENQ 5
ENQ 6
ENQ 7
ENQ 8
EMPTY
DEQ
DEQ
DEQ

*/
