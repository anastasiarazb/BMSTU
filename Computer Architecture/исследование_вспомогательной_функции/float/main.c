#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define POW2(n) (1 << n)

#define newline putchar('\n');
#define whitespace putchar(' ');
#define SHOW_EXPT(i) {int k; for(k = 0; k < i; ++k) printf(" ");}
#define SHOW_MANT(i)     {\
    printf("1");\
    int k;\
    for(k = 22; k >= 0; --k){ \
       printf("%d", 1 & (i >> k)); \
    }\
}

#define show_binary(byte) {\
    int k; \
    for(k = 7; k >= 0; --k){ \
        printf("%d", 1 & ((byte) >> k)); \
    } \
    /*newline;*/\
 }


union Num{
    struct {
        unsigned int mantissa : 23;
        unsigned int exp : 8;
        unsigned int sign : 1;
    } parts;
    float f;
    int32_t i;
    unsigned char bytes[4];
};

void show_float(union Num);
void scan_float(int32_t *E);
union Num sum(const union Num a, const union Num b);
union Num b_generator(union Num a, union Num dx, int n);

float make_float(int sign, int exp, int mant)
{
    union Num new_float;
    new_float.parts.exp = exp + 127;
    new_float.parts.sign = sign;
    new_float.parts.mantissa = mant;
    return new_float.f;
}

void show_parts(union Num n){
    printf("f = %.16G\ns = %d, exp = %d, mant = %#-.8X\nexpt = ",
           n.f, n.parts.sign, n.parts.exp - 127, n.parts.mantissa);
    int i;
    for(i = 7; i >= 0; --i){
        printf("%d", 1 & (n.parts.exp >> i));
    }
    printf("\nmants: 1.");
    for(i = 22; i >= 0; --i){
        printf("%d", 1 & (n.parts.mantissa >> i));
    }
    newline;
    newline;
}

int main(void)
{
//    scanf("%x", &n.i);
//    scanf("%e", &n.f);
    printf("SIZEOF^ %lu\n", sizeof(union Num));
    union Num x0, b, dx, dx2, new_n;

    x0.f = 0.5;
    printf("x0:\n");
    show_parts(x0);

    dx.f = 1.75/POW2(24);
    printf("dx:\n");
    show_parts(dx);


    b.f = 0.9375/*/POW2(22)*/ /*(1.75/POW2(24))*POW2(23)*/ /*1.043081283569336E-07*/;
    printf("b:\n");
    show_parts(b);

    printf("another b:\n");
    show_parts(b_generator(x0, dx, 2));

    dx2.f = (1.0+FLT_EPSILON)/POW2(25) /*2.980232594040899E-08*/;
    printf("dx2 100...001:\n");
    show_parts(dx2);
    show_parts(sum(x0, dx2));

    printf("\nРАЗНОСТЬ %d, -----%.20G\n", (0.9375 - 0.5)/POW2(22) == 1.75/POW2(24), dx.f*(1 << 22));
//    scan_float(&new_n.i);
    new_n.f = make_float(0, -1, 0b111 << 20/**POW2(20)*//*00000000000000000000*/);
    show_parts(new_n);
    new_n.f = make_float(0, -1, 0b10101010101010101010101);
    show_parts(new_n);

    printf("1.15 = %g, 115 = %g\n", 1.15f, 1.15f*100.0f);

    return 0;
}

void show_f(float num){
    union Num n;
    n.f = num;
    printf("s = %d\n", 1 & (n.i >> 7));
    n.i <<= 1;     //Съели знаковый бит
    printf("E = %d, e = %d, ", n.bytes[3], n.bytes[3]-127);
    int exp = -(n.bytes[3]-127);
    show_binary(n.bytes[3]);
    n.i >>= 1;
    n.i &= 0x7FFFFF; //Надо как-то обнулить все, кроме 23 бит мантиссы
    printf("\nM = 1.");
    int i;
    for(i = 22; i >= 0; --i){
        printf("%d", 1 & (n.i >> i));
    }
    newline;
    SHOW_EXPT(exp);
    SHOW_MANT(n.i);
    newline;
}

void show_float(union Num n)
{
    printf("%d %#X %.16G\n", n.i, n.i, n.f);
    newline;
    int i, j;
    for(i = 3; i >= 0; --i){
        show_binary(n.bytes[i]);
        whitespace;
    }
    newline;
    for(i = 3; i >= 0; --i){
        printf("%#-8X ", *(n.bytes + i));
    }
    newline;
    for(j = 31; j >= 0; --j){
        printf("%d", 1 & (n.i >> j));
    }
    newline;
    show_f(n.f);
    newline;
    newline;
    newline;
}
void scan_float(int32_t *E)
{
    *E = 0;
    char M[24] = "10101010101010101010101";  //В нормальной форме
    printf("Exp Мант (без первой единицы)\n");
    char exp = -1;
//    scanf("%d %s", &exp, M);
    exp += 127;
    *E = (*E | (int32_t)exp) << 23;
    show_binary(*((char *)E+3));  //Показать 3-й, старший байт целого числа(LE, нумерация с 0)
    int i;
    newline;
    for(i = 0; M[i]; ++i){
        *E |= (M[i]-'0') << (22 - i); //Записать мантиссу
    }
//    printf("%G\n", *(float *)&E);
//    show_float(E);
}

union Num sum(const union Num a, const union Num b)
{
    int e1 = a.parts.exp - 127;/*(a.i >> 23) - 127*/;
    int e2 = b.parts.exp - 127/*(b.i >> 23) - 127*/;
    printf("|%d - %d| = %d\n", e1, e2, abs(e1-e2));
    SHOW_MANT(a.i);
    printf("\n%*s", abs(e1-e2), "");
    SHOW_MANT(b.i);
    newline;
    union Num x;
    x.f = a.f+b.f;
    SHOW_MANT(x.i);
    newline;
    return x;
}

union Num b_generator(union Num a, union Num dx, int delta_radix)
{
    union Num b;
    b.f = a.f + dx.f*POW2(delta_radix);
    show_parts(b);
    return b;
}

/*
1.043081283569336E-07; -24 11*/

// -24 01111111111111111111111
// -1  11100000000000000000000 0.9375
// 2^22 = 4 194 304
//Чтобы прибавлял 1 вместо 1/2 + 1*2:23   100000000000000000000001 -25 00000000000000000000001

//Чтобы погрешность была заметной, пусть переменная x имеет постоянный порядок и имеет частичное перекрытие с ненулевой частью мантиссы переменной dx, так, чтобы при суммировании происходило округление всегда в одну сторону.
//Пусть x0 = 1*2-1 и  	dx = 1,75*2-24, то есть их мантиссы перекрываются так:
//100000000000000000000000
//+                      111000000000000000000000
//и их сумма будет округлена до

//100000000000000000000010,
//То есть по факту шаг dx будет равен 2, а не 1,75.

//Возьмем в качестве правой границы число 0,9375 (1.1112 * 102-1, чтобы получить
//(1.1112 - 1) *102-1 = 1.112 * 102-10), а в качестве steps число 222.
