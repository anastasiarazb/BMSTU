#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABETH_SIZE 26
#define STR_LEN 100000
#define MEM_LIST_SIZE 65536
//#define alloc (trie)calloc(1, sizeof(struct Trie));
//#define my_free free

#define alloc silly_alloc(&Container)
#define my_free(x) silly_free(x, &Container)
typedef char boolean;
typedef struct Trie
{
    boolean word_end; //Метка конца слова, равна нулю, если символ - служебный
    int val;          // Количество строк множества, имеющих указанный префикс == чиcло вершин в поддереве
    struct Trie *parent;
    struct Trie *arcs[ALPHABETH_SIZE];
} *trie;
//-------------Объявление менеджера памяти--------------------------------------
typedef struct Trie mem_type;
typedef mem_type *q_type;

typedef struct CB_Queue
{
    q_type *data;
    int cap;   //Размер массива
    int count; //Количество элементов в очереди
    int head;  //Откуда брать элемент для удаления
    int tail;  //Куда класть элемент при добавлении
} *cb_queue;

typedef struct MemPage
{
    int top;
    mem_type array[MEM_LIST_SIZE];  //Массив целевого типа данных
} *mem_page;

struct MemContainer
{
    int top;  //Количество в элементов в массиве равно top+1
    mem_page *pages;
    cb_queue Freed;
} Container;

mem_type *silly_alloc(struct MemContainer *C);
void silly_free(mem_type *elem, struct MemContainer *Container); //Пометить данный элемент как свободный
void init_memory(struct MemContainer *Container);
void destroy_container(struct MemContainer *Container);   //Освободить всю выделенную память


//---------------------------Работа с бором--------------------------------------


   void show_trie(trie T, char *k);

trie init_trie()
{
    trie New = alloc; //Все заполнено нулями при аллокации
    memset(New, 0, sizeof(struct Trie));
    return New;
}

boolean map_empty(trie T)
{
    if(T->word_end) return 0;
    int i;
    for(i = 0; i < ALPHABETH_SIZE; ++i){
        if (T->arcs[i] != NULL) return 0;
    }
    return 1;
}

#define KEY(c) (c - 'a')

void descend(trie T, char *k, int *i, trie *x)
{
    *x = T;
    trie y = NULL;
    int len = strnlen(k, STR_LEN);
    for(*i = 0; *i < len; *i += 1){
        y = (*x)->arcs[KEY(k[*i])];
        if(y == NULL) break;
        *x = y;
    }
}

int lookup(trie T, char *k)
{
    trie x = NULL;
    int i;
    descend(T, k, &i, &x);
    if(i != (int)strnlen(k, STR_LEN) || !x->word_end){
        printf("LOOKUP: error, function descend works incorrect.\n");
        return -1;
    }
    return x->val;
}

void increase_val(trie x)
{
    trie Temp = x;
    while(Temp)
    {
        ++Temp->val;
        Temp = Temp->parent;
    }
}

void decrease_val(trie x)
{
    trie Temp = x;
    while(Temp)
    {
        --Temp->val;
        Temp = Temp->parent;
    }
}

trie insert_trie(trie T, char *k)
{
    trie x = NULL;
    int i;
    descend(T, k, &i, &x);
    int len = strnlen(k, STR_LEN);
//    printf("%s is_leaf = %d", k, !x->subordinate);
    if(i == len && x->word_end){
//        printf("INSERT: error, function descend works incorrect. Maybe the word \"%s\" is already inserted.\n", k);
        return x; //Такая строка уже есть, ничего не делать
    }
    trie y = NULL;
    for(; i < len; ++i){
        y = init_trie();         //Новая промежуточная вершина, все поля равны 0, в т.ч. метка конца слова
        x->arcs[KEY(k[i])] = y;  //Создается новая вершина и приставляется в нужное место к x
        y->parent = x;
        x = y;
    }
    x->word_end = 1;  //Конец слова
    increase_val(x);
//    printf("INSERT:\n");
//    show_trie(T, k);
    return x;
}

void delete_trie_node(trie T, char *k)
{
    trie x = NULL;
    int i, j;
    descend(T, k, &i, &x);
    int len = strnlen(k, STR_LEN);
    if(i != len || !x->word_end){
        printf("DELETE: error, function descend works incorrect. Maybe, the word \"%s\" does not exist.\n", k);
        return;
    }
    x->word_end = 0;
    while(x->parent && !x->word_end){
        for(j = 0; j < ALPHABETH_SIZE && (x->arcs[j] == NULL); ++j);
        if(j < ALPHABETH_SIZE) break;
        if(x->val > 1) break;
        trie y = x->parent;
        --i;
        y->arcs[KEY(k[i])] = NULL;
        my_free(x);
        x = y;
    }
    decrease_val(x);
//    printf("DELETE:\n");
//    show_trie(T, k);
}

void show_trie(trie T, char *k)
{
    trie x = T;
    trie y = NULL;
    int i;
    putchar('\n');
    int len = strnlen(k, STR_LEN);
    for(i = 0; i < len; i += 1){
        y = (x)->arcs[KEY(k[i])];
        printf("%c %d\n", k[i], x->val);
        if(y == NULL) break;
        x = y;
    }
    putchar('\n');
}

int prefix(trie T, char *k)
{
    trie x = T, y = NULL;
    int i = 0, len = strnlen(k, STR_LEN);
    while (i < len){
        y = x->arcs[KEY(k[i])];
        if(!y) return 0;       //Строки с таким префиксом не найдено
        x = y;
        ++i;
    }
    return x->val;
}

//---------------------------------------------------------------------------

