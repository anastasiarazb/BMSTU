#include <stdio.h>

int main(void)
{
    long long n, x;
    scanf("%lld%lld", &n, &x);
    long long a;                 //Текущий коэффицент
    long long i;                 //Счетчик
    long long result, diff;      //Значение полинома и его производной
    scanf("%lld", &result);  //Пусть стартовое значение равно первому индексу (результат для 0-й степени)
    diff = result*n;       //  а его производная - произведению первого индекса и степени при старшем х.
    for(i = 1; i <= n; ++i) {
        scanf("%lld", &a);
        result = result*x+a;
        if(i < n)
            diff = diff*x+a*(n-i);
    }
    printf("\n%lld %lld\n", result, diff);
    return 0;
}

// valgrind ~/Qt/1module/1-polynom-Debug/1-polynom
