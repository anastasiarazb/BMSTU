#include <stdio.h>

#define DEBUG
#define NUM_1

#define NOT_LESS(arg1, arg2) (!less(arg1, arg2))  //Больше либо равно
#define PEAK(arg) (NOT_LESS(arg, (arg)-1) \
                   && NOT_LESS(arg, (arg)+1))
#define RISE(arg) (NOT_LESS((arg)+1, arg))    //образуют ли 3 подряд идущих э-та
                                              //неубывающую последовательность
                                              //можно сравнивать только вторую пару
                                              //т.к. мы уже делали проверку на пик

int array[] = { 0, 1, 3, 4, 5, 3, 2, 0, -1, -2, -3, -4};
#define LEN 12
unsigned long peak(unsigned long, int (*)(unsigned long, unsigned long));
#ifdef NUM_1
int less(unsigned long i, unsigned long j)
{
    if (i == j) return 0;

    if (i < j) {
        if (j <= 166955682719104716UL) return 1;
        if (i >= 166955682719104716UL) return 0;
        return (166955682719104716UL-i) < (j-166955682719104716UL);
    }

    if (i <= 166955682719104716UL) return 0;
    if (j >= 166955682719104716UL) return 1;
    return (166955682719104716UL-j) < (i-166955682719104716UL);
}

unsigned long peak(unsigned long, int (*)(unsigned long, unsigned long));

int main()
{
    unsigned long i = peak(436141173616756838UL, less);
    if (i == 166955682719104716UL) {
        printf("CORRECT\n");
    } else {
        /* Если функция peak работает правильно,
        сюда никогда не будет передано
        управление! */
        printf("WRONG\n");
    }
    return 0;
}

#else
int less(unsigned long i, unsigned long j)
{
        return array[i] < array[j];
}

int main()
{
        unsigned long i = peak(LEN, less);
                printf("%lu\n", i);
        return 0;
}
#endif
unsigned long bin_search(unsigned long nel,
                         int (*less)(unsigned long i, unsigned long j));
unsigned long ternar_search(unsigned long nel,
                         int (*less)(unsigned long i, unsigned long j));

unsigned long peak(unsigned long nel,
                   int (*less)(unsigned long i, unsigned long j))
{
    if (NOT_LESS(0, 1)) return 0;
    if (NOT_LESS(nel-1, nel-2)) return nel-1;
    return ternar_search(nel, less);   //Для nel < 3 сюда не будет передано управление
}

/* Косыми чертами обозначены нижняя и верхняя границы проверяемого интервала,
 *если / - значит в этой точке последовательность неубывает, значит справа от нее
 *гарантированно расположен локальный максимум (пик, обозначен а).
 *(При монотонно возрастающей последовательности он будет равен последнему эл-ту.)
 *При делении интервала в отношении 1:2 возможны 2 варианта:
{...../..a..}
{..a..\.....}
 *значит, если в средней точке последовательность убывает, мы берем интервал
 *слева от нее, в противном случае - справа.
 */

unsigned long bin_search(unsigned long nel,
                         int (*less)(unsigned long i, unsigned long j))
{
    unsigned long low = 1, high = nel-2; //Крайние эл-ты мы уже проверили, теперь
                                         //можно сравнивать тройки элементов
    unsigned long med;
    while(low < high){
        if(PEAK(low)) return low;
        if(PEAK(high)) return high;
        med = low + (high - low)/2;
        if(PEAK(med)) return med;
        if(RISE(med)){
            low = med + 1;
            --high;
        } else {
            high = med - 1;
            ++low;
        }
    }
    return med; //Сюда не должно передаваться управление
}

/* Косыми чертами обозначены нижняя и верхняя границы третей проверяемого интервала,
 *если / - значит в этой точке последовательность неубывает, значит справа от нее
 *гарантированно расположен пик (обозначен а).
 *При делении интервала в отношении 1:3 возможны 4 варианта:
{...../...../..a..}
{...../..a..\.....}
{..a..\...../.....}
{..a..\.....\.....}
 *значит, если в нижней точке последовательность убывает, мы берем интервал
 *слева от нее, в противном случае - справа.
 */
unsigned long ternar_search(unsigned long nel,
                         int (*less)(unsigned long i, unsigned long j))
{
    unsigned long low = 1, high = nel-2;  //Границы интервала
    unsigned long left, right, interval;  //left - последняя точка левой трети интервала
                                          //right - последняя точка второй трети интервала
    while(1){
        if(PEAK(low)) return low;
        if(PEAK(high)) return high;
        interval = high - low;
        left = low + interval/3;
        right = high - interval/3;
        if(PEAK(left)) {
            printf("interval = %lu, low = %lu, high = %lu, left = %lu, right = %lu\n",
                   interval, low, high, left, right);
            return left;
        }
        if(PEAK(right)) {
            printf("interval = %lu, low = %lu, high = %lu, left = %lu, right = %lu\n",
                   interval, low, high, left, right);
            return right;
        }
        if(RISE(left) && RISE(right)){
            low = right + 1;
            --high;
        }
        if(RISE(left) && (!RISE(right))){
            low  = left + 1;
            high = right - 1;
        } else {
            ++low;
            high = left - 1;
        }
        printf("interval = %lu, low = %lu, high = %lu, left = %lu, right = %lu\n",
               interval, low, high, left, right);
//        printf("low [%lu] = %d, left [%lu] = %d, right [%lu] = %d, high [%lu] = %d\n",
//               low, array[low], left, array[left], right, array[right], high, array[high]);
    }
    return 0; //Сюда не передастся управление, выход гарантированно произойдет в цикле
}

//valgrind /home/nastasia/Qt/1module/12-peak-Debug/12-peak

/*int less(unsigned long i, unsigned long j)
{
    if (i == j) return 0;

    if (i < j) {
        if (j <= 11241155978086311589UL) return 1;
        if (i >= 11241155978086311589UL) return 0;
        return (11241155978086311589UL-i) < (j-11241155978086311589UL);
    }

    if (i <= 11241155978086311589UL) return 0;
    if (j >= 11241155978086311589UL) return 1;
    return (11241155978086311589UL-j) < (i-11241155978086311589UL);
}

unsigned long peak(unsigned long, int (*)(unsigned long, unsigned long));

int main(int argc, char **argv)
{
    unsigned long i = peak(13356955260197607378UL, less);
    if (i == 11241155978086311589UL) {
        printf("CORRECT\n");
    } else {
        printf("WRONG\n");
    }
    return 0;
}
*/
