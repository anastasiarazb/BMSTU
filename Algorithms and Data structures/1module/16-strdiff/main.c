#include <stdio.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define GET_BIT(str, bit) ((str[(bit)/8] >> ((bit) % 8)) & 1)

int strdiff(char *a, char *b);
void show_string(char *str);

int main()
{
        char s1[1000], s2[1000];
        gets(s1);
        gets(s2);
        show_string(s1);
        show_string(s2);
        printf("%d\n", strdiff(s1, s2));

        return 0;
}

int strdiff(char *a, char *b)
{
    int i;
    int strlen_a = strlen(a) + 1;
    int strlen_b = strlen(b) + 1;
    for(i = 0; i < 8 * MIN(strlen_a, strlen_b); ++i){
        printf("GET_BIT(%d) = %d GET_BIT(%d) = %d\n", i, GET_BIT(a, i), i, GET_BIT(b, i));
        if (GET_BIT(a, i) != GET_BIT(b, i)){
            return i;
        }
    }
    return -1;
}

void show_string(char *str)
{
    int i;
    for(i = 0; i <= 8 * (int)strlen(str); ++i)
    {
        if(!(i%8)) printf(" ");
        printf("%d", GET_BIT(str, i));
    }
    printf("\n");
}
/*
void show_string(char *str)
{
    FILE *file = fopen("/home/nastasia/Qt/1module/16-strdiff/codes.txt", "a");
    int i;
    for(i = 0; i < 8 * ((int)strlen(str)+1); ++i)
    {
        if(!(i%8)) fprintf(file, " ");
        fprintf(file,"%d", GET_BIT(str, i));
    }
    fprintf(file, "\n");
    fclose(file);
}
*/

//valgrind /home/nastasia/Qt/1module/16-strdiff-Debug/16-strdiff
/*
lmikzcakdijibzzjceejcdjbzlkzlliie
lmikzcakdijibzzjceejcdjbzlkzlliiedjbccbickjiielmeckdmzembe
266

deljai
deljaize
49
*/
