#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100
#define SIZE 26         //Alphabeth size
#define LEN(l, str) for((l) = 0; str[l]; ++(l))
#define KEY(c, str) (str[c]-'a')
#define MAX(a, b) ((a) > (b) ? (a) : (b))

char *T;
char *S;           //Префиксная функция
int *sigma;        //Суффиксная функция
int delta1[SIZE];  //Таблица стоп-символов
int *delta2;
int lenT;
int lenS;

void sigma_func(void);
void delta1_func(void);
void delta2_func(void);
void bmall(void);

int main(int argc, char **argv)
{
    if (argc == 3) {
        S = argv[1];
        T = argv[2];
    } else {
        T = (char *)malloc(MAXLEN);
        S = (char *)malloc(MAXLEN);
        scanf("%s%s", S, T);
    }
    LEN(lenT, T);
    LEN(lenS, S);
    sigma = (int *)malloc(lenS*sizeof(int));
    delta2 = (int *)malloc(lenS*sizeof(int));
    bmall();
    free(sigma);
    free(delta2);
    if (argc != 3){
        free(T);
        free(S);
    }
    return 0;
}

//Сигма(i) равно индексу элемента строки, правее которого расположен самый
//длинный собственный суффикс строки S, равный префиксу подстроки S[i:len-1]
//Гарантирует, что для любого i от 0 до len-1 справедливо, что delta[i] >=
//len-i;

void sigma_func(void)
{
    int i, t;
    sigma[lenS-1] = t = lenS-1;
    for (i = lenS-2; i >= 0; --i){
        while (t < lenS-1 && S[t] != S[i])
            t = sigma[t+1];
        if (S[t] == S[i])
            --t;
        sigma[i] = t;
    }
}

void delta1_func(void)
{
    int a;
    for (a = 0; a < SIZE; ++a)
        delta1[a] = lenS;
    for (a = 0; a < lenS; ++a)
        delta1[KEY(a, S)] = lenS - 1 - a;
}

void delta2_func(void)
{
    sigma_func();
    int i, t;
    t = sigma[0];
    for (i = 0; i < lenS; ++i) {
        while (t < i)
            t = sigma[t+1];
        delta2[i] = lenS - i + t;
        //Не существует правдоподобного вхождения суффикса в строке, но имеется собственный суффикс, равный префиксу
    }
    for (i = 0; i < lenS; ++i){
        t = i;
        while (t < lenS - 1) {
            t = sigma[t+1];
            if (S[i] != S[t])
                delta2[t] = lenS - 1 - i;
            //начало самого правого правдоподобного вхождения суффикса в S
        }
    }
}

void bmall(void)
{
    delta1_func();
    delta2_func();
    int k, i;
    for(k = lenS - 1 ; k < lenT; k += MAX(delta1[KEY(k, T)], delta2[i])) {
        for(i = lenS - 1; T[k] == S[i]; --i, --k)
            if(i == 0) {
                printf("%d ", k);
                break;
            }
    }
}

//valgrind /home/nastasia/Qt/2module/bmall-Debug/bmall aa caacaaa
//aa caacaaa 1 4 5
//abbb abbbbcbcbabbb
//cacba ccacbabbccacbabcaccacacbacacbabccacacbaabccbcacbaaacacbacccacbaacaa
