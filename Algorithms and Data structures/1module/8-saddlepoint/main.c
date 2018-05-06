#include <stdio.h>

void scan_array(int m, int n);
void find_saddle(int m, int n);

int max_of_string[10];
int min_of_column[10]; //Размер таблицы не превышает 10х10

int main(void)
{
    int m, n;
    scanf("%d%d", &m, &n);
    scan_array(m, n);
//    int i;
//    for(i = 0; i<m;++i) printf("[%d] %d\n", i, max_of_string[i]);
//    printf("\n");
//    for(i = 0; i<n;++i) printf("[%d] %d\n", i, min_of_column[i]);
    find_saddle(m ,n);
    return 0;
}

//Поиск максимумы и минимумы в строках и столбцах
//и запись их в соответствующие массивы
void scan_array(int m, int n)
{
    int i, j, num;   //i - счетчик по столбцам (до m), j - по строкам (до n)
    //Чтобы не делать лишних проверок на нулевой индекс в столбце в общем цикле,
    //отсканируем сначала первую строку, каждый элемент которой автоматически
    //становится минимальным из осканированных в своем столбце
    scanf("%d", &num);                           //Сканируем первый элемент.
    min_of_column[0] = max_of_string[0] = num;  //С элемента [0][0] начинаем делать все сравнения
    for(j = 1; j < n; ++j){
        scanf("%d", &num);
        min_of_column[j] = num;
        if(num > max_of_string[0]){
            max_of_string[0] = num;           //Находим максимум в первой строке
        }
    }
    //Считываем остальной массив
    for(i = 1; i < m; ++i){
        for(j = 0; j < n; ++j){
            scanf("%d", &num);
            if(!j || num > max_of_string[i]){
                max_of_string[i] = num;
            }
            if(num < min_of_column[j]){
                min_of_column[j] = num;
            }
        }
    }
}

void find_saddle(int m, int n)
{
    int i, j;
    for(i = 0; i < m; ++i){
        for(j = 0; j < n; ++j){
            if(max_of_string[i] == min_of_column[j]){
                printf("%d %d\n", i, j);
                return;
            }
        }
    }
    printf("none\n");
}

//valgrind /home/nastasia/Qt/1module/8-saddlepoint-Debug/8-saddlepoint
//3 4 10 1 9 7 4 3 6 5 8 11 12 2

