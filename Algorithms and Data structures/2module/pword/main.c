#include <stdio.h>
#include <malloc.h>
#include <string.h>

void pref_func();
int pword();

char *str1;  //Строка-"донор"
char *str2;  //Проверяется на вхождение префиксов первой строки
int *pi;
int len1;

int main(int argc, char **argv)
{
    if (argc > 1){
        str1 = argv[1];
        str2 = argv[2];
    } else {
        str1 = (char *)malloc(100);
        str2 = (char *)malloc(100);
        scanf("%s%s", str1, str2);
    }
    for(len1 = 0; str1[len1]; ++len1);      //Длина первой строки
    pi = (int *)malloc(len1*sizeof(int));
    pref_func();
    if (pword())
        printf("yes\n");
    else
        printf("no\n");
    free(pi);
    if (argc == 1){
        free(str1);
        free(str2);
    }
    return 0;
}
void pref_func()
{
    int i, t;
    pi[0] = t = 0;
    for (i = 1; str1[i]; ++i){
        while (t && str1[t] != str1[i]){
            t = pi[t-1];
        }
        if (str1[t] == str1[i]){
            ++t;
        }
        pi[i] = t;
    }
}

int pword()
{
    int k;     //Счетчик по 1-й строке
    int q;     //Счетчик по 2-й подстроке
    for (q = k = 0; str2[k]; ++k){
        while (q && (str1[q] != str2[k])){
            q = pi[q-1];
        }
        if (str1[q] == str2[k]){
            ++q;
        }
        if (!q) return 0;
    }
    return 1;
}
