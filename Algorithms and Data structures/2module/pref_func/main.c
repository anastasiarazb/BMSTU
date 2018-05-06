#include <stdio.h>

#define MAXLEN 100
#define SIZE 26         //Alphabeth size
#define LEN(l) for((l) = 0; str[l]; ++(l))
#define KEY(c) (str[c]-'a')

char str[MAXLEN];
int pi[MAXLEN];       //Префиксная функция
int sigma[MAXLEN];    //Суффиксная функция
int delta1[SIZE];     //Таблица стоп-символов
int delta2[MAXLEN];   //
int len;

void pi_func(void);
void sigma_func(void);
void delta1_func(void);
void delta2_func(void);

//void menu(int argc, char **argv);

int main(/*int argc, char **argv*/)
{
//    int choice = menu(argc, argv);
    int c;
    while((c=getchar()) != '\n'){
        ungetc(c, stdin);
        scanf("%s", str);
        LEN(len);
        pi_func();
        sigma_func();
        printf("\n");
        delta1_func();
        printf("\n\n");
        getchar();
    }
    return 0;
}

//Пи равно индексу элемента строки, левее которого расположен самый длинный собственный префикс строки S, равный суффиксу подстроки S[i:i]
void pi_func(void)
{
    int i, t;
    pi[0] = t = 0;
    for (i = 1; str[i]; ++i){
        while (t && str[t] != str[i])
            t = pi[t-1];
        if (str[t] == str[i])
            ++t;
        pi[i] = t;
    }
    for (i = 0; str[i]; ++i) printf(" %c ", str[i]);
    printf("     (%d)\n", i);
    for (i = 0; str[i]; ++i) printf("%2d ", pi[i]);
    printf("     (pi)\n");
}

//Сигма(i) равно индексу элемента строки, правее которого расположен самый
//длинный собственный суффикс строки S, равный префиксу подстроки S[i:len-1]
//Гарантирует, что для любого i от 0 до len-1 справедливо, что delta[i] >=
//len-i;
void sigma_func(void)
{
    int i, t;
    sigma[len-1] = t = len-1;
    for (i = len-2; i >= 0; --i){
        while (t < len-1 && str[t] != str[i])
            t = sigma[t+1];
        if (str[t] == str[i])
            --t;
        sigma[i] = t;
    }
    for (i = 0; str[i]; ++i) printf("%2d ", sigma[i]);
    printf("     (sigma)\n");
}

void delta1_func(void)
{
    int a;
    for (a = 0; a < SIZE; ++a)
        delta1[a] = len;
    for (a = 0; a < len; ++a)
        delta1[KEY(a)] = len - 1 - a;
    for (a = 0; a < SIZE; ++a)
        printf("  %c", 'a'+ a);
    putchar('\n');
    for (a = 0; a < SIZE; ++a)
        printf(" %2d", delta1[a]);
}

void delta2_func(void)
{
    int i, t;
    t = sigma[0];
    for (i = 0; i < len; ++i) {
        while (t < i)
            t = sigma[t+1];
        delta2[i] = len - i + t;
        //Не существует правдоподобного вхождения суффикса в строке, но имеется собственный суффикс, равный префиксу
    }
    for (i = 0; i < len; ++i){
        t = i;
        while (t < len - 1){
            t = sigma[t+1];
            if (str[i] != str[t])
                delta2[t] = len - 1 - i;
            //начало самого правого правдоподобного вхождения суффикса в S
        }
    }
}
