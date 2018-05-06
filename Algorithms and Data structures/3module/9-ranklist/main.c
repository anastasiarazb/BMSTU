#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 10 //Длина строки фиксированно меньше 10
#define _size_(of_struct) sizeof(struct of_struct)

#define even(n) !(n & 1)
typedef char boolean;
typedef boolean (*cmp_type)(int, int);

typedef struct Elem
{
    int key;
    char value[MAX_LEN];
    struct Elem **next;  //elem *next
    int *span;
} *elem;

typedef struct List
{
    elem Head;
    int nel;
    int levels; //log2(nel)
    void *buffer; //Буфер для операций Skip и Insert
    cmp_type compare;
} *list;

int fast_log2(int n);
elem init_elem(list L);

list init_list(int nel, cmp_type compare)
{
    list New = (list)malloc(sizeof(struct List));
    New->nel = nel;
    New->levels = fast_log2(nel)+1/*16*/;
    New->buffer = malloc(New->levels * sizeof(elem) + New->levels * sizeof(int));
    New->compare = compare;
    New->Head = init_elem(New);
    return New;
}

#define sum_len sizeof(struct Elem) + L->levels * sizeof(elem) + L->levels * sizeof(int)
elem init_elem(list L)
{
    elem New_list = (elem)calloc(1, sum_len);
    New_list->next = (elem *)(New_list+1);
    New_list->span = (int *)(New_list->next+L->levels);
    return New_list;
}

elem succ(elem x){
    return x->next[0];
}

boolean list_empty(elem Head)
{
    return !Head->next[0];
}

boolean compare_min(int a, int b)
{
    return a < b;
}

//Формирует массив указателей p размера levels такой, что p[i] - указатель на элемент в списке с номером i, после которого может располагаться элемент с ключом k;
// в p[i] элемент меньший, чем k, либо это указатель на головной элемент
// следующий за p[i] элемент больше k, либо он хвостовой
elem *skip(list L, int key) {
    elem *p = (elem *)L->buffer;
    elem x = L->Head;
    int i;
    for(i = L->levels - 1; i >= 0; --i) {
        while(x->next[i] != NULL && L->compare(x->next[i]->key, key)){
            x = x->next[i];
        }
        p[i] = x;
    }
    return p;
}

#define alloc_elem_ptr (elem *)malloc(levels*sizeof(elem))
elem lookup(list L, int key)
{
    elem *p = skip(L, key);
    elem Temp = succ(p[0]);
    if (!Temp || Temp->key != key)
    {
        printf("LOOKUP: Error. The key is not found.\n");
        return NULL;
    }
    return Temp;
}
//-------------------------------------------------------------------------
      elem *rank_skip(list L, elem New);

elem insert(list L, elem x) {
    int key = x->key;
    elem *p = skip(L, x->key);
    if (p[0]->next[0] && p[0]->next[0]->key == key){
        printf("INSERT: Panic, the elem is already exist.\n");
        return NULL;
    }
    printf("INSERT: %d, %s\n", x->key, x->value);
    int r = 2 * rand();
    int i;
    for(i = 0; i < L->levels && even(r); ++i){
        x->next[i] = p[i]->next[i];
        p[i]->next[i] = x;
        r /= 2;
    }
    while(i < L->levels){
        x->next[i] = NULL;
        ++i;
    }
    return x;
}

//--------------------------------------------------------

      void show_list(list L);

