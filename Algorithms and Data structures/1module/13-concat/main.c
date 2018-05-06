#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define MAX_LEN 100

char *concat(char**, int);
int main(void)
{
    int n, i;
    char* str;
    scanf("%d", &n);
    char **string = (char**)calloc(n, sizeof(char *));
    for (i = 0; i < n; ++i) {
        *(string + i) = (char *) calloc(MAX_LEN, 1);
        scanf("%s", *(string + i));
    }
    puts(str = concat(string, n));
    for (i = 0; i < n; ++i) free(*(string+i));
    free(string);
    free(str);
    return 0;
}

char *concat(char **s, int n)
{
    int sumlength, i;
    for(sumlength = i = 0; i < n; ++i){
        sumlength += strlen(*(s+i));
    }
    char* str = (char*)calloc(sumlength+1, 1);// +1 для '\0'
    for(i = 0; i < n; ++i){
        strcat(str, *(s+i));
    }
    return str;
}

//valgrind /home/nastasia/Qt/1module/13-concat-Debug/13-concat
