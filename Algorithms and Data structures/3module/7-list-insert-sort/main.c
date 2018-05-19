#include <stdio.h>
#include <stdlib.h>

//#define DEBUG

#ifdef DEBUG
#define out printf
#define SHOW_LIST(n) {\
elem help = Head;\
printf("list:  ");\
    int i;\
for(i = 0; i < n; ++i){\
    printf("[%d] = %d;  ", help->index, help->v);\
    help = help->next;\
}\
putchar('\n');\
}
#else
#define out(...)
#define SHOW_LIST(n)
#endif

//------------Работа с двунаправленным списком--------------------
#define alloc_elem (struct Elem *)malloc(sizeof(struct Elem))

typedef int val_type;

typedef struct Elem {
    struct Elem *prev, *next;
    int v;
    int index;
} *elem;


elem init_double_linked_list()
{
    elem New_Head = alloc_elem;
    New_Head->next = New_Head->prev = New_Head;
    New_Head->index = 0;
    return New_Head;
}


void insert_after(elem Current_elem, val_type val)
{
    elem New_elem = alloc_elem;
    New_elem->v = val;
    Current_elem->next->prev = New_elem;
    New_elem->next = Current_elem->next; //Связали с предыдущим
    Current_elem->next = New_elem;
    New_elem->prev = Current_elem;
    New_elem->index = Current_elem->index + 1;
    ++New_elem->next->index;
    out("value = %d, index = %d, prev = %d, next = %d\n", New_elem->v, New_elem->index, New_elem->prev->index, New_elem->next->index);
}

void insert_before_head(elem Head, val_type val)
{
    insert_after(Head, Head->v);
    Head->v = val;

}

void insert_after_tail(elem Head, val_type val)
{
    elem Tail = alloc_elem;
    Tail->v = val;
    Tail->next = Head;
    Tail->prev = Head->prev;
    Tail->prev->next = Tail;
    Head->prev = Tail;
    Tail->index = Tail->prev->index + 1;
    out("value = %d, index = %d, prev = %d, next = %d\n", Tail->v, Tail->index, Tail->prev->index, Tail->next->index);
}

void delete_elem(elem Elem_to_del)
{
    out("delete_elem [%d] = %d\n", Elem_to_del->index, Elem_to_del->v);
    Elem_to_del->next->prev = Elem_to_del->prev; //Переписать следующий
    Elem_to_del->prev->next = Elem_to_del->next; //Переписать предыдущий
    --Elem_to_del->next->index;
    free(Elem_to_del);
}

void delete_tail(elem Head)
{
    elem current;
    current = Head->prev;
    current->prev->next = Head;
    Head->prev = current->prev;
    free(current);
}

void delete_list(elem Head)
{
    elem Tail = Head->prev;
    while(Tail != Head){
        Tail = Tail->prev;
        out("del_ind = %d\n", Tail->next->index);
        free(Tail->next);
    }
    free(Head);
}

//----------------------------------------------------
//----------------Сортировка вставками-----------------

int compare(elem a, elem compared)
{
    out("a[%d] = %d, compared = %d\n", a->index, a->v, compared->v);
    return a->v > compared->v;
}


void insertion_sort(elem Head, int n)
{
    elem temp = alloc_elem;            //Хранилище сравниваемого элемента
    elem i, loc;          //Индексы. i - начало неотсортированной части, loc - конец отсортированной
    int counter = 0;
    int counter2 = 0;
    for(i = Head->next; /*counter2 < 10 && */i != Head; counter2++){
//        printf("i[%d] = %d\n", i->index, i->v);
        for(*temp = *i, loc = i->prev; loc->next != Head && compare(loc, temp); loc = loc->prev){

        }
        if(loc->next == Head){
            insert_before_head(Head, i->v);
        } else {
        insert_after(loc, i->v);            //По окончании вставляем элемент на свое место
        }
        i = i->next;
        delete_elem(i->prev);
        SHOW_LIST(n)
    }
    free(temp);
}


int main(void)
{
    elem Head = init_double_linked_list();
    int n;
    scanf("%d", &n);
    int i, temp;
    if(n) scanf("%d", &Head->v);
    for(i = 1; i < n; ++i){
        scanf("%d", &temp);
        insert_after_tail(Head, temp);
    }
    SHOW_LIST(n)
    insertion_sort(Head, n);
    SHOW_LIST(n)
    elem help = Head;
    for(i = 0; i < n; ++i){
        printf("%d ", help->v);
        help = help->next;
    }
    putchar('\n');
    delete_list(Head);
    return 0;
}

/*Составьте программу listisort.c, выполняющую сортировку двунаправленного кольцевого списка целых чисел по возрастанию. В программе должен быть реализован алгоритм сортировки вставками.

Элементы списка должны быть представлены структурой
struct Elem {
    struct Elem *prev, *next;
    int v;
};

Алгоритм сортировки вставками, адаптированный для списков, должен выполнять не более n обменов элементов, где n – длина списка.

Программа должна считывать со стандартного потока ввода размер списка n и значения элементов списка. В стандартный поток вывода должны выводиться элементы отсортированного списка.

20
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 -1 -2 -3 -4 -5

7

*/
/*§5. Двунаправленный кольцевой список с ограничителем
Реализация двунаправленного кольцевого списка с ограничителем:
– каждый элемент списка представляется структурой v, prev, next , где
v ∈ V , prev – указатель на предыдущий элемент, next – указатель на сле-
дующий элемент;
– список представляет собой указатель на элемент-ограничитель
?, prev, next , где ? – произвольное значение из V (не используется).
Инициализация списка, проверка на пустоту и вычисление длины:
1 I n i t D o u b l e L i n k e d L i s t ( out l )
2 l ← у к а з а т е л ь на новый э л е м е н т
3 l.prev ← l , l.next ← l
4 L i s t E m p t y ( i n l ) : empty
5
 empty ← l.next = l
6 L i s t L e n g t h ( i n l ) : len
7
 len ← 0 , x ← l
8
 w h i l e x.next = l :
9
 len ← len + 1
10
 x ← x.next
20
Поиск элемента с заданным значением возвращает указатель на найден-
ный элемент или указатель на элемент-ограничитель, если ничего не най-
дено:
1
 ListSearch ( in l , in v ): x
2
 x ← l.next
3
 w h i l e x = l and x.v = v :
4
 x ← x.next
Вставка нового элемента y после уже существующего элемента x:
1
 I n s e r t A f t e r ( in x ,
 in y )
2
 z ← x.next
3
 x.next ← y
4
 y.prev ← x
5
 y.next ← z
6
 z.prev ← y
InsertBefore, InsertBeforeHead и InsertAfterTail – аналогично.
21
Удаление элемента x:
1
 D e l e t e ( i n x )
2
 y ← x.prev
3
 z ← x.next
4
 y.next ← z
5
 z.prev ← y
6
 −− Для порядка отсоединим x :
7
 x.prev ←NULL
8
 x.next ←NULL
DeleteAfter, DeleteBefore, DeleteHead и DeleteTail – аналогично.
Нетрудно сообразить, что операции ListLength и ListSearch работают за
время O (n), где n – длина списка. Все остальные операции работают за
константное время.
Принципиальные отличия списка от массива – отсутствие возможности
быстрого доступа к i-тому элементу, но зато размер списка не фиксиро-
ван, и, кроме того, вставка и удаление элементов выполняются за кон-
стантное время.
22
*/
