#include <stdio.h>
#include <math.h>

#define BIT(i) (1&b>>(i))

int main(void)
{
    unsigned long long a, b, m;
    scanf("%llu%llu%llu", &a, &b, &m);
    if(!m) {
        printf("ERROR: devision by zero.\n");
        return 0;
    }
    int n = log2(b);               //Длина двоичного представления числа b
    unsigned long long result = a*BIT(n);
//    printf("result = 2*a*%llu%%m = %llu\n", BIT(n), result);
    for(--n ; n >= 0; --n){
//        printf("result = (2*%llu + a*%llu%%m)%%m", result, BIT(n));
        result = (2*result%m + a*BIT(n)%m)%m;
//        printf(" = %llu\n", result);
    }
    printf("%llu\n", result);
    return 0;
}

//valgrind Qt/1module/2-mulmod-Debug/2-mulmod
