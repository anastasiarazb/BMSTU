#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define alloc(n, type) (type *)malloc(n*sizeof(type))

typedef char boolean;
typedef int my_type;
typedef my_type (* bin_op)(my_type, my_type);

//--------------Работа со стеком-----------------------
typedef struct Stack{
    my_type *data;
    int cap;
    int top;
} stack;

void init_stack(stack *s, int n)
{
    s->data = alloc(n, my_type);
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

void push(stack *s, my_type x)
{
    if(s->top == s->cap){
        s->data = (my_type *)realloc(s->data, 2*s->cap*sizeof(my_type));
        s->cap *= 2;
    }
    s->data[s->top] = x;
    ++s->top;
}

my_type pop(stack *s)
{
    if(stack_empty(s)){
        printf("Error: stack is empty.\n");
        delete_stack(s);
        exit(1);
    }
    --s->top;
    return s->data[s->top];
}

void scan_data(stack *s)
{
    int i, nel = s->cap;
    for(i = 0; i < nel; ++i){
        scanf("%d", &s->data[i]);
    }
}
void show_stack(stack *s)
{
    printf("STACK: cap = %d, top = %d\n       ", s->cap, s->top);
    int i;
    for(i = 0; i < s->top; ++i){
        printf("%d ", s->data[i]);
    }
    putchar('\n');
}

//--------------------------------------------------

//-----------------Бинарные арифметические операции---------

my_type add(my_type a, my_type b)
{
    return a+b;
}

my_type sub(my_type a, my_type b)
{
    return a-b;
}

my_type mul(my_type a, my_type b)
{
    return a*b;
}

my_type divide(my_type a, my_type b)
{
    return a/b;
}

my_type max(my_type a, my_type b)
{
    return a > b ? a : b;
}

my_type min(my_type a, my_type b)
{
    return a < b ? a : b;
}

void arithmetics(stack *s, bin_op operation)
{
    my_type a = pop(s);
    my_type b = pop(s);
    push(s, operation(a, b));
}

//--------------------------------------------

void neg(stack *s)
{
    my_type new_val = pop(s);
    push(s, -new_val);
}

void dup(stack *s)
{
    push(s, s->data[s->top-1]);
}

void swap(stack *s)
{
    my_type a = pop(s);
    my_type b = pop(s);
    push(s, a);
    push(s, b);
}

//---------------------------------------------------------------------------

#define NAME_LEN 100 //Длина имени команды
#define COMMAND(str) if(!strcmp(command, #str))
void menu(stack *s, int n)
{
    int  i;
    char command[NAME_LEN];
    my_type arg;
    for(i = 0; i < n; ++i){
        scanf("%s", command);
        COMMAND(CONST){
            scanf("%d", &arg);
            push(s, arg);
        }
        if(s->top > 1){
            COMMAND(SUB){
                arithmetics(s, sub);
            }
            COMMAND(ADD){
                arithmetics(s, add);
            }
            COMMAND(MUL){
                arithmetics(s, mul);
            }
            COMMAND(DIV){
                arithmetics(s, divide);
            }
            COMMAND(MAX){
                arithmetics(s, max);
            }
            COMMAND(MIN){
                arithmetics(s, min);
            }

            COMMAND(SWAP){
                swap(s);
            }
        }
        COMMAND(NEG){
            neg(s);
        }
        COMMAND(DUP){
            dup(s);
        }
    }
}

int main(void)
{
    stack *S = alloc(1, stack);
    int n;
    scanf("%d", &n);
    init_stack(S, n);
    menu(S, n);
    printf("%d\n", S->data[S->top-1]);
    delete_stack(S);
    free(S);
    return 0;
}

/*Пусть стековая машина – это устройство для выполнения арифметических операций, использующее для хранения промежуточных результатов вычислений стек целых чисел. Подразумевается, что каждая операция берёт операнды из стека и оставляет на стеке результат.

Составьте программу stackmachine.c, моделирующую работу стековой машины.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит общее количество выполняемых операций n (0 < n ≤ 100000). Каждая из следующих n строк содержит описание операции.

Стековая машина должна обеспечивать выполнение следующих операций:

    CONST x – кладёт в стек число x (-1000000000 < x < 1000000000);
    ADD – сложение (снимает со стека два операнда a и b и кладёт в стек их сумму);
    SUB – вычитание (снимает со стека операнд a, затем снимает со стека операнд b, кладёт в стек a - b);
    MUL – умножение (снимает со стека два операнда a и b и кладёт в стек их произведение);
    DIV – деление (снимает со стека операнд a, затем снимает со стека операнд b, кладёт в стек результат целочисленного деления a на b);
    MAX – максимум двух чисел (снимает со стека два операнда a и b и кладёт в стек max(a,b));
    MIN – минимум двух чисел (снимает со стека два операнда a и b и кладёт в стек min(a,b));
    NEG – меняет знак числа, находящегося на вершине стека;
    DUP – кладёт в стек копию числа, находящегося на вершине стека;
    SWAP – меняет местами два числа, находящиеся на вершине стека.

Можно считать, что последовательность операций составлена правильно, то есть перед вызовом каждой операции стек содержит нужное ей количество операндов, деление на ноль и переполнение не возникают, и, кроме того, в результате выполнения всех операций на стеке остаётся единственное число.

Формат результата работы программы

В стандартный поток вывода необходимо вывести число, оставшееся на вершине стека в результате выполнения последовательности операций.

Пример работы программы


Входные данные


Выходные данные


6
CONST 10
NEG
CONST 20
SUB
CONST 15
MAX


30

valgrind /home/nastasia/Qt/3-module/2-stackmachine-Debug/2-stackmachine


*/
