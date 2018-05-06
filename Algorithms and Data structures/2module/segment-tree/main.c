#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#define DEBUG

#ifdef DEBUG
#define out printf
#define newline putchar('\n');
#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
#define NUMELEM(array) (sizeof(array)/sizeof(*(array)))
#define PRINTF_ARR(arr, nel, spec) {int i; for(i = 0; i < (nel); ++i) printf(spec, (arr)[i]); putchar('\n');}

#else
#define out(...)
#define newline
#define SHOW_RECURSION_DEPTH(i)
#define PRINTF_ARR(arr, nel, spec) {int i; for(i = 0; i < (nel); ++i) printf(spec, (arr)[i]); putchar('\n');}
#endif

#define MAXLEN (1000000+1)
#define KEY(a) ((int)((a)-'a'))
#define SET_BIT(i) (1 << (i))
#define IS_HD(i) (!(((i)-1) & (i)))

typedef int32_t my_type;
typedef my_type (*bin_op)(my_type, my_type);

void segment_tree_build(int n, bin_op op);
int segment_tree_query(int r, int l, int n, bin_op op);
void segment_tree_update(int i, int val, int n, bin_op op);
void menu(int n, bin_op op);
int scan_encode(my_type *arr, int max);

my_type mass[MAXLEN];
my_type *seg_tree;


/*-------Бинарные операции-----------*/
my_type sum(my_type a, my_type b)
{
    return a + b;
}
my_type max(my_type a, my_type b)
{
    return (a > b) ? a : b;
}

my_type XOR(my_type a, my_type b)
{
    return a ^ b;
}


#define OPERATION XOR

int main(void)
{
    int len = scan_encode(mass, MAXLEN);
    seg_tree = (int *)malloc(4*len*sizeof(my_type));
    segment_tree_build(len, OPERATION);
    menu(len, OPERATION);
    free(seg_tree);
    return 0;
}

#define COMMAND(str) if(!strcmp(command, str))
void menu(int n, bin_op op)
{
    int m, i;
    char command[4]; //Команда состоит не более чем из 3-х букв.
    scanf("%d", &m);
    int arg1, arg2;
    for(i = 0; i < m; ++i){
        scanf("%3s", command);
        COMMAND("UPD"){
            scanf("%d ", &arg1);
            scan_encode(mass+arg1, MAXLEN-arg1);
            segment_tree_build(n, op);
        }
        COMMAND("HD"){
            scanf("%d %d", &arg1, &arg2);
            printf(IS_HD(segment_tree_query(arg1, arg2, n, op)) ? "YES\n" : "NO\n");
        }
    }
}

//-----------------Обработка массива с исходными данными-------------------

//Считывание и запись в массив не буквы, числа, в котором в единицу установлен только бит,
//соответствующий порядковому номеру буквы в алфавите
int scan_encode(my_type *arr, int max)
{
    scanf(" ");
    int i, c;
    for(i = 0; i < max && !isspace(c = getchar()); ++i){
        arr[i] = SET_BIT(KEY(c));
    }
//    arr[i] = 0;
    return i;
}

//Трассировка: вывод двоичного представления числа и возврат его значения
my_type show_int_bin(const my_type num)
{
    int bit;
//    for(bit = 8 * (int)sizeof(int) - 1; bit >= 0; --bit){
    for(bit = 0; bit < 8*(int)sizeof(my_type); ++bit){
        printf("%d", 1 & (num >> bit));
    }
    putchar('\n');
    return num;
}

/*--------------Функции для работы с деревьями отрезков---------------------*/

#define INIT_CHILDREN(i) \
    int m = (a+b)/2;\
    int left_child = 2*(i)+1;\
    int right_child = left_child + 1;    //Медиана и индексы потомков

#define GO_TO_LEFT left_child, a, m      //Обращение к правому (левому) потомку
#define GO_TO_RIGHT right_child, m+1, b

   /*------------Построение дерева----------*/
void seg_build_help(int current, int a, int b, bin_op op, int counter)
{
    if(a == b){
        seg_tree[current] = mass[a];
    } else {
        INIT_CHILDREN(current);
        seg_build_help(GO_TO_LEFT, op, counter+1);
        seg_build_help(GO_TO_RIGHT, op, counter+1);
        seg_tree[current] = op(seg_tree[left_child], seg_tree[right_child]);
    }
    SHOW_RECURSION_DEPTH(counter);
    out("current = %d, [%d:%d] = %d\n", current, a, b, seg_tree[current]);
}