#define NAME_LEN 100 //Длина имени команды
#define COMMAND(str) if(!strcmp(command, #str))
void menu(trie T, int n)
{
    int  i;
    char command[NAME_LEN];
    char buffer[STR_LEN];
    for(i = 0; i < n; ++i){
        scanf("%s %s", command, buffer);
        COMMAND(INSERT){
            insert_trie(T, buffer);
//            printf("Insert: %s\n", buffer);
//            show_trie(T, buffer);
            continue;
        }
        COMMAND(DELETE){
            delete_trie_node(T, buffer);
//            printf("Delete: %s\n", buffer);
//            show_trie(T, buffer);
            continue;
        }
        COMMAND(PREFIX){
            printf("%d\n", prefix(T, buffer));
            continue;
        }
    }
}

int main(void)
{
    init_memory(&Container);
    trie Root = init_trie();
    int n;
    scanf("%d", &n);
    menu(Root, n);
    destroy_container(&Container);
    return 0;
}

//-----------------------МЕНЕДЖЕР ПАМЯТИ------------------------------------

//------------Реализация очереди через кольцевой буфер-------------

cb_queue init_queue(int nel)
{
    cb_queue q = (cb_queue)malloc(sizeof(struct CB_Queue));
    q->data = (q_type *)malloc(nel*sizeof(q_type)); //Так как память будет перевыделяться, этот указатель должен быть отдельно
    q->cap = nel;
    q->count = q->head = q->tail = 0;
    return q;
}

void delete_queue(cb_queue q)
{
    free(q->data);
    free(q);
}

boolean queue_empty(cb_queue q)
{
    return !q->count;
}

void enqueue(cb_queue q, q_type x)
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

q_type dequeue(cb_queue q)
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

//-------------------------Работа с байтовой страницей--------------
mem_page create_mem_page()
{
    mem_page New = (mem_page)malloc(sizeof(struct MemPage));
    if(New == NULL){
        printf("Oops, there is no memory\n");
        destroy_container(&Container);
    }
    New->top = 0;
    return New;
}

boolean full_page(mem_page page)
{
    return page->top == MEM_LIST_SIZE;
}
//-----------------------Работа с хранилищем выделенной памяти---------------

//Инициализировать хранилище выделенной памяти и первую страницу
void init_memory(struct MemContainer *Container)
{
    Container->Freed = init_queue(MEM_LIST_SIZE);
    Container->pages = (mem_page *)malloc(sizeof(mem_page));
    Container->pages[0] = create_mem_page();
    Container->top = 0;
}

void increase_container(struct MemContainer *Container)
{
    ++Container->top;
    Container->pages = (mem_page *)realloc(Container->pages, (Container->top + 1) * sizeof(mem_page));
    Container->pages[Container->top] = create_mem_page();
}

void destroy_container(struct MemContainer *Container)
{
    int i;
    for(i = 0; i <= Container->top; ++i){
        free(Container->pages[i]);
    }
    free(Container->pages);
    delete_queue(Container->Freed);
}

//-----------------------------------------------------------------

//Если есть свободная память - она выделяется, если нет - смотрим в очередь,
//нет ли там удаленных элементов, если нет - выделяем новую страницу
mem_type *silly_alloc(struct MemContainer *C)
{
    mem_page last = C->pages[C->top];
    mem_type *ptr = NULL;
    if(!full_page(last)){
        ptr = last->array + last->top;
        ++last->top;
        return ptr; //Вернуть указатель на последний свободный элемент массива
    } else {
        if(!queue_empty(C->Freed)){
            return dequeue(C->Freed);
        } else {
            increase_container(C);
            last = C->pages[C->top];
            ptr = &last->array[last->top];
            ++last->top;
            return ptr; //Вернуть указатель на последний свободный элемент массива
        }
    }
}

void silly_free(mem_type *elem, struct MemContainer *Container)
{
    enqueue(Container->Freed, elem);
}


/*Строки с общими префиксами
Условие
Баллы: 3

Реализуйте структуру данных, представляющую множество строк с операциями Insert (добавление строки в множество), Delete (удаление строки из множества) и Prefix (подсчёт количества строк множества, имеющих указанных префикс). Операции Insert и Delete должны работать за время O(len (k)), где k – добавляемая или удаляемая строка, а операция Prefix – за время O(len (p)) – где p – префикс.

Составьте программу ptrie.c, демонстрирующую работоспособность реализованных операций.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит общее количество выполняемых операций n (0 < n ≤ 10000). Каждая из следующих n строк содержит описание операции.

Операция либо имеет форму INSERT k (добавить в множество строку k, 0 < len(k ) < 100000), либо форму DELETE k (удалить из множества имеющуюся в нём строку k), либо форму PREFIX p (вычислить количество строк в множестве, имеющих префикс p).

Отметим, что аргументы операций – это строки, составленные из маленьких латинских букв.

Кроме того, допустим вызов операции INSERT для строки, уже присутствующей в множестве.

Формат результата работы программы

Для каждой операции PREFIX вывести в стандартный поток вывода количество строк в множестве, имеющих указанный префикс.

Пример работы программы


Входные данные


Выходные данные

*/
/*
9
INSERT abcdef
INSERT abcd
INSERT abcd
PREFIX abcdef
INSERT ababab
PREFIX ab
DELETE abcd
PREFIX ab
PREFIX qwerty

http://pastebin.com/yQC9t6m1
1
3
2
0


valgrind ~/Qt/3-module/14-prefixes-bor-Debug/14-prefixes-bor

4 allocs, 4 frees, 15,206,832 bytes allocated - антон, 14

total heap usage: 47 allocs, 47 frees, 49,302,048 bytes allocated

*/
