#include <stdio.h>

int array[] = {
    1703931461,
    625969021,
    -6420879
};

int compare(void *a, void *b)
{
    int va = *(int*)a;
    int vb = *(int*)b;
//    printf("va = %d, vb = %d\n %d", va, vb, va < vb? -1 : 1);
    if (va == vb) return 0;
    return va < vb ? -1 : 1;
}

int maxarray(void*, unsigned long, unsigned long,
    int (*)(void *a, void *b));

int main()
{
    printf("%d\n", maxarray(array, 3, sizeof(int), compare));
    return 0;
}
// Сравниваем байты элементов, если старший байт больше, присваиваем этот индекс элементу, если равен, проверяем след байт, если меньше - переходим к следующему элементу

//int maxarray(void *base, unsigned long nel, unsigned long width,
//             int (*compare)(void *a, void *b)){
//    unsigned long index, i;
//    int res_of_comp;
//    char *arr = (char*)base;
//    for (index = 0, i = width; i < nel*width; i += width){
//        res_of_comp = compare(arr + index, arr + i);
//        if(res_of_comp == -1) index = i;
//    }
//    return index/width;
//}

int maxarray(void *base, unsigned long nel, unsigned long width,
             int (*compare)(void *a, void *b))
{
    unsigned long index, i;
    int res_of_comp;
    for (index = 0, i = width; i < nel*width; i += width){
        res_of_comp = compare(base + index, base + i);
        if(res_of_comp < 0) index = i;
//        printf("%d %d resof-comp = %d\n", *(int*)(base+index), *(int*)(base+i), res_of_comp);
    }
    return index/width;
}
// printf("%ld %ld i= %lu index= %lu res= %d\n", *(arr + index),  *(arr + i), i, index, res_of_comp);

//valgrind /home/nastasia/Qt/10-maxarray-Debug/10-maxarray
