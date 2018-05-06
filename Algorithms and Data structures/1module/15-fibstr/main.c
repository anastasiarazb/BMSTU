#include <stdio.h>
#include <malloc.h>
#include <string.h>

int fibonacci(int n);
char *fibstr(int n);

int main(void)
{
    int n;
    scanf("%d", &n);
//    fibonacci(i);
//    int i;
//    for(k = 1; k <= i; k++){
//        char *s = fibstr(k);
//        printf("%s ", s);
//        free(s);
//    }
    char *s = fibstr(n);
    puts(s);
    free(s);
    return 0;
}

char *fibstr(int n)
{
    if(!n) {
        printf("Error");
        return NULL;
    }
    int length = fibonacci(n) + 1; //+1 для '\0'
    char *s1 = (char*)calloc(length, sizeof(char));
    char *s2 = (char*)calloc(length, sizeof(char));
    char *temp;
    *s1 = 'a'; //Теперь строка s1 = "a", а s2 = "b",
    *s2 = 'b'; //т.к. второй элемент массива после calloc инициализирован в NULL
    switch(n){
    case 1:
        free(s2);
        return s1;
    case 2:
        free(s1);
        return s2;
    }
    for (; n > 2; --n) {
        strcat(s1, s2); // Сцепить две строки
        temp = s2;      // Поменять местами s2 и новую s2 перед следующей итерацией
        s2 = s1;
        s1 = temp;
    }
    free(s1);
    return s2;
}

int fibonacci(int n)
{
    int f1, f2, temp, i;
    for (f1 = f2 = 1, i = 3; i <= n; ++i){
        temp = f2;
        f2 += f1;
        f1 = temp;
    }
    return f2;
}

//valgrind ~/Qt/1module/15-fibstr-Debug/15-fibstr
