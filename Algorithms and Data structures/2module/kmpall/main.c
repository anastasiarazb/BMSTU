#include <stdio.h>
#include <malloc.h>
#define MAX_LEN 1000

#define DEBUG
#ifdef DEBUG
#define out printf
#define show_arr(arr, len) {int i; for(i = 0; i < len; ++i) {printf("%d ", arr[i]);} putchar('\n');}
#else
#define out
#endif

void  pref_func();
void kmpall();

char *str;
char *substr;
int len_substr;
int *pref;

int main(int argc, char **argv)
{
    if (argc == 3) {
        str = argv[1];
        substr = argv[2];
    } else {
        str = (char *)malloc(MAX_LEN);
        substr = (char *)malloc(MAX_LEN);
        scanf("%s%s", str, substr);
    }
    for (len_substr = 0; substr[len_substr]; ++len_substr);
    pref = (int *)malloc(len_substr*sizeof(int));
    pref_func();
    kmpall();
    if (argc != 3) {
        free(str);
        free(substr);
    }
    free(pref);
    return 0;
}
void pref_func()
{
    int i, t;
    pref[0] = t = 0;
    for (i = 1; substr[i]; ++i){
        while (t && substr[t] != substr[i])
            t = pref[t-1];
        if (substr[t] == substr[i])
            ++t;
        pref[i] = t;
    }
    show_arr(pref, len_substr);
}
void kmpall()
{
    int k;     //Счетчик по основной строке
    int q = 0; //Счетчик по искомой подстроке
    for (k = 0; str[k]; ++k){
        while (q > 0 && (substr[q] != str[k]))
            q = pref[q-1];
        if (substr[q] == str[k])
            ++q;
        if (q == len_substr)
            printf("%d\n", k-len_substr+1);
    }
}

/*
Составьте программу kmpall.c, осуществляющую поиск всех вхождений подстроки S в
строку T. В программе должен быть реализован алгоритм Кнута-Морриса-Пратта,
изменённый таким образом, чтобы при нахождении очередного вхождения S в T
алгоритм не завершался, а продолжал сканировать строку T.

Строки S и T должны передаваться в программу через аргументы командной строки.
Программа должна выводить в стандартный поток вывода индексы первых символов
всех вхождений S в T.
*/