elem rank_insert(list L, elem New) {
//    printf("\nINSERT: %d, %s\n", New->key, New->value);
    int key = New->key;
    int i, m = L->levels;
    elem *p = (elem *)L->buffer;
    int *distances = (int *)(p+m);   //Перевести указатель за массив p
    elem x = L->Head;
    int counter;
    i = m-1;
    //Найти "стартовый" элемент, предшествующий вставляемому на самом высоком уровне
    while(x->next[i] != NULL && L->compare(x->next[i]->key, key)){
        x = x->next[i];
//        printf("\n%d %d", x->key, x->span[i]);
    }
    p[i] = x;
    for(i = m-2; i >= 0; --i) {
        counter = 0;
        while(x->next[i] != NULL && L->compare(x->next[i]->key, key)){
            counter += x->span[i];    //Вычисление расстояния от "стартового" элемента на данном уровне
            x = x->next[i];
//            printf("\n%d %d", x->key, x->span[i]);
        }
        distances[i+1] = counter;  //Для каждого уровня подсчитывается, сколько пройдено на предыдущем
        p[i] = x;
    }
    distances[0] = 0;    //На нулевом уровне вставляемый элемент следует непосредственно за предыдущим
//    printf("\nDISTANCES 1:\n");
//    for(i = m-1; i >= 0; --i){
//        printf("%d\n", distances[i]);
//    }
    for(i = 1; i < m; ++i) {
        distances[i] += distances[i-1];  // Расстановка суммарных дистанций для каждого уровня
    }
//    printf("\nDISTANCES 2:\n");
//    for(i = m-1; i >= 0; --i){
//        printf("%d\n", distances[i]);
//    }
    if (p[0]->next[0] && p[0]->next[0]->key == key){
        printf("INSERT: Panic, the key %d is already exist.\n", key);
        free(New);
        return p[0]->next[0];
    }
    x = New;
    int r = 2 * rand();
    int old_span;
    for(i = 0; i < L->levels && even(r); ++i){
        x->next[i] = p[i]->next[i];
        p[i]->next[i] = x;
        old_span = p[i]->span[i];
        p[i]->span[i] = distances[i] + 1;  //Добавляем единицу к расстоянию, так как добавляется элемент
        x->span[i] = old_span - distances[i];
//        printf("x->span[%d] = %d, p->span %d\n", i, x->span[i], p[i]->span[i]);
        r /= 2;
    }

    //Если элемента нет на уровне, то next[i] и span[i] = 0 (по умолчанию при выделении памяти)
    //а расстояние от прошлого элемента до возможного следующего увеличивается на единицу
    while(i < m){
        //        x->next[i] = NULL;
        //        x->span[i] = 0;
        ++p[i]->span[i];
        ++i;
    }
//    for(i = 0; i < m; ++i){
//        printf("P SPAN[%d] %d el %d\n", i, p[i]->span[i], p[i]->key);
//    }
//    show_list(L);
    return x;
}
//----------------------------------------------------------------------------
int rank(list L, int key) {
//    printf("\nRANK: %d\n", key);
    int i, m = L->levels;
    elem *p = (elem *)L->buffer;
//    int *distances = (int *)(p+m);   //Перевести указатель за массив p
    elem x = L->Head;
    int counter = 0;
    for(i = m-1; i >= 0; --i) {
        while(x->next[i] != NULL && L->compare(x->next[i]->key, key)){
            counter += x->span[i];
            x = x->next[i];
        }
        p[i] = x;
    }
//    printf("\nDISTANCES 1:\n");
//    for(i = m-1; i >= 0; --i){
//        printf("%d\n", distances[i]);
//    }
//    for(i = m-1, counter = 0; i >= 0; --i) {
//        counter += distances[i];  // Подсчет суммы
//    }
    if (!p[0]->next[0] || p[0]->next[0]->key != key){
        printf("RANK: Panic, the key %d is not found.\n", key);
        return -1;
    }
//    printf("RANK %d = %d\n", key, counter);
//    show_list(L);
    return counter;
}
//-------------------------------------------------------------------------
void rank_delete_elem(list L, int key)
{
//    printf("DELETE ELEM: %d\n", key);
    elem *p = skip(L, key);
    elem x = succ(p[0]);
    if (!x || x->key != key)
    {
        printf("DELETE ELEM: Error. The key is not found.\n");
        return;
    }
    int m = L->levels;
    int i;
    for(i = 0; i < m && p[i]->next[i] == x; ++i){
        p[i]->next[i] = x->next[i];
        p[i]->span[i] += x->span[i] - 1;
    }
    while(i < m){
        --p[i]->span[i];
        ++i;
    }
//    show_list(L);
    free(x);
}

void delete_elem(list L, int key)
{
    elem *p = skip(L, key);
    elem x = succ(p[0]);
    if (!x || x->key != key)
    {
        printf("DELETE ELEM: Error. The key is not found.\n");
        return;
    }
    int m = L->levels;
    int i;
    for(i = 0; i < m && p[i]->next[i] == x; ++i){
        p[i]->next[i] = x->next[i];
    }
    free(x);
}

void show_list(list L)
{
    elem x = L->Head;
    elem current = x;
    int i = L->levels-1;
    int len = 1;
    printf("List:\n");
    for(; i >= 0; --i){
        x = current = L->Head;
        while(x){
            while(/*current && */current!= x){
//                printf("|%*s", 6+2+len+5, "");
                printf("|%*s", 6+5, "");
                current = current->next[0];
            }
//            printf("| {%2d, %*s, <%d>} ", x->key, len, x->value, x->span[i]);
            printf("| {%2d, <%d>} ", x->key, x->span[i]);
            x = x->next[i];
            current = current->next[0];
        }
        putchar('\n');
    }

}

