#define _USE_MATH_DEFINES
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <assert.h>
#include <float.h>
#define SQUARE(x) ((x)*(x))
#define newline putchar('\n');

#define SHOW_EXPT(i) {int k; for(k = 0; k < i; ++k) printf(" ");}
#define POW2(n) (1 << (n))           //Быстрое вычисление степени 2
//#define EXPECTED_DX 1.75/POW2(24)  //Желаемый шаг, в двоичном представлении 1.75_10 == 1.11_2
//#define EXPECTED_DX (1.0+FLT_EPSILON)/POW2(25)

typedef float type;

#define DEBUG
#ifdef DEBUG
#define out printf
#define SHOW_MANT show_mant
#define SHOW_PARTS show_parts
#else
#define out(...)
#define SHOW_MANT(...)
#define SHOW_PARTS(...)
#endif

union Num;
float show_mant(const float a);
float make_float(int, int, int);
float show_parts(float);
int get_expt(float f);
struct Curve;

float dx_straight(float fi, struct Curve* ptr)
{
    return 1.0;
}

float dy_straight(float fi, struct Curve* ptr)
{
    return 0.0;
}

struct Curve
{
    float (*dx)(float fi, struct Curve *);
    float (*dy)(float fi, struct Curve *);
} straight = {
    dx_straight,
    dy_straight
};


float f(float fi, void *params)
{
    struct Curve *c = params;
    float dx = c->dx(fi, c);
    float dy = c->dy(fi, c);
    return sqrt(pow(dx, 2)+pow(dy, 2));
}

type integrate(type left, type right, unsigned long steps, type (*func)(type, void *), void *params)
{
    type res = 0;
    type x = left;

    type last_x = x;
    type dx = (right-left)/steps;
    type dx_now, dx_last;
    int dx_const = 1;
//    show_mant(dx);
    unsigned long counter = 0;
    dx_now = dx_last = (x + dx) - x;
//    SHOW_MANT(left);
//    SHOW_EXPT(get_expt(left) - get_expt(dx));
//    SHOW_MANT(dx);
    float real_dx, next_x;

    for(x = left;
//        x < right
        x+dx < right
        && counter < 10000000000; /*x += dx*/){

//        next_x = x+dx;
//        real_dx = next_x-x;
//        res += func(x + real_dx/2, params)*real_dx;
        res += func(x + dx/2, params)*dx;
        last_x = x;
        x += dx;
        dx_now = x-last_x;
        dx_const &= (dx_now == dx_last);
        dx_last = dx_now;
        last_x = x;
        ++counter;
    }
    dx_now = right - x;
    res += func(x + dx_now/2, params)*dx_now;
    ++counter;
    printf("%lu; %lu; %G; %G; %s; %G; %G",
           steps, counter, dx, dx_last, dx_const ? "true" : "0", res, dx*steps*dx/dx_last/*+dx*/);

    return res;
}



int main()
{
    setlocale( LC_ALL, "" );
    /* использовать , или . для отделения дробной части */
//    printf("%G %s\n", f(0,0,0,0), f(0,0,0,0) == /*(double)*/1? "true" : "false");
    unsigned long n;
    float left, right;
    printf("Ожидаемое число шагов; Реальное число шагов; dx ожидаемое; "
           "dx реальное; Разность постоянна?; "
           "Полученный результат; Ожидаемый результат с учетом погрешности; Требуемый результат\n");
//    float expected_dx = EXPECTED_DX;  //Желаемый шаг, в двоичном представлении 1.75_10 == 1.11_2
    left = 0.5;
//    integrate(0.5, 0.9375, 1 << 22, f, &straight);
    int d_radix = 2;     //На сколько разрядов в двоичном представлении смещается мантисса переменной right
    right = make_float(0, -1, 0b10101010101010101010101);
    for(d_radix = 2; d_radix < 23; d_radix += 2){
        n = POW2(d_radix);
//        right = left + expected_dx*n;
//        SHOW_MANT(right);
//        SHOW_MANT(expected_dx);
//        out("\n");
//        newline;
//        newline;

        integrate(left, right, n, f, &straight);
        printf("; %G\n", right-left);
    }

//    printf("\n\nПредполагаемое число шагов; Покрытый интервал\n");
//    int i;
//    for(i = 0; i <)

    return 0;
}

//type f2(type fi, type r, type m, type h)
//{
//    return 1.5*sqrt(2.9725-1.65*cos(fi));
//}


// //type length(type left, type right, unsigned long steps)
// //{
// //    type dx = (right-left)/steps, x;
// //    type len = right-left;
// //    unsigned long counter = 0;
// //    printf("Текущий шаг;Ширина шага;Текущий х;Итог;\n");
// //    for(x = left; x < right; x += dx){
// //        printf("%lu; %f; %f; %f;\n", counter, dx, x, len);
// //        ++counter;
// //    }
// // //    printf("%lu; %f; %f; %f;\n", counter, dx, x, len); //Сколько итераций было реально вычислено
// //    return x;
// //}

