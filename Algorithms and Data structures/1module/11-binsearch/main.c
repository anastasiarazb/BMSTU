#include <stdio.h>
#define TYPE int
TYPE array [] = {1, 2, 30, 45, 50, 51, 55, 60};
const TYPE k = 55;

int compare (unsigned long i){
    if (array[i] == k) return 0;
    if (array[i] < k) return -1;
    return 1;
}
unsigned long binsearch(unsigned long, int (*)(unsigned long));


int main(void)
{
    printf("%lu\n", binsearch(8, compare));
    return 0;
}


//Должнен возвращаться индекс заданного элемента или nel, если его нет
//         printf("%lu %d", mid, array[mid]); putchar('\n');
unsigned long binsearch (unsigned long nel, int (*compare)(unsigned long i)){
    unsigned long low =0, high = nel-1, mid;
    while(low <= high){
        mid = (low+high)/2;
        if(compare(mid) == -1) low = mid + 1;
        else if(compare(mid) == 1) high = mid - 1;
        else return mid;
    }
    return nel;
}
