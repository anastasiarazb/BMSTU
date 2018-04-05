#define _USE_MATH_DEFINES
#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <locale.h>

typedef double my_type;
typedef my_type (*f_type)(my_type, my_type, my_type, my_type);


#define out printf
//#define out(...)

my_type dx(my_type fi, my_type r, my_type m, my_type h)
{
    return r*(m+1)*(-m*sin(m*fi)+h*(m+1)*sin((m+1)*fi));
}

my_type dy(my_type fi, my_type r, my_type m, my_type h)
{
    return r*(m+1)*(m*cos(m*fi)-h*(m+1)*cos((m+1)*fi));
}

my_type f(my_type fi, my_type r, my_type m, my_type h)
{
    return sqrt(pow(dx(fi, r, m, h), 2)+pow(dy(fi, r, m, h), 2));
}

my_type f2(my_type fi, my_type r, my_type m, my_type h)
{
    return 1.5*sqrt(2.9725-1.65*cos(fi));
}

my_type integrate1(my_type left, my_type right, size_t steps, f_type func, my_type r, my_type m, my_type h)
{
    my_type res = 0;
    my_type x = left;
    my_type dx = (right-left)/steps;
    unsigned long counter = 0;          //Заводим счетчик
    for(x = left; x < right; x += dx){
        res += func(x + dx/2, r, m, h)*dx;
        ++counter;                      //Увеличиваем счетчик на единицу после каждой выполненной итерации
    }
    printf("%lu; %lu; %f; %f; ", counter, steps, x, right-left); // Число реально выполненных итераций, требуемое число итераций, реально посчитанный интервал, требуемый интервал
    return res;
}

my_type integrate2(my_type left, my_type right, size_t steps, f_type func, my_type r, my_type m, my_type h)
{
    my_type res = 0;
    my_type x = left;
    my_type dx = (right-left)/steps;
    size_t counter = 0;
    for(x = left; x+dx <= right; x += dx){
        res += func(x + dx/2, r, m, h)*dx;
        ++counter;
    }
//    printf("%lu; ", counter);
    dx = right-x; //Последний интервал считается отдельно
    res += func(x+dx/2, r, m, h)*dx;
    ++counter;
    printf("%lu; %lu; %f; %f; ", counter, steps, x+dx, right-left); // Число реально выполненных итераций, требуемое число итераций, реально посчитанный интервал, требуемый интервал
    return res;
}

my_type integrate3(my_type left, my_type right, unsigned long steps, f_type func, my_type r, my_type m, my_type h)
{
    my_type res = 0;
    my_type x = left;
    my_type dx = (right-left)/steps;
    unsigned long counter = 0;
    for(x = left; counter <= steps; x += dx){
        res += func(x + dx/2, r, m, h)*dx;
        ++counter;
    }
    printf("%lu; ", counter-1);
    printf("%f; %f; ", x, right-left); //Число реально выполненных итераций
    return res;
}

my_type integrate4(my_type left, my_type right, unsigned long steps, f_type func, my_type r, my_type m, my_type h)
{
    my_type res = 0;
    my_type x = left;
    my_type dx = (right-left)/steps;
    size_t counter = 0;
    for(x = left; x+dx <= right; x += dx){
        res += func(x + dx/2, r, m, h)*dx;
        ++counter;
    }
    printf("%lu; ", counter);
    dx = right-x; //Последний интервал считается отдельно
    res += func(x+dx/2, r, m, h)*dx;
    ++counter;
    printf("%f; %f; ", x+dx, right-left);
    return res;
}

int main()
{
    long n;
    my_type L = 0, R = 4*M_PI;
    my_type V, V0 = 31.81995;     /* точное решение, посчитанное аналитически */ //31.819960994220723
    my_type r = 1, m = 0.5, h = 1.1;
    /* точное решение */
    setlocale( LC_ALL, "" );
//Сравнение двух функций
//    printf("Универсальная функция;Функция для данной кривой;Абсолютная погрешность;Относительная ошибка\n");
//    long steps = 100;
//    my_type res_f, res_f2;
//    float dx = (R-L)/steps, x = L+dx/2;
//    for(n = 0; n < steps; ++n, x+=dx){
//        res_f = f(x, r, m, h);
//        res_f2 = f2(x, r, m, h);
//        printf("%.16G; %.16G; %.16G; %.16G\n", res_f, res_f2, res_f-res_f2, (res_f-res_f2)/res_f2);
//    }
    /* использовать , или . для отделения дробной части */
    //Изменение здесь
    printf("Реальное число шагов;Требуемое число шагов;Реально посчитанный интервал;Требуемый интервал;Результат программы;Ожидаемый результат;Относительная погрешность\n");
    for(n = 1; n < 101; n += 1){
        V = integrate4( L, R, n, f, r, m, h );
        /* приближенное решение для n шагов */
        printf( "%f; %f; %.6G\n", V, V0, (V-V0)/V0 );
        /* n и относительная ошибка */
    }
    return 0;
}

