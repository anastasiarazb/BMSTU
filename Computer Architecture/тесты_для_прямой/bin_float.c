#include <stdio.h>
#include <stdlib.h>
#include <float.h>

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

float show_parts(const float f){
    union Num n;
    n.f = f;
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
    return f;
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
    char M[24];  //В нормальной форме
    printf("Exp Мант (без первой единицы)\n");
    char exp;
    scanf("%d %s", &exp, M);
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
    show_parts(b.f);
    return b;
}

float show_mant(const float a)
{
    union Num n;
    n.f = a;
    printf("1.");
    int i;
    for(i = 22; i >= 0; --i){
        printf("%d", 1 & (n.parts.mantissa >> i));
    }
    newline;
    return a;
}

float make_float(int sign, int exp, int mant)
{
    union Num new_float;
    new_float.parts.exp = exp + 127;
    new_float.parts.sign = sign;
    new_float.parts.mantissa = mant;
    return new_float.f;
}

int get_expt(float f)
{
    union Num n;
    n.f = f;
    return n.parts.exp;
}
