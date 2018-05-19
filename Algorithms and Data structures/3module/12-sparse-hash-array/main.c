#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 100

typedef int (*hash_type)(int, int);

typedef struct Elem {
    struct Elem *next;
    int value; //Значение
    int index;  //Индекс в разреженном массиве
} *elem;

typedef struct List
{
    elem Head;
} *list;

typedef struct Hash_table
{
    list *table;
    int nel;
    hash_type h;
} *hash_table;

list init_new_list()
{
    list New_list = (list)malloc(sizeof(struct List));
    New_list->Head = NULL;
    return New_list;
}

elem init_new_elem()
{
    elem New_elem = (elem)malloc(sizeof(struct Elem));
    New_elem->next = NULL;
    return New_elem;
}


#define hash_table_size sizeof(sizeof(struct Hash_table) + n*sizeof(list))

hash_table init_hash_table(int n, hash_type hash_func)
{
    hash_table New_table = (hash_table)malloc(hash_table_size);
    New_table->table = (list  *)(New_table+1);
    New_table->nel = n;
    New_table->h = hash_func;
    int i;
    for(i = 0; i < n; ++i){
        New_table->table[i]->Head = NULL /*= init_new_list()*/;
    }
    return New_table;
}

//------------------Добавление элемента-------------------

elem list_search(elem Head, int index)
{
    elem Current = Head;
    while(Current && Current->index != index){
        Current = Current->next;
    }
    return Current;
}

//Поиск последнего элемента, чей индекс меньше текущего
int list_search_last_least(list L, int index, elem *Prev)
{
    elem Current = L->Head;
    *Prev = Current;
    while(Current && Current->index < index){
        *Prev = Current;
        Current = Current->next;
    }
    return Current->index == index;
}

void insert_before_head(list L, elem New_elem)
{
    New_elem->next = L->Head;
    L->Head = New_elem->next;
}

void insert_after(elem *T, elem New_elem)
{
    if(!*T){
        *T = New_elem;
        return;
    }
    New_elem->next = T[0]->next;
    T[0]->next = New_elem;
}

//------------------Удаление-------------------

void delete_from_head(elem T)
{
    if(!T) return;
    elem Current = T;
    elem Next = Current->next;
    while(Next != NULL){
//        printf("free %d %d\n", Current->index, Current->value);
        free(Current);
        Current = Next;
        Next = Current->next;
    }
//    printf("free %d %d\n", Current->index, Current->value);
    free(Current);
}


void delete_table(elem *table, int len)
{
    int i;
    for(i = 0; i < len; ++i){
        delete_from_head(table[i]);
    }
    free(table);
}

//----------------Вывод на консоль списка--------------------

void print_list(elem Head)
{
    elem Temp = Head;
    if(!Head) printf("Nothing to do\n");
    while(Temp != NULL){
        printf("%d %d\n", Temp->index, Temp->value);
        Temp = Temp->next;
    }
}

elem scan_elem()
{
    elem New = init_new_elem();
    scanf("%d %d", &New->index, &New->value);
    return New;
}

//-----------------------------------------------------------
int hash(int i, int m)
{
    return i % m;
}

void reassign(elem Old, elem New)
{
    Old->value = New->value;
    free(New);
}

//ASSIGN i v (присвоить значение v элементу разреженного массива с индексом i)

void insert_hash_table(elem *T, elem New, int nel)
{
    int i = hash(New->index, nel);
    elem Temp = list_search(T[i], New->index);
    if(Temp){ //Элемент с таким индексом уже имеется
        reassign(Temp, New);
        return;
    }
    insert_after(&T[i], New);
}

int lookup(elem *T, int index, int nel)
{
    int i = hash(index, nel);
    elem temp = list_search(T[i], index);
    if(!temp) return 0;
    return temp->value;
}

#define NAME_LEN 100 //Длина имени команды
#define COMMAND(str) if(!strcmp(command, #str))
void menu(elem *table, int n, int length)
{
    int  i;
    int index;
    char command[NAME_LEN];
    for(i = 0; i < n; ++i){
        scanf("%s", command);
        COMMAND(ASSIGN){
            elem New = scan_elem();
            insert_hash_table(table, New, length);
//            printf("ASSIGNED: %d %d\n", New->index, New->value);
        }
        COMMAND(AT){
            scanf("%d", &index);
            printf("%d\n", lookup(table, index, length));
        }

    }
}

int main(void)
{
    int command_num, table_len;
    scanf("%d %d", &command_num, &table_len);
    elem *table = (elem *)calloc(table_len, sizeof(elem));  //В таблицу будет вставлено элементов не больше, чем команд, указатели изначально нулевые
    menu(table, command_num, table_len);
    delete_table(table, table_len);
    return 0;
}

/*Разреженный массив
Условие
Баллы: 3

Разреженный массив – это массив большого размера, большинство элементов которого равны нулю. Хранение разреженного массива в памяти целиком нецелесообразно или даже вовсе невозможно из-за его большого размера, поэтому разумным решением является хранение только ненулевых элементов за счёт некоторого снижения скорости операций над массивом.

Разреженный целочисленный массив можно представить как ассоциативный массив, в котором и ключи, и значения являются целыми числами. При этом наличие в ассоциативном массиве словарной пары ⟨k,v⟩ означает, что k-тый элемент разреженного массива равен v. Если же в ассоциативном массиве нет пары с ключом k, то считается, что k-тый элемент разреженного массива равен нулю.

Будем считать, что в ассоциативном массиве, представляющем разреженный массив, вообще нет словарных пар со значением ноль. Это означает, что если k-тому элементу разреженного массива, содержащему ненулевое значение, присваивается ноль, то словарная пара с ключом k вообще удаляется из ассоциативного массива.

Пусть A – множество разреженных целочисленных массивов. Определим основные операции над разреженным целочисленным массивом:

    At : A × ℕ → ℤ – возвращает k-тый элемент массива,
    Assign : A × ℕ × ℤ → A – присваивает новое значение k-тому элементу массива.

Пусть разреженный целочисленный массив представлен в виде хеш-таблицы размера m. Реализуйте для него операции At и Assign. Составьте программу disparray.c, демонстрирующую работоспособность реализованных операций.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит общее количество выполняемых операций n (0 < n ≤ 100000), а вторая – размер хеш-таблицы m. Каждая из следующих n строк содержит описание операции.

Операция либо имеет форму ASSIGN i v (присвоить значение v элементу разреженного массива с индексом i; 0 ≤ i < 1000000000; -1000000000 < v < 1000000000), либо форму AT i (вывести значение элемента разреженного массива с индексом i).

Формат результата работы программы

Для каждой операции AT вывести в стандартный поток вывода значение элемента разреженного массива с индексом i.

Пример работы программы


Входные данные


Выходные данные


6
1023
AT 1000000
ASSIGN 1000000 5
ASSIGN 2000000 10
AT 2000000
AT 1500000
AT 1000000


0
10
0
5

6
2
ASSIGN 5 0
ASSIGN 6 -7
AT 6
ASSIGN 6 4
ASSIGN 4 8
AT 6

http://pastebin.com/VV591MJR

valgrind ~/Qt/3-module/12-sparse-hash-array-Debug/12-sparse-hash-array

Указание.
    Пусть хеш-функция вычисляется как h(i) = i (mod m ).

*/