void segment_tree_build(int n, bin_op op)
{
    out("SEGMENT TREE BUILD:\n");
    seg_build_help(0, 0, n-1, op, 0);
}


      /*----------------Обращение к дереву-----------------*/
int seg_query_help(int l, int r, int current, int a, int b, bin_op op, int counter)
{
    SHOW_RECURSION_DEPTH(counter);
    out("current = %d, l:r = [%d:%d] m = %d a:b = [%d:%d]\n",
        current, l, r, (a+b)/2, a, b);
    if((l == a) && (r == b)){
        SHOW_RECURSION_DEPTH(counter);
        out("l:r = a:b, t[a:b] = t[%d] = %d\n", current, seg_tree[current]);
        return seg_tree[current];
    }
    INIT_CHILDREN(current);
    if(r <= m){
        return seg_query_help(l, r, GO_TO_LEFT, op, counter+1);
    }
    if(l > m){
        return seg_query_help(l, r, GO_TO_RIGHT, op, counter+1);
    }
    return  op(seg_query_help(l, m, GO_TO_LEFT, op, counter+1),
               seg_query_help(m+1, r, GO_TO_RIGHT, op, counter+1));

}
int segment_tree_query(int l, int r, int n, bin_op op)
{
    out("SEGMENT TREE QUERY: [%d:%d]\n", l, n);
    return seg_query_help(l, r, 0, 0, n-1, op, 0);
}


     /*-----------------Обновление дерева----------------------*/

void seg_upd_help(int current, int a, int b, const int i, const int val, bin_op op, int counter)
{
    if(a == b){
        seg_tree[current] = val;
    } else {
        INIT_CHILDREN(current);
        (i <= m) ? seg_upd_help(GO_TO_LEFT, i, val, op, counter+1)
                 : seg_upd_help(GO_TO_RIGHT, i, val, op, counter+1);
        seg_tree[current] = op(seg_tree[left_child], seg_tree[right_child]);
    }
    SHOW_RECURSION_DEPTH(counter);
    out("current = %d, [%d:%d] = %d\n", current, a, b, seg_tree[current]);
}

void segment_tree_update(int i, int val, int n, bin_op op)
{
    out("SEGMENT TREE UPDATE: val = %d, i = %d\n", val, i);
    seg_upd_help(0, 0, n-1, i, val, op, 0);
}

//valgrind ~/Qt/2module/segment-tree-Debug/segment-tree < ~/Qt/2module/skorb22.txt > ~/Qt/2module/out.txt

/*

Гипердром – это строка, из букв которой можно составить палиндром. Другими словами, любая буква имеет чётное количество вхождений (возможно, нулевое) в гипердром чётной длины. Если же гипердром имеет нечётную длину, то ровно одна буква имеет нечётное количество вхождений.

Составьте программу rangehd.c, определяющую, является ли указанная подстрока строки гипердромом. Строка время от времени может изменяться.
Формат входных данных

Первая строчка, считываемая со стандартного потока ввода, содержит строку размера n (0 < n ≤ 1000000). Строка состоит из маленьких латинских букв.

Вторая строчка содержит общее количество выполняемых операций m (0 < m ≤ 10000). Каждая из следующих m строчек содержит описание операции.

Операция либо имеет форму HD l r (определить, является ли подстрока, начинающаяся с индекса l и заканчивающаяся индексом r, гипердромом), либо форму UPD i s (заменить подстроку, начинающуюся с индекса i, строкой s).

Формат результата работы программы

Для каждой операции HD вывести в стандартный поток вывода слово «YES», если подстрока является гипердромом, или слово «NO» в противном случае.

Пример работы программы


Входные данные


Выходные данные


aababab
6
HD 0 6
HD 1 6
HD 0 3
UPD 2 qqq
HD 0 6
HD 1 5


YES
NO
NO
NO
YES



tt
11
HD 0 0
HD 1 1
HD 0 1
UPD 0 u
HD 0 0
HD 1 1
HD 0 1
UPD 1 u
HD 0 0
HD 1 1
HD 0 1


YES
YES
YES
YES
YES
NO
YES
YES
YES*/
