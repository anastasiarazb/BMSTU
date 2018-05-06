#include <stdio.h>
#include <math.h>
#define NUM_OF_ELEM 8
#define IS_BIT_ON(bit) ((flag >> (bit)) & 1)

//Так как количество элементов массива равно количеству бит
//в байте, мы можем использовать одну переменную типа char
//как флаг, где i-й бит будет установлен в 0, если i-е число
//из второго массива уже имеет пару в первом массиве ("вычеркнуто")
//и сравнивать только те элементы, для которых он равен 1.


int main()
{
    int i, j, a[NUM_OF_ELEM], b[NUM_OF_ELEM];
    unsigned char flag = 255;
    for (i = 0; i < NUM_OF_ELEM; ++i) scanf("%d", a + i);
    for (i = 0; i < NUM_OF_ELEM; ++i) scanf("%d", b + i);
    for (i = 0; i < NUM_OF_ELEM; ++i){
        for(j = 0; j < NUM_OF_ELEM; ++j){
//            printf("a[%d] = %d b[%d] = %d flag = %d IS_BIT_ON = %d\n", i, a[i], j, b[j], flag, IS_BIT_ON(j));
            if((a[i] == b[j]) && IS_BIT_ON(j)){     //Нашли совпавший эл., и ранее к нему пары не было
                flag -= (unsigned char)pow(2, j);   //"выключили" i-й бит,
                break;                              //переходим к след. эл-ту.
            }  //В противном случае проверяем следующий элемент из b
        }
    }
    //В конце если каждому элементу из a был найден элемент из b, flag = 0,
    //если какому-то элементу из b не нашлось пары в а, флаг будет больше 0.
    printf("%s\n", flag ? "no" : "yes");
    return 0;
}

//Перестановка элементов массива
