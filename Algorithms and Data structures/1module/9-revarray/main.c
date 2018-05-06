#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define current_type unsigned char
#define NUMELEM 7

current_type array[NUMELEM]={1, 2, 3, 4, 5, 6, 7};

void revarray (void*, unsigned long, unsigned long);
//void revarray2 (void*, unsigned long, unsigned long);
int main(void)
{
    revarray(array, NUMELEM, sizeof(current_type));
    int i;
    for(i=0; i<NUMELEM; i++){
        printf("%d\n", array [i]);
    }
    return 0;
}

/* Тип char всегда занимает 1 байт, независимо от типа платформы и количества бит в байте
 * => можно массив типа void представить как массив из nel*width элементов char и перемещать их блоками по width элементов*/
void revarray(void *array, unsigned long nel, unsigned long width){
    unsigned long i, j, k, t;
    char temp, *from, *to;
    char *array2 = (char*) array;
    for(j = width*(nel-1), i = k = 0; k < nel/2; ++k){
        for(t=0, from = array2+i, to=array2+j; t < width; ++t){ /*Переписывание байтов одной пары чисел*/
            temp = *(to+t);
            *(to+t) = *(from+t);
            *(from+t) = temp;
        }
        i+=width;
        j-=width;
    }
    array = (void*)array2;
}
//P.S. Можно ли было подключить файлы <stdlib.h> и <string.h>
//и использовать функции malloc и memcpy, чтобы не писать внутренний цикл?


void revarray2(void *array, unsigned long nel, unsigned long width){
    void *temp = malloc(width);
    unsigned long i;
    for(i = 0; i < (nel/2); i += width){
        memcpy(temp, array+i, width);
        memcpy(array+i, array + (width*(nel-1) - i), width);
        memcpy(array + (width*(nel-1) - i), temp, width);
    }
    free(temp);
}

//valgrind /home/nastasia/Qt/1module/9-revarray-Debug/9-revarray
