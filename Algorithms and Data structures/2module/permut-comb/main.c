#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SET_BIT(i) (1 << (i))
#define GET_BIT(arr, index) (1 & ((arr) >> (index)))

#define DEBUG

#ifdef DEBUG
#define out printf
#define newline putchar('\n');
#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
#define SHOW_ARR show_arr

#else
#define out
#define newline
#define SHOW_RECURSION_DEPTH(i)
#define SHOW_ARR
#endif

//Функции обработки массива, хранящемся в виде битов числа типа int
int init_arr(int m);
int len_arr(int arr);
void show_arr(int arr);
int erase_q_arr(int arr, int q);
int add_q_arr(int arr, int q);

//Функции обработки кортежа, хранящемся в массиве типа int
void show_cort(int *cort, int nel);
int *add_q_cort(int *cort, int q, int i);
void test_cort(int *cort, int m);

void permut(int arr, int m, int nel);
void permut_rec(int arr, int m, int nel, int counter, int *cort);

void PermutRep(int arr, int m, int nel);
void PermutRep_rec(int arr, int m, int nel, int counter, int *cort);

void comb(int arr, int m, int nel);
void comb_rec(short arr, int m, const int nel, int c, int counter);
void comb_all(int arr, int nel);
void comb_rec_all(int arr, const int nel, int c, int counter);

int array;
int mass[] = {1, 2};
int length = sizeof(mass)/sizeof(*mass);

int main(void)
{
    array = init_arr(6);
//    show_arr(array, mass, len);
//    array = erase_q_arr(array, 2);
//    show_arr(array, mass, len);
//    printf("%d \n", len_arr(array));
    int *cort = calloc(length+1, sizeof(int));
//    add_q_cort(cort, 3);
//    add_q_cort(cort, 2);
//    add_q_cort(cort, 2);
//    show_cort(cort);
//    printf("%d\n", init_arr(8));
//    comb_all(array, length);
    PermutRep(array, 2, 6);
//    comb(array, length/2, length);
//   permut(array, len, len);
//    test_cort(cort, 4);
    free(cort);
    return 0;
}

/*---Функции бработки множества, хранящегося в целом числе------*/

//Отметить все элементы, как принадлежащие множеству
int init_arr(int m)
{
    return SET_BIT(m)-1;
}

int len_arr(int arr) //Подсчет количества ненулевых битов
{
    int i, counter;
    for(i = counter = 0; i < 8*(int)sizeof(int); ++i){
        counter += GET_BIT(arr, i);
    }
    return counter;
}

//Показать элементы, принадлежащие множеству
void show_arr(int arr)
{
    int i;
    if(!arr){
        printf("<nothing>");
    } else {
        for(i = 0; i < (int)sizeof(int)*8; ++i){
            if(GET_BIT(arr, i)){
                printf("%d ", mass[i]);
            }
        }
    }
    newline;
}


//возвращает массив с удаленным элементом q
int erase_q_arr(int arr, int q)
{
//    printf("erase q = %c; ", mass[q]);
//    show_arr(arr - SET_BIT(q), mass, 4);
    return GET_BIT(arr, q) ? (arr ^ SET_BIT(q)) : arr;
}

int add_q_arr(int arr, int q)
{
//    printf("add q = %c; ", mass[q]);
//    show_arr(arr + SET_BIT(q), mass, 4);
    return arr | SET_BIT(q);
}

/*-----------Функции обработки кортежа, хранящегося в массиве-----*/
void test_cort(int *cort, int m)
{
    int i;
    for(i = 0; i < m; ++i){
        add_q_cort(cort, i, i);
        show_cort(cort, i+1);
    }
}
int *add_q_cort(int *cort, int q, int i)
{
//    printf("ADD: size = %d, last = %c\n", cort[0], mass[q]);
    cort[i] = q;
    return cort;
}
void show_cort(int *cort, int nel)
{
    int i;
    for(i = 0; i < nel; ++i){
//        printf("%c  cort[%d] = [%d]\n", mass[cort[i]], i, cort[i]);
        printf("%c ", mass[cort[i]]);
    }
    newline;
}

/*-------------------------------------------------------------------*/
//Размещения без повторений из m элементов по nel-элементому множеству
void permut(int arr, int m, int nel)
{
    int *cort = (int *)calloc(m, sizeof(int));
    permut_rec(arr, m, nel, 0, cort);
    free(cort);
}
void permut_rec(int arr, int m, int nel, int counter, int *cort)
{
    if(counter == m){
 //        printf("to do smth with arr\n");
        show_cort(cort, counter);
 //        printf("Where is my cort?!");
    } else {
        int i;
        for(i = 0; i < nel; ++i){
            if(GET_BIT(arr, i)){     //Для каждого элемента, принадлежащего множеству
 //                SHOW_RECURSION_DEPTH(counter);
 //                show_cort(cort, nel);
                permut_rec(erase_q_arr(arr, i), m, nel, counter + 1, add_q_cort(cort, i, counter));
            }
        }
    }
}

