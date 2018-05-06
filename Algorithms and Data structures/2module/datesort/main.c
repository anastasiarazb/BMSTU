#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Date {
    int Day, Month, Year;
} date;

int key(int radix, date *ymd);
int alph_size(int radix);
void distributionsort(date D[], int radix, int len, int alphabet_size,
                      int (*key)(int radix, date *ymd));
void radixsort(date D[], int len,
               int (*key)(int radix, date *ymd));

int main(void)
{
    int n;
    scanf("%d", &n);
    date *Dates = (date *)malloc(n*sizeof(date));
    int i;
    for (i = 0; i < n; ++i){
        scanf("%d%d%d", &Dates[i].Year, &Dates[i].Month, &Dates[i].Day);
    }
    radixsort(Dates, n, key);
    for (i = 0; i < n; ++i){
        printf("%d %d %d\n", Dates[i].Year, Dates[i].Month, Dates[i].Day);
    }
    free(Dates);
    return 0;
}

int key(int radix, date *ymd)
{
    switch(radix){
    case 0:
        return ymd->Day - 1;
    case 1:
        return ymd->Month - 1;
    case 2:
        return ymd->Year - 1970;
    } //[0...60]
    return 0;
}

// 31 - max length of alphabeth;
void distributionsort(date D[], int radix, int len, int alphabet_size, int (*key)(int radix, date *ymd))
{
    int *count = (int *)calloc(alphabet_size, sizeof(int));
    int i, k;
    //Вычисляем, сколько раз встречается каждый ключ
    for (i = 0; i < len; ++i) {
        k = key(radix, D+i);
        count[k] += 1;
    }
    //Для каждого ключа вычисляем индекс, непосредственно перед которым может
    // располагаться запись, соответствующая данному ключу (сумма всех предшествующих ячеек).
    for (i = 1; i < alphabet_size; ++i) {
        count[i] += count[i - 1];
    }
    //Запись в доп.последовательность в правильном порядке
    date *array = (date *)malloc(len*sizeof(date));
    for (i = len-1; i >= 0; --i) {
        k = key(radix, D+i);
        count[k] -= 1;
        array[count[k]] = D[i];

    }
    memcpy(D, array, len*sizeof(date));
    free(count);
    free(array);
}
void radixsort(date D[], int len,
               int (*key)(int radix, date *ymd))
{
    int i;
    for (i = 0; i < 3; ++i){
        distributionsort(D, i, len, alph_size(i), key);
    }
}

int alph_size(int radix)
{
    switch(radix){
    case 0:
        return 31;
    case 1:
        return 12;
    case 2:
        return 61;
    }
    return -1; //сюда не будет передано управление
}

/*valgrind ~/Qt/2module/datesort-Debug/datesort*/
/*

Поле Day может принимать значения от 1 до 31, поле Month – от 1 до 12, а поле Year – от 1970 до 2030.

4
2008 12 6
1998 4 6
1971 8 22
2004 1 21

1971 8 22
1998 4 6
2004 1 21
2008 12 6

*/