void delete_list(list L)
{
    elem temp = L->Head;
    elem prev = NULL;
    while(temp != NULL){
        prev = temp;
        temp = temp->next[0];
//        printf("Free: %d\n", prev->key);
        free(prev);
    }
    free(L->buffer);
    free(L);
}

//-------------------------------------------------------------------
#define NAME_LEN 100 //Длина имени команды
#define COMMAND(str) if(!strcmp(command, #str))
void menu(list L, int n)
{
    int  i;
    char command[NAME_LEN];
    elem Temp = NULL;
    int arg;
    for(i = 0; i < n; ++i, Temp = NULL){
//        if(!(i % 1000)) show_tree(T);
        scanf("%s", command);
        COMMAND(INSERT){  //INSERT k v (добавить словарную пару, в которой ключ k – целое число, значение v – строка, составленная из латинских букв)
            Temp = init_elem(L);
            scanf("%d %s", &Temp->key, Temp->value);
            rank_insert(L, Temp);
            continue;
        }
        COMMAND(LOOKUP){ //LOOKUP k (вывести строку, связанную с ключом k)
            scanf("%d", &arg);
            Temp = lookup(L, arg);
            printf("%s\n", Temp->value);
            continue;
        }
        COMMAND(DELETE){  // DELETE k (удалить строку, связанную с ключом k)
            scanf("%d", &arg);
            rank_delete_elem(L, arg);
            continue;
        }
        COMMAND(RANK){
            scanf("%d", &arg);
            printf("%d\n", rank(L, arg));
        }
    }
}

int main(void)
{
    int n;
    scanf("%d", &n);
    list L = init_list(n, compare_min);
    menu(L, n);
//    show_list(L);
    delete_list(L);
    return 0;
}


//Быстрое вычисление целой части логарифма по основанию 2 из целого числа
// (не более 16 итераций)
int fast_log2(int n)
{
    if(n < 1) {
        printf("MY_LOG2: incorrect number (overflow/zero-value): %u.\n", n);
        return -1;
    }
    if(n == 1) return 0;
    int delta, i, test;
    i = delta = 16;
    while(1){
        delta /= 2;
        test = n >> i;
        if(test == 1) return i;
        if(test) {
            i += delta;
        } else {
            i -= delta;
        }
    }
    return -1; //При правильной работе сюда не будет передано управление
}

/*Ранги элементов в списке с пропусками
Условие
Баллы: 3

Операция Rank : A×K → ℕ для ассоциативного массива вычисляет порядковый номер словарной пары с ключом k в отсортированной последовательности входящих в ассоциативный массив словарных пар. Пары нумеруются, начиная с нуля.

Модифицируйте представление и реализацию списка с пропусками, чтобы операция Rank для него работала в среднем за время O(lg n).

Составьте программу ranklist.c, демонстрирующую работоспособность реализованной операции.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит общее количество выполняемых операций n (0 < n ≤ 100000). Каждая из следующих n строк содержит описание операции.

Операция либо имеет форму INSERT k v (добавить в список с пропусками словарную пару, в которой ключ k – целое число, значение v – строка, составленная из латинских букв; -1000000000 < k < 1000000000, len(v) < 10), либо форму LOOKUP k (вывести строку, связанную с ключом k), либо форму DELETE k (удалить строку, связанную с ключом k), либо форму RANK k (вывести порядковый номер словарной пары с ключом k).

Можно считать, что последовательность операций составлена правильно.

Формат результата работы программы

Для каждой операции LOOKUP вывести в стандартный поток вывода строку, связанную с ключом k. Для каждой операции RANK вывести в стандартный поток вывода порядковый номер словарной пары с ключом k.

Пример работы программы


Входные данные


Выходные данные


6
INSERT 10 qwerty
INSERT 15 asdf
INSERT 5 zxc
LOOKUP 10
DELETE 10
RANK 15


qwerty
1

19
INSERT 10 q
INSERT 15 a
INSERT 5 g
INSERT 14 f
INSERT 35 h
RANK 38
INSERT 38 s
DELETE 5
INSERT 40 o
INSERT 3 g
RANK 35
DELETE 15
INSERT 3 f
DELETE 15
INSERT 56 h
INSERT 35 d
DELETE 3
RANK 56
RANK 38


qwerty

http://pastebin.com/1JrX0uWm
http://pastebin.com/PF6v9iuY

valgrind ~/Qt/3-module/9-ranklist-Debug/9-ranklist

Указание.
    Представление списка с пропусками нужно модифицировать следующим образом: каждый элемент списка должен включать в себя массив целых чисел span размера m, где m – количество уровней в списке. При этом i-тый элемент массива span должен содержать расстояние от данного элемента до следующего элемента на i-том уровне.

*/