/*-------------------------------------------------------------------*/
//Размещения с повторениям
void PermutRep(int arr, int m, int nel)
{
    int *cort = (int *)malloc(m*sizeof(int));
    PermutRep_rec(arr, m, nel, 0, cort);
    free(cort);
}

void PermutRep_rec(int arr, int m, int nel, int counter, int *cort)
{
    if(counter == m){
        show_cort(cort, counter);
    } else {
        int i;
        for(i = 0; i < nel; ++i){
//            SHOW_RECURSION_DEPTH(counter);
//           printf("i = %d; m = %d; counter = %d     ", i, m, counter);
           add_q_cort(cort, i, counter);
//           show_cort(cort, counter+1);
//            printf("Go-to-permut.\n");
            PermutRep_rec(arr, m, nel, counter+1, cort);
        }
    }
}

/*-----------------Вычислить все комбинации из m по nel-------------------------------*/

void comb(int arr, int m, int nel)
{
    comb_rec(arr, m, nel, 0, 0);
}

void comb_rec(short arr, int m, const int nel, int c, int counter)
{
    if(!m){
        printf("to do smth with arr: ");
        show_arr(c);
 //        printf("Where is my array?!");
    } else {
        int Q1 = arr;
        int i;/*
        SHOW_RECURSION_DEPTH(counter);
        show_arr(c);*/
        for(i = 0; i < nel; ++i){
            if(GET_BIT(Q1, i)){     //Для каждого элемента, принадлежащего множеству
                 if(len_arr(Q1) < m) break;   //Запрещает делать лишние итерации и генерировать
                 // сочетания, которые заведомо не могут быть дополнены до нужной длины,
                 // например C(3, 6) ({5}, {5, 6})

//                SHOW_RECURSION_DEPTH(counter);
//                out(" LEN = %d, m = %d, counter = %d\n", len_arr(Q1), m, counter);
//                SHOW_RECURSION_DEPTH(counter);
//                printf("arr: ");
//                show_arr(arr);
//                SHOW_RECURSION_DEPTH(counter);
//                out("Q: ");
//                show_arr(Q1);
                SHOW_RECURSION_DEPTH(counter);
//                printf("c: ");
                show_arr(c);
                Q1 = erase_q_arr(Q1, i);
//                short new_c = add_q_arr(c, i);
               // show_arr(new_c, mass, nel);
                comb_rec(Q1, m - 1, nel, add_q_arr(c, i), counter + 1);
            }
        }
    }
}

/*--------------Вычислить все возможные комбинации---------------*/

void comb_all(int arr, int nel)
{
    comb_rec_all(arr, nel, 0, 0);
}

void comb_rec_all(int arr, const int nel, int c, int counter)
{
    show_arr(c);
    if (counter != nel){
        int Q1 = arr; //arr не изменяется в цикле, постоянен на одном и том же уровне
        //вложенности рекурсивных вызовов, порожденных одним циклом
        int i;
        for(i = 0; i < nel; ++i){
//            if(len_arr(Q1) < m) break; //Запрещает делать лишние итерации и генерировать
            // сочетания, которые заведомо не могут быть дополнены до нужной длины,
            // например C(3, 6) ({5}, {5, 6}), а при вычислении всех возможных сочетаний
            //нам нужно вычислять и их
            if(GET_BIT(arr, i)){

//                SHOW_RECURSION_DEPTH(counter);
//                out("i = %d, m = %d, counter = %d\n", i, m, counter);
//                SHOW_RECURSION_DEPTH(counter);
//                out("Q1: ");
//                SHOW_ARR(Q1);
//                SHOW_RECURSION_DEPTH(counter);
//                out("arr: ");
//                SHOW_ARR(arr);
//                SHOW_RECURSION_DEPTH(counter);
//                out("c: ");
//                SHOW_ARR(c);

                Q1 = erase_q_arr(Q1, i); //Q1 изменяется в цикле на одном и том же уровне вложенности
                //множество "с" постоянно на одном и том же уровне вложенности, передается на другой
                //уровень, дополненный следующим элементом
                comb_rec_all(Q1, nel, add_q_arr(c, i), counter+1);
            }
        }
    }
}



//valgrind -v /home/nastasia/Qt/2module/permut-comb-Debug/permut-comb

// http://pastebin.com/2J7X2u2w

