#include <stdio.h>

int main(void)
{
    long long n, arr[93] = { 1, 2 };
    int i;
    scanf("%lld", &n);
    //Заполнение массива числами Фибоначчи
    for (i = 2; arr[i - 1] <= n; ++i)
        arr[i] = arr[i - 1] + arr[i - 2];

    for (i -= 2; i >= 0; --i){
        if (n >= arr[i]) {
            printf("1");
            n -= arr[i];
        }
        else printf("0");
    }
    putchar('\n');
    return 0;
}
