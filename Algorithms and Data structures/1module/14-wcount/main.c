#include <stdio.h>

#define MAX_LEN 100   //Максимальная длина считываемой строки

int wcount(char*);

int main(void)
{
    char str[MAX_LEN] = {'\0'};
    gets(str);
    printf("%d\n", wcount(str));
    return 0;
}
int wcount(char *s)
{
    int i, counter;
    if(!s[0]) return 0;            //Пустая строка
    counter = s[0] == ' ' ? 0 : 1; // Если первый символ не пробельный, то засчитать начало слова
    for(i = 1; s[i] != '\0'; ++i){
        if((s[i-1]== ' ') && (s[i]!= ' ')) ++counter;
        //Подсчитывает, сколько существует начал слов, т.е.
        //Сколько раз было сочетание "пробел-буква"
    }
    return counter;
}

//valgrind --track-origins=yes /home/nastasia/Qt/1module/14-wcount-Debug/14-wcount
