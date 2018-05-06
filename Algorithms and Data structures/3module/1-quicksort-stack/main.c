#include <stdio.h>
#include <stdlib.h>

//#define DEBUG

#ifdef DEBUG
#define out printf
#define newline putchar('\n');
#define SHOW_ARR(arr, left, right) void print_arr(int *arr, int left, int right); print_arr(arr, left, right);
#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
#define PRINTF_ARR(arr, nel, spec) {int i; for(i = 0; i < (nel); ++i) printf(spec, (arr)[i]); putchar('\n');}

#else
#define out(...)
#define newline putchar('\n');
#define SHOW_RECURSION_DEPTH(i)
#define SHOW_ARR(...)
#define PRINTF_ARR(arr, nel, spec)
#endif


#define alloc(n, type) (type *)malloc(n*sizeof(type))

typedef char boolean;
typedef int my_type;
typedef my_type (* cmp_type)(const my_type *, const my_type *);

typedef struct Task {
    int low, high;
} task_type;

//--------------Работа со стеком-----------------------
typedef struct Task stack_type;

typedef struct Stack{
    stack_type *data;
    int cap;
    int top;
} stack;

void init_stack(stack *s, int n);
void delete_stack(stack *s);
boolean stack_empty(stack *s);
void push(stack *s, stack_type x);
stack_type pop(stack *s);
void scan_data(stack *s);
void show_stack(stack *s);
//-------------------------------------

int partition(my_type *base, int low, int high, cmp_type cmp);
void quicksort(my_type *base, int m, cmp_type compare);

int compare(const int *elem1, const int *elem2)
{
//    printf("COMPARE: elem1 = %d, elem2 = %d\n", *elem1, *elem2);
    if(*elem1 == *elem2) return 0;
    return (*elem1 > *elem2) ? 1 : -1;
}
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int main(void)
{
    int n;
    scanf("%d", &n);
    int *arr = alloc(n, int);
    int i;
    for (i = 0; i < n; ++i) scanf("%d", arr + i);
    quicksort(arr, n, compare);
    for (i = 0; i < n; ++i) printf("%d ", arr[i]);
    free(arr);
    return 0;
}

int partition(my_type *base, int low, int high, cmp_type cmp)
{
    SHOW_ARR(base, low, high);
    int i, j;                    //i - кол-во элементов, меньших опорного base[high]
    for(j = i = low; j < high; ++j){
        if(cmp(base + j, base + high) == -1){
            swap(base+i, base+j);
            ++i;
        }
    }
    swap(base+i, base+high);
    SHOW_ARR(base, low, high);
    return i;
}


void quicksort(my_type *base, int nel, cmp_type compare)
{
    stack *task_stack = alloc(1, stack);
    init_stack(task_stack, 4*nel);
    int q;          //Граница разделения
    int len1, len2; //Длины отрезков, получившихся при разделении
    task_type current_task, new_task;
    current_task.low = 0;
    current_task.high = nel-1;
    int low, high;
    push(task_stack, current_task); //На дне стека - весь массив
    while (!stack_empty(task_stack)){
        //Разбить верхний массив на стеке на 2 части
        current_task = pop(task_stack);
        low = current_task.low;
        high = current_task.high;
        q = partition(base, low, high, compare);
        len1 = q - low;
        len2 =  high - q;
        out("SORT: current low = %d, high = %d, q = %d, len1 = %d, len2 = %d\n", current_task.low, current_task.high, q, len1, len2);
        //Положить на вершину стека непустой подмассив
        //Элемент q уже стоит на своем месте
        if(len1 > 1){
            new_task.low = low;
            new_task.high = q - 1;
            push(task_stack, new_task);
        }
        if(len2 > 1){
            new_task.low = q + 1;
            new_task.high = high;
            push(task_stack, new_task);
        }
    }
    delete_stack(task_stack);
    free(task_stack);
}


//--------------Работа со стеком-----------------------

void init_stack(stack *s, int n)
{
    s->data = alloc(n, stack_type);
    s->cap = n;
    s->top = 0;
}

void delete_stack(stack *s)
{
    free(s->data);
    s->cap = 0;
    s->top = 0;
}

boolean stack_empty(stack *s)
{
    return !s->top;
}

void push(stack *s, stack_type x)
{
    if(s->top == s->cap){
        s->data = (stack_type *)realloc(s->data, 2*s->cap*sizeof(stack_type));
        s->cap *= 2;
    }
    s->data[s->top] = x;
    ++s->top;
}

stack_type pop(stack *s)
{
    if(stack_empty(s)){
        printf("Error: stack is empty.\n");
        delete_stack(s);
        exit(1);
    }
    --s->top;
    return s->data[s->top];
}


void show_data(stack_type elem)
{
    printf("low = %d,  high = %d", elem.low, elem.high);
    putchar('\n');
}

void show_stack(stack *s)
{
    printf("STACK: cap = %d, top = %d\n       ", s->cap, s->top);
    int i;
    for(i = 0; i < s->top; ++i){
        show_data(s->data[i]);
    }
    putchar('\n');
}

//--------------------------------------------------
void print_arr(int *arr, int left, int right)
{
    printf("SUB_ARR: [%d : %d]\n     ", left, right);
    while(left <= right){
        printf("%d ", arr[left++]);
    }
    putchar('\n');
}

/*Необходимо составить программу qsstack.c, осуществляющую сортировку массива целых чисел в порядке возрастания.

В программе должен быть реализован нерекурсивный алгоритм быстрой сортировки, использующий в своей работе стек заданий. Каждое задание описывает координаты подмассива, который нужно отсортировать, и представляет собой структуру
struct Task {
    int low, high;
};

Программа должна считывать со стандартного потока ввода размер массива n и значения элементов массива. В стандартный поток вывода должны выводиться элементы отсортированного массива. */
/*
Антон
1 http://pastebin.com/8jepLE7F
Антон
2 http://pastebin.com/HKL2RuTg
3 http://pastebin.com/RVsae1GC
4 http://pastebin.com/UhidDfTy
5 http://pastebin.com/pxafpjwh
6 http://pastebin.com/PeDs56N8
7 http://pastebin.com/FAsaUUGm
8 http://pastebin.com/svB01pse
9 http://pastebin.com/1JrX0uWm
10 http://pastebin.com/wKh3z2a9
11 http://pastebin.com/aSr97Zuc
12 http://pastebin.com/VV591MJR
13 http://pastebin.com/seh5K9TZ
14 http://pastebin.com/yQC9t6m1


valgrind ~/Qt/3-module/1-quicksort-stack-Debug/1-quicksort-stack
  */
