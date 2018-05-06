#include <stdio.h>
#include <math.h>

int main(void){
    int A, B, i, count;
    scanf("%d", &count);
    for (A = 0; count; --count){
        scanf("%d", &i);
        A += (int)pow(2, i);
    }
    scanf("%d", &count);
    for (B = 0; count; --count){
        scanf("%d", &i);
        B += (int)pow(2, i);
    }
    A &= B;
    for (count = 0; count < 32; ++count)
        if ((A >> count) & 1) printf("%d ", count);
    return 0;
}
