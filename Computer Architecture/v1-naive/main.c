#define _USE_MATH_DEFINES
#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <float.h>

struct Curve;
float dx_epitr(float fi, struct Curve *ep);
float dy_epitr(float fi, struct Curve *ep);

struct Curve
{
    float (*dx)(float fi, struct Curve *);
    float (*dy)(float fi, struct Curve *);
    float r;
    float m;
    float h;
} epitrochoid = {
    dx_epitr,
    dy_epitr,
    1,
    0.5,
    1.1};

//#define out printf
#define out(...)

float dx_epitr(float fi, struct Curve *ep)
{
    float r = ep->r;
    float m = ep->m;
    float h = ep->h;
    return r*(m+1)*(-m*sin(m*fi)+h*(m+1)*sin((m+1)*fi));
}

float dy_epitr(float fi, struct Curve *ep)
{
    float r = ep->r;
    float m = ep->m;
    float h = ep->h;
    return r*(m+1)*(m*cos(m*fi)-h*(m+1)*cos((m+1)*fi));
}

float f(float fi, void *params)
{
    struct Curve *c = params;
    float dx = c->dx(fi, c);
    float dy = c->dy(fi, c);
    return sqrt(pow(dx, 2)+pow(dy, 2));
}

float f2(float fi, void *params)
{
    return 1.5*sqrt(2.9725-1.65*cos(fi));
}

float integrate(float left, float right, unsigned long steps, float (*func)(float, void *), void *params)
{
    float res = 0;
    float x = left;
    float dx = (right-left)/steps;
    unsigned long counter = 0;        //Счетчик
    for(x = left; x < right && counter < 1000000; x += dx){
        res += func(x + dx/2, params)*dx;
        ++counter;         //Счетчик увеличивается на единицу после каждой итерации
    }
    //    printf("dx = %G\ndx == 0: %s\nleft = %G\nright = %G\nright > left: %s\nx = %G\nx == left: %s\ncounter = %lu\n",
    //           dx,
    //           dx == 0 ? "true" : "false",
    //           left, right,
    //           !(right - left == 0) ? "true" : "false",
    //           x,
    //           x == left ? "true" : "false",
    //           counter);
    return res;
}


 float integrate_a(float left, float right, unsigned long steps, float* real_right, float (*f)(float, void *), void *params)
 {
 float sum
 = 0;
 float x2, x = left;
 float dx
 = (right - left)/steps;
 for(;;)
 {
 x2 = x + dx;
 if (x2 > right) break;
 sum += 0.5 * (f(x, params) + f(x2, params)) * dx;
 x = x2;
 }
 sum += 0.5 * (f(right, params) + f(x, params)) * (right - x);

 (*real_right) = x + (right - x);

 return sum;
}


float integrate2(float left, float right, unsigned long steps, float (*func)(float, float, float, float), float r, float m, float h)
{
    float sum = 0;
    float x = left;
    float dx = (right-left)/steps;
    for(x = left; x < right; x += dx){
//        out("dx = %f, x0 = %f, f(x0) = %f, Si = %f\n", dx, x + dx/2, func(x + dx/2, r, m, h), func(x + dx/2, r, m, h)*dx);
        sum += func(x + dx/2, r, m, h);
//        if(steps == 3 || steps == 1) out("n = %d, left = %f, right = %f, dx = %f, func = %f\n", steps, left, right, dx, func((x + dx)/2, r, m, h));
    }
    return sum*dx;
}

int main()
{
    long n = 100;
    float L = 0, R = 4*M_PI;
//    Инициализация кривой начальными значениями

//    epitrochoid.r = 1;
//    epitrochoid.m = 0.5;
//    epitrochoid.h = 1.1;
//    epitrochoid.dx = dx_epitr;
//    epitrochoid.dy = dy_epitr;

    setlocale( LC_ALL, "" );
    printf("INTEGRATE: %.16G\n", integrate(L, R, n, f, &epitrochoid));
    return 0;
}

/*
 *
 Выход за правую границу:

float integrate(float left, float right, unsigned long steps, float (*func)(float, float, float, float), float r, float m, float h)
{
    float res = 0;
    float x = left;
    float dx = (right-left)/steps;
    unsigned long counter = 0;        //Счетчик
    for(x = left; x < right; x += dx){
        out("dx = %f, x0 = %f, f(x0) = %f, Si = %f\n", dx, x + dx/2, func(x + dx/2, r, m, h), func(x + dx/2, r, m, h)*dx);
        res += func(x + dx/2, r, m, h)*dx;
        ++counter;         //Счетчик увеличивается на единицу после каждой итерации
    }
    printf("%ld;%ld;", counter, counter-steps);
    printf("%f;%f;%f;", right, x, (x-right)/right);
    printf("%G;", res);
    return res;
}
int main()
{
    long n;
    float L = 0, R = 4*M_PI;
//    float V, V0 = F( R ) - F( L );
    float V, V0 = 31.81995;       //31.8199609942207229
    float r = 1, m = 0.5, h = 1.1;
    // точное решение
    setlocale( LC_ALL, "" );
    // использовать , или . для отделения дробной части
//V = integrate( L, R, 10, f, r, m, h );

//    V = integrate( L, R, 3, f, r, m, h );
    printf("Ожидаемое число шагов;Реальное число шагов;Разность;Ожидаемая правая граница;Реальная правая граница;Относительная погрешность правой границы;Результат программы;Ожидаемый результат;Абсолютная погрешность;Относительная погрешность;\n");
    for( n = 1; n < 101; n += 1){
        printf("%ld;", n);
        V = integrate( L, R, n, f, r, m, h );
        // приближенное решение для n шагов
        printf( "%.5f; %.5f; %.6G\n",V0, V-V0, (V-V0)/V0 ); //Точность типа float приблизительно 6 разрядов
        // n и относительная ошибка
    }
    return 0;
}
    */

/*
x = r(m+1)*(cos(m*fi)-h*cos((m+1)*fi))
y = r(m+1)*(sin(m*fi)-h*sin((m+1)*fi))

x' = r(m+1)*(-m*sin(m*fi)+h(m+1)sin((m+1)*fi))
y' = r(m+1)*(m*cos(m*fi)-h(m+1)cos((m+1)*fi))

или
х = 1.5*(cos(0.5*fi)-1.1*cos(1.5*fi))
1.5*(sin(0.5*fi)-1.1*sin(1.5*fi))

x = r(m+1)*cos(m*fi)-h*cos((m+1)*fi)
y = r(m+1)*sin(m*fi)-h*sin((m+1)*fi)

x' = -m*r(m+1)*sin(m*fi)+h(m+1)sin((m+1)*fi)
y' = r(m+1)*m*cos(m*fi)-h(m+1)cos((m+1)*fi)

r = 1, m = 0.5, h = 1.1 fi [0; 4*pi]

для первого варианта
х" = -0.75 sin(0.5 x)+2.475 sin(1.5 x)
у = 0.75 cos(0.5 x)-2.475 cos(1.5 x)

sum of squares -(5940*cos(x)-10701)/1600

31,82

для второго
х = -0.75 sin(0.5 x)+1.65 sin(1.5 x)
0.75 cos(0.5 x)-1.65 cos(1.5 x)

сумма их квадратов -(495*cos(x)-657)/200
интеграл равен -(495*cos(x)-657)/200 21,8201 Приблизительно

возьмем прямую в качестве кривой
x = t,
y = 5;

x' = 1;
y' = 0;
integral (sqrt(x'^2 + y'^2)) = integral (1) = x
[0, 5] = 5;
*/
