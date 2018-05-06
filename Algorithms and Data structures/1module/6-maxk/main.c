#include <stdio.h>

#define ERROR {printf("Error: k > n.\n"); return 0;}

int main(void)
{
    int n, k, i, j, current, sum, max;
    scanf("%d", &n);
    int a[n];
    for(i = 0; i < n; ++i){
        scanf("%d", a+i);
    }
    scanf("%d", &k);
    if(k > n) ERROR;
    for(max = sum = i = j = 0; i < n; ++i){
        current = a[i];
        sum += current;
        if (i < k) {
            max = sum;
            continue; //Пока только аккумулируем сумму части массива [0; k-1]
        }
        sum -= a[j++]; // Прибавляем новый элемент и вычитаем эл-т, оставшийся позади интервала [j+1;i]
        if(sum > max){
            max = sum;
        }
    }
    printf("%d", max);
    return 0;
}

/*
int main(void)
{
    int n, k, i, count, sum, max = 0;
    scanf("%d", &n);
    if (n > 1000000) ERROR;
    int a[n];
    for(i = 0; i < n; i++) scanf("%d", a + i);
    scanf("%d", &k);
    if (k > n) ERROR;
    for (i = 0; i <= n - k; i+= 1 - k){
        for (sum = 0, count = k; count; count--, i++) sum += a[i];
        if (sum > max) max = sum;
    }
    printf("%d", max);
    return 0;
}
*/
