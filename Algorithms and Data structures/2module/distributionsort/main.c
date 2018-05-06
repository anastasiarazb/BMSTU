#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define MAX_LENGTH 1000000

int key(char c);
void dsort(char *str, int len, int m, int (*key)(char c));

int main(void)
{
    char str[MAX_LENGTH];
    scanf("%s", str);
    int i;
    dsort(str, strlen(str), 26, key);
    for(i = 0; str[i]; ++i) printf("%c", str[i]);
    return 0;
}
int key(char c)
{
    return (int)(c -'a');
}

void dsort(char *str, int len, int alphabet_size,   //len - количество букв в строке без учета '\0'
           int (*key)(char c))                      //alphabet_size - размер множества ключей
{
    int *count = (int *)calloc(alphabet_size, sizeof(int));
    int i, k;
    //Вычисляем, сколько раз встречается каждый ключ
    for (i = 0; i < len; ++i) {
        k = key(str[i]);
        count[k] += 1;
    }
    //Для каждого ключа вычисляем индекс, непосредственно перед которым может
    // располагаться запись, соответствующая данному ключу (сумма всех предшествующих ячеек).
    for (i = 1; i < alphabet_size; ++i) {
        count[i] += count[i - 1];
    }
    //Запись в доп.последовательность в правильном порядке
    char *array = (char *)malloc(len);
    for (i = len-1; i >= 0; --i) {
        k = key(str[i]);
        count[k] -= 1;
        array[count[k]] = str[i];

    }
    memcpy(str, array, len);
    free(count);
    free(array);
}

/*Сортировка распределением
valgrind ~/Qt/2module/distributionsort-Debug/distributionsort*/
