#include <stdio.h>
#include <stdlib.h>

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

void comb_all(int nel);
//Функции обработки массива, хранящемся в виде битов числа типа int
int init_arr(int m);
int len_arr(int arr);
void show_arr(int arr);
int erase_q_arr(int arr, int q);
int add_q_arr(int arr, int q);

/*--------Глобальные переменные----------*/
int result;
int *mass;

int main(void)
{
    int n, i;
    scanf("%d ", &n);
    mass = (int *)malloc(n*sizeof(int));

    for(i = 0; i < n; ++i){
        scanf("%d", mass+i);
    }
    comb_all(n);
    printf("%d\n", result);
//    printf("%x", SET_BIT(18));
    free(mass);
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
    return GET_BIT(arr, q) ? (arr ^ SET_BIT(q)) : arr;
}

int add_q_arr(int arr, int q)
{
    return arr | SET_BIT(q);
}
/*----Функции генерации суммы элементов сочетаний и проверки на равенство стпени 2-ки---*/
int sum_of_mass(int arr)
{
    int i, sum;
    for(i = sum = 0; i < 8*(int)sizeof(int); ++i){
        if(GET_BIT(arr, i)){
            sum += mass[i];
        }
    }
    return sum;
}

int is_power2(int i)
{
    return i > 0 && !((i-1) & i);
}

/*--------------Функция вычисления сочетаний-----------------------*/

void comb_rec(int arr, int m, const int nel, int c, int counter) //Counter надо раскомментировать в DEBUG-версии, это счетчик глубины рекурсии
{
    if (!m){
//        SHOW_ARR(c);
        int sum = c; /*sum_of_mass(c);*/
        result += is_power2(sum);
        out("sum = %d, is_power2 = %d\n", sum, is_power2(sum));
    } else {
        int Q1 = arr; //arr не изменяется в цикле, постоянен на одном и том же уровне
        //вложенности рекурсивных вызовов, порожденных одним циклом
        int i;
        for(i = 0; i < nel; ++i){
            if(len_arr(Q1) < m) break;
            if(GET_BIT(arr, i)){

                SHOW_RECURSION_DEPTH(counter);
                out("i = %d, m = %d, counter = %d\n", i, m, counter);
                SHOW_RECURSION_DEPTH(counter);
                out("Q1: ");
                SHOW_ARR(Q1);
                SHOW_RECURSION_DEPTH(counter);
                out("arr: ");
                SHOW_ARR(arr);
                SHOW_RECURSION_DEPTH(counter);
                out("c: ");
                SHOW_ARR(c);

                Q1 = erase_q_arr(Q1, i); //Q1 изменяется в цикле на одном и том же уровне вложенности
                //множество "с" постоянно на одном и том же уровне вложенности, передается на другой
                //уровень, дополненный следующим элементом
                comb_rec(Q1, m - 1, nel, /*add_q_arr(c, i)*/c+mass[i], counter+1);
            }
        }
    }
}

void comb_rec_all(int arr, const int nel, int c, int counter) //Counter надо раскомментировать в DEBUG-версии, это счетчик глубины рекурсии
{
//    printf("sum = %d\n", c);
//    show_arr(c);
    //Так как новая перестановка строится добавлением последовательно всех
    //элементов к старой, мы можем не вычислять их заново, а просто выводить до перехода к
    //следующему уровню рекурсии
    result += is_power2(c);
    if (counter != nel){
        int Q1 = arr; //arr не изменяется в цикле, постоянен на одном и том же уровне
        //вложенности рекурсивных вызовов, порожденных одним циклом
        int i;
        for(i = counter; i < nel; ++i){
//            if(len_arr(Q1) < m) break; //Запрещает делать лишние итерации и генерировать
            // сочетания, которые заведомо не могут быть дополнены до нужной длины,
            // например C(3, 6) ({5}, {5, 6}), а при вычислении всех возможных сочетаний
            //нам нужно вычислять и их
           // if(GET_BIT(arr, i)){

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
                comb_rec_all(Q1, nel, c + mass[i], counter+1);
           // }
        }
    }
}

void comb_all(int nel)
{
    comb_rec_all(init_arr(nel), nel, 0, 0);
}

/*17
-84 -75 -69 -63 -56 -52 -50 -41 -34 -32 -28 -25 -15 -12 -10 -3 2
17 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17

21
-102729 -100381 -96133 -87647 -79649 -73456 -66765 -60898 -56492 -55020 -49820 -48185 -39504 -32935 -24181 -18257 -13696 -12965 -8221 -4988 -2983

*/
