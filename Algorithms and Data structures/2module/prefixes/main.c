#include <stdio.h>
#include <malloc.h>

void pref_func();
void find_prefixes();

char *str;
int *pref;

int main(int argc, char **argv)
{
    if (argc > 1)
        str = argv[1];
    else {
        str = (char *)malloc(100);
        scanf("%s", str);
    }
    int len;
    for (len = 0; str[len]; ++len);
    pref = (int *)malloc(len*sizeof(int));
    pref_func();
    find_prefixes();
    free(pref);
    return 0;
}
void pref_func()
{
    int i, t;
    pref[0] = t = 0;
    for (i = 1; str[i]; ++i){
        while (t && str[t] != str[i])
            t = pref[t-1];
        if (str[t] == str[i])
            ++t;
        pref[i] = t;
    }
}
void find_prefixes()
{
    int len;
    for (len = 2; str[len-1]; ++len) {
        int substr_len = len-pref[len-1];         //Длина предполагаемой повторяющейся построки
        if(substr_len && pref[len-1] && !(len % substr_len))
            printf("%d %d\n", len, len/substr_len);
    }
}

/*Составьте программу prefixes.c, выполняющую поиск всех периодических
префиксов заданной строки S. Префикс является периодическим, если его можно
представить в виде {ddd} (k раз), где d – некоторая подстрока. Для поиска
префиксов программа должна строить префиксную функцию для строки S.
Префикс, состоящий из одинаковых подстрок d.

Программа получает строку S через аргументы командной строки, и для каждого
найденного префикса выводит в стандартный поток вывода два числа: длину
префикса n и количество повторений k подстроки d в префиксе.

Например, пусть программа вызвана как

    ./prefixes aabaabaabaab

Тогда программа должна выводить в стандартный поток вывода

    2 2

    6 2

    9 3

    12 4
*/
