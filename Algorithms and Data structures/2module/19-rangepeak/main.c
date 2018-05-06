#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define PRINTF_ARR(...)
#endif

typedef char boolean;
typedef int my_type;
typedef my_type (* bin_op)(my_type, my_type);
typedef my_type (* un_op)(my_type);

boolean is_peak(int arg);
my_type sum(my_type a, my_type b);
void segment_tree_build(int n, bin_op op);
int segment_tree_query(int r, int l, int n, bin_op op);
void segment_tree_update(int i, int val, int n, bin_op op);
void menu(int n, bin_op op);

#define OPERATION sum
int *mass;
int *seg_tree;

int last_index;

int main(void)
{
    int n;
    scanf("%d", &n);
    last_index = n-1;
    mass = (int *)malloc(n*sizeof(int));
    int i;
    for(i = 0; i < n; ++i){
        scanf("%d", mass+i);
    }
    seg_tree = (int *)malloc(4*n*sizeof(my_type));
    segment_tree_build(n, OPERATION);
    menu(n, OPERATION);
    free(mass);
    free(seg_tree);
    return 0;
}
#define COMMAND(str) if(!strcmp(command, str))
void menu(int n, bin_op op)
{
    int m, i;
    char command[5]; //Команда состоит фиксированно не более чем из 4-х букв.
    scanf("%d", &m);
    int arg1, arg2;
    for(i = 0; i < m; ++i){
        scanf("%4s %d %d", command, &arg1, &arg2);
        COMMAND("UPD"){
            mass[arg1] = arg2;
            segment_tree_update(arg1, is_peak(arg1), n, op);
            if(arg1) segment_tree_update(arg1-1, is_peak(arg1-1), n, op);
            if(arg1 < last_index) segment_tree_update(arg1+1, is_peak(arg1+1), n, op);
        }
        COMMAND("PEAK"){
            printf("%d\n", segment_tree_query(arg1, arg2, n, op));
        }
    }
}

/*--------------Функции опознавания пиков----------------------------*/

boolean not_less(int i, int j)
{
    return mass[i] >= mass[j];
}

boolean is_peak(int arg)
{
    if(!last_index) return 1; //В последовательности только 1 элемент
    if(!arg){
        return not_less(arg, arg+1);
    }
    if(arg == last_index){
        return not_less(arg, arg-1);
    }
    return not_less(arg, arg-1)
            && not_less(arg, arg+1);
//    return ((!arg && not_less(arg, arg+1) && printf("!arg && not_less(arg, arg+1)\n"))
//            || (printf("%d (arg == last_index) not_less(arg, arg-1)\n", arg) && (arg == last_index)
//                && not_less(arg, arg-1))
//            || ((not_less(arg, arg-1)
//                && not_less(arg, arg+1)) && printf("(arg == last_index) not_less(arg, arg-1)\n"))) ?
//           1 : 0;
}

/*----------------------------------------------------*/

/*-------Бинарные операции-----------*/
my_type sum(my_type a, my_type b)
{
    return a + b;
}

my_type max(my_type a, my_type b)
{
    return (a > b) ? a : b;
}

/*----------------------------------*/

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
        seg_tree[current] = is_peak(a);
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

//void seg_upd_help(int current, int a, int b, const int i, const int val, bin_op op, int counter)
//{
//    if((b - a) == 2){
//        seg_tree[current] = val;
//        if(current) seg_tree[current-1] = is_peak(current-1);
//        if(current < last_index) seg_tree[current+1] = is_peak(current+1);
//    } else {
//        INIT_CHILDREN(current);
//        (i <= m) ? seg_upd_help(GO_TO_LEFT, i, val, op, counter+1)
//                 : seg_upd_help(GO_TO_RIGHT, i, val, op, counter+1);
//        seg_tree[current] = op(seg_tree[left_child], seg_tree[right_child]);
//    }
//    SHOW_RECURSION_DEPTH(counter);
//    out("current = %d, [%d:%d] = %d\n", current, a, b, seg_tree[current]);
//}

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

// valgrind ~/Qt/2module/19-rangepeak-Debug/19-rangepeak < /home/nastasia/Qt/2module/peak20.txt > /home/nastasia/Qt/2module/out.txt
/*Количество пиков в подпоследовательности
Условие
Баллы: 2

Напомним, что элемент последовательности чисел, значение которого – не меньше значений его непосредственных соседей, называется пиком.

Составьте программу rangepeak.c, выполняющую вычисление количества пиков на различных интервалах последовательности целых чисел, которая время от времени изменяется.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит размер последовательности n (0 < n ≤ 1000000). Во второй строке перечислены элементы последовательности. Каждый элемент представляет собой целое число, находящееся в диапазоне от -109 до 109. Элементы разделяются пробелами.

Третья строка содержит общее количество выполняемых операций m (0 < m ≤ 20000). Каждая из следующих m строк содержит описание операции.

Операция либо имеет форму PEAK l r (вычислить количество пиков в подпоследовательности, начинающейся с элемента с индексом l и заканчивающейся элементом с индексом r), либо форму UPD i v (присвоить значение v элементу с индексом i).

Формат результата работы программы

Для каждой операции PEAK вывести в стандартный поток вывода количество пиков в указанной подпоследовательности.

Пример работы программы


Входные данные


Выходные данные


5
1 2 3 4 3
6
PEAK 0 4
PEAK 0 2
PEAK 4 4
UPD 3 2
PEAK 0 4
PEAK 0 2


1
0
0
2
1
*/