//type integrate(type left, type right, unsigned long steps, type (*func)(type, type, type, type), type r, type m, type h)
//{
//    type res = 0;
//    type x = left;
//    type len = right-left;
//    type dx = (right-left)/steps;
//    unsigned long counter = 0;
//    for(x = left; x < right && counter < 10000000000; x += dx){
//        res += func(x + dx/2, r, m, h)*dx;
//        ++counter;
//        if(!(counter%1000)) printf("%lu; %lu; %G; %G; %G; %G; %G; %G\n", steps, counter, len, x, dx, 1, res, func(0, r, m, h));
//    }
// //    res *= dx;
//    printf("%lu; %lu; %G; %G; %G; %G; %G\n", steps, counter, len, x, dx, 1, res);
//    return res;
//}

//int main()
//{
//    long n;
//    type L = 0, R = 1;
//    setlocale( LC_ALL, "" );
//    printf("%d\n", f(0,0,0,0));
//    printf("Ожидаемое число шагов; Реальное число шагов; Ожидаемый промежуток; Реально покрытый промежуток;Смещение dx;Ожидаемый результат;Реальный результат;\n");
// //    for(n = 1; n < 1000000000; n *= 10){
//        integrate(L, R, 1000000, f, 0, 0, 0);
// //    }
//    return 0;
//}

/*
#define _USE_MATH_DEFINES
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

#define SQUARE(x) ((x)*(x))

typedef float type;
#define out printf
//#define out(...)

type dx(type fi, type r, type m, type h)
{
    return 1;
}

type dy(type fi, type r, type m, type h)
{
    return 0;
}

type f(type fi, type r, type m, type h)
{
    type der_x = dx(fi,r,m,h),
          der_y = dy(fi,r,m,h);
//    printf("%.15G ", SQUARE(der_x)+SQUARE(der_y));
    return sqrt(SQUARE(der_x)+SQUARE(der_y));
//    return sqrt(pow(der_x, 2) + pow(der_y, 2));
}

type f2(type fi, type r, type m, type h)
{
    return 1.5*sqrt(2.9725-1.65*cos(fi));
}

type integrate(type left, type right, unsigned long steps, type (*func)(type, type, type, type), type r, type m, type h)
{
    type res = 0;
    type x = left;
    type len = right-left;
    type dx = (right-left)/steps;
    unsigned long counter = 0;
    printf("Текущий шаг;Ширина шага dx;Текущий х;Итоговое покрытие; Результат\n");
    for(x = left; x < right; x += dx){
//        out("dx = %f, x0 = %f, f(x0) = %f, Si = %f\n", dx, x + dx/2, func(x + dx/2, r, m, h), func(x + dx/2, r, m, h)*dx);
        res += func(x + dx/2, r, m, h);
        ++counter;
    }
    res *= dx;
    printf("%lu; %f; %f; %f; %f;\n", counter, dx, x, len, res);
    return res;
}

type length(type left, type right, unsigned long steps)
{
    type dx = (right-left)/steps, x;
    type len = right-left;
    unsigned long counter = 0;
    printf("Текущий шаг;Ширина шага;Текущий х;Итог;\n");
    for(x = left; x < right; x += dx){
//        printf("%lu; %f; %f; %f;\n", counter, dx, x, len);
        ++counter;
    }
    printf("%lu; %f; %f; %f;\n", counter, dx, x, len); //Сколько итераций было реально вычислено
    return x;
}

int main()
{
    long n;
    type L = 0, R = 1;
//    type V, V0 = F( R ) - F( L );
    type V, V0 = 5;       //31.81995
    type r = 1, m = 0.5, h = 1.1;
    // точное решение
    setlocale( LC_ALL, "" );
    // использовать , или . для отделения дробной части
//    integrate( L, R, 1000000, f, 0, 0, 5 );
    integrate( L, R, 1000000, f, 0, 0, 5 );
//    length(L, R, 1000000);
//    printf("%G %s\n", f(0,0,0,5), f(0,0,0,5) == (type)1? "true" : "false");
//    V = integrate( L, R, 3, f, r, m, h );
//    printf("Текущий шаг;Ширина шага;Текущий х;\n");
//    for( n = 1; n < 201; n += 1){
//        V = integrate( L, R, n, f, r, m, h );
//        // приближенное решение для n шагов
//        printf( "%ld; %.5f; %.5f; %.5f; %.6G\n", n, V, V0, V-V0, (V-V0)/V0 );
//        // n и относительная ошибка
//    }
    return 0;
}


type integrate(type left, type right, unsigned long steps, type (*func)(type, type, type, type), type r, type m, type h)
{
    type res = 0;
    type x = left;
    type len = right-left;
    type dx = (right-left)/steps;
    unsigned long counter = 0;
    printf("Текущий шаг;Ширина шага dx;Текущий х;Итоговое покрытие; Результат\n");
    for(x = left; x < right; x += dx){
        res += func(x + dx/2, r, m, h)*dx;
        ++counter;
//        printf("%lu; %f; %f; %f; %f;\n", counter, dx, x, len, res);
    }
    res *= dx;
//    printf("%lu; %f; %f; %f; %f;\n", counter, dx, x, len, res);
    return res;
}*/

/*type L = 0, R = 1; length(L, R, 100);*/


//| tee /home/nastasia/Архитектура/Арха_дз/тесты_для_прямой/ошибка_смещения_2.txt
