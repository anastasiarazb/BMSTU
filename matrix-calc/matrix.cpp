#include "matrix.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <stdio.h>

//#define NOT_INITIALISED(&M) (!(M).rows||!(M).columns)
using namespace std;

#ifndef MATRIX_MALLOC
//Выделение памяти под двумерный массив вида type arr[M][N];
//type **arr уже должен быть объявлен
//Работает и для calloc(..., 1);
#define MATRIX_MALLOC(arr, M, N, type) { \
    /* (M)*sizeof(int *) - Длина области под массив указателей*/ \
    /* (N)*sizeof(int) - Длина одной строки*/ \
    arr = (type **)malloc((M)*sizeof(type *) + (M)*(N)*sizeof(type)); \
    arr[0] = (type *)(arr + M); /*Переходим на ту область, где начинается массив строк*/\
    int i; \
    for(i = 1; i < (M); ++i){ \
       arr[i] = arr[i-1]+N;     /*Отступаем от начала предыдущей строки на ее длину*/\
    } \
 }
#endif

#ifndef SHOW_RECURSION_DEPTH
#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
#endif

/*************************************************************
 * Операции выделения-освобождения памяти,
 * инициализации матриц, ввода-вывода
 *************************************************************/

//конструктор

//Matrix::Matrix(){
//    rows = 0;
//    columns = 0;
//    initialised = false;
//}
Matrix::Matrix(int m = 0, int n = 0)
{
    rows = m;
    columns = n;
    if(!rows || !columns){   //Если значения не заданы явно, то инициализируем их в 0
        initialised = false; //<-- Это значит, что память не выделена
//        cout << "Конструктор запущен, память НЕ выделена\n";
        return;
    }

    MATRIX_MALLOC(matrix, m, n, float);

//    register int i;
//    matrix = new float *[rows]; //Задаем массив указателей на строки
//    for (i = 0; i < rows; i++){
//        matrix[i] = new float[columns]; //Выделение памяти под строки
//    }
//    cout << "Конструктор запущен, память ВЫДЕЛЕНА\n";
    initialised = true;

}

//Выделение памяти под таблицу и инициализация значений rows и columns, выставление initialised в true
void Matrix::initMatrix(int m, int n)
{
    rows = m;
    columns = n;
    MATRIX_MALLOC(matrix, m, n, float);
    initialised = true;
}

//Считывание размерности и элементов матрицы
void Matrix::scanMatrix()
{
    cout << "Введите размерность матрицы:\n";
    int m, n;
    cin >> m;
    cin >> n;
    while (!(m && n)){ //Если какой-то из аргументов равен 0
        cout << "Impossible number, try again and enter an integer.\n";
        cin >> m >> n;
    }
    initMatrix(m, n); //Инициализируем матрицу
    int i, j;
    cout << "Введите элементы матрицы:\n";
    for (i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){ //Считывание строк
            cin >> matrix[i][j];
        }
    }

}

void Matrix::fillMatrix()
{
    if(initialised){
        int i, j;
        for (i = 0; i < rows; i++){
            for (j = 0; j < columns; j++){ //Считывание строк
                cin >> matrix[i][j];
            }
        }
    }
}

//Освобождение памяти, установка initialised в false;
Matrix::~Matrix()
{
    if(initialised){
        free(matrix);
    } else {
        return;
    }
    initialised = false;
}

//Следующие три функции нужны для вывода матрицы
int lenOfNum(int num) //Вычисление количества символов числа, считая знак
{
    register int length = num < 0 ? 1 : 0; //Если есть знак минус, то приплюсовать и его, иначе начать с 0
    for(; num; length++, num /= 10);
    return length;
}

int Matrix::howLongIsTheLongestNumber() const //Кол-во символов в самом длинном числе
{
    int maxLength = 0;
    register int i, j;
    //Находим самое большое число
    for (i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){
            if(lenOfNum(matrix[i][j]) > maxLength)
                maxLength = lenOfNum(matrix[i][j]);
        }
    }
    return maxLength;

}

//Вывод матрицы на экран
void Matrix::printMatrix() const
{
    int width = howLongIsTheLongestNumber();
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){ //Считывание строк
            cout << right << setw(width) << matrix[i][j] << ' ';
//            cout << matrix[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void Matrix::printMatrix(string name) const
{
    cout << "Матрица " << name <<
            " (" << rows << 'x' << columns << "):"<<endl;
    printMatrix();
}

/*************************************************************
 * Генерация матриц
 *************************************************************/

//Создание матрицы, равной данной.
void Matrix::generateCopyMatrix(Matrix *source)
{
    initMatrix(source->rows, source->columns);
    register int i, j;
    for (i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){
            matrix[i][j] = source->matrix[i][j];
        }
    }

}

//Копирование элементов в матрицу, память под которую уже выделена
int Matrix::copyElements(Matrix *to, Matrix *from)
{
    if(!initialised) return 1;
    if((to->rows != from->rows) || (to->columns != from->columns))
        return 2; //Вернуть код ошибки, если матрицы разного типа
    register int i, j;
    for (i = 0; i < from->rows; i++){
        for (j = 0; j < from->columns; j++){
            to->matrix[i][j] = from->matrix[i][j];
        }
    }
    return 0; //Операция выполнена успешно
}

void Matrix::operator=(const Matrix &source)
{
    if(!source.initialised){
        cout << "Эй-эй! Я еще не инициализирован, куда ты меня приравниваешь?!\n";
        return;
    }
    if(!initialised){
        initMatrix(source.rows, source.columns); //Если исходная матрица еще не инициализирована, выделить под нее память
    } else {
        this->~Matrix();
        initMatrix(source.rows, source.columns);
    } //Если матрица уже была инициализирована, то освободить ее и инициализировать как матрицу типа source
    register int i, j;
    for (i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){
            matrix[i][j] = source.matrix[i][j];
        }
    }
}

//Генерация единичной матрицы
Matrix &Matrix::generateIdentityMatrix(int rank) {
    initMatrix(rank, rank);
    register int i, j;
    for(i = 0; i < rank; i++){
        for(j = 0; j < rank; j++)
            matrix[i][j] = (i == j)? 1 : 0;
    }
    return *this;
}


/****************************************************************
 * Операциии над матрицами
 ****************************************************************/

//Произведение матриц (с проверкой на выделение памяти)
int Matrix::multiplMatrix(Matrix const *first, Matrix const *second)
{
    if(first->columns != second->rows) {
        cout << "Матрицы несогласованы. Введите матрицы размерностью (m x n) и (n x q).\nКоличество столбцов первой матрицы должно совпадать с количеством строк второй.\n";
        return 1;
    }
    if(!initialised) initMatrix(first->rows, second->columns);
    register int i, j, k;
    for(i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){ //Каждый элемент таблицы
            matrix[i][j] = 0;
            for(k = 0; k < first->columns; k++) //Суммирование элементов
                matrix[i][j] += first->matrix[i][k]
                               *second->matrix[k][j];
        }
    }
    return 0; //Операция выполнена успешно
}

int Matrix::full_mul_matrix(Matrix const *first, Matrix const *second)
{
    if(first->columns != second->rows) {
        cout << "Матрицы несогласованы. Введите матрицы размерностью (m x n) и (n x q).\nКоличество столбцов первой матрицы должно совпадать с количеством строк второй.\n";
        return 1;
    }
    if(!initialised) initMatrix(first->rows, second->columns);
    register int i, j, k;
    for(i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){ //Каждый элемент таблицы
            matrix[i][j] = 0;
            for(k = 0; k < first->columns; k++){ //Суммирование элементов
                matrix[i][j] += first->matrix[i][k]
                               * second->matrix[k][j];
//                printf("%e * %e + ", first->matrix[i][k], second->matrix[k][j]);
                cout << first->matrix[i][k] << " * " << second->matrix[k][j] << " + ";
            }
            cout << "\b\b | ";
        }
        cout << endl;
    }
    cout << endl;
    return 0; //Операция выполнена успешно
}

// Возведение матрицы в квадрат с проверкой на выделенную память
int Matrix::squareMatrix(Matrix *source)
{
    if(source->rows != source->columns){
        cout << "Матрица не квадратная, невозможно возвести ее в степень\n";
        return 1;
    }
    if (!initialised) initMatrix(source->rows, source->columns);
    register int i, j, k;
    for(i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){ //Каждый элемент таблицы
            matrix[i][j] = 0;
            for(k = 0; k < source->columns; k++) //Суммирование элементов
                matrix[i][j] += source->matrix[i][k]
                               *source->matrix[k][j];
        }
    }
    return 0; //Операция выполнена успешно
}

/*Возведение матрицы в степень. Пользуемся тем, что A^num = (A^(num/2))^2 для четного num и
A^num = А*(A^(num-1)) для нечетного. Для натуральных чисел выглядит как:

long fastExpt1(int x, int y)
{
    if (y == 0) return 1;
    //if (y < 0) return 1/fastExpt1(x, -y);
    int i = y/2, result = 1;
    for (; i > 0; result *= x*x, i--);
    return y%2 ? result*x : result;
}

Но сначала простая реализация
*/

int Matrix::fast_powerMatrix(Matrix *source, int num)
{
    if(source->rows != source->columns){
        cout << "Матрица не квадратная, невозможно возвести ее в степень\n";
        return 1;
    }
    if(num == 0) { //Возведение матрицы в нулевую степень дает единичную матрицу того-же порядка
        generateIdentityMatrix(source->rows);
        return 0;
    }
    int i = num/2;
    generateCopyMatrix(source); //Делаем наш объект копией изначальной матрицы (с выделением памяти)
    Matrix *temp = new Matrix;
    Matrix *temp2 = new Matrix;
    temp->initMatrix(rows, rows);
    temp2->initMatrix(rows, rows);
    for (--i; i > 0; i--){
        temp2->squareMatrix(source);
        temp->multiplMatrix(this, temp2); //Умножаем результат сразу на квадрат матрицы
        copyElements(this, temp); //Копируем в наш объект получившийся результат
    }
    if(!(num % 2)){ //Если степень нечетная, еще раз умножаем получившийся результат на начальную матрицу
        temp->multiplMatrix(this, source);
        copyElements(this, temp);
    }
    temp->~Matrix();
    temp2->~Matrix();
    delete temp;
    delete temp2;
    return 0;

}
/*
int simpleExpt(int x, int y){
    int result;
    if (y == 0) return 1;
    for (result = 1; y; y--, result *= x);
    return result;
}
*/


//Простое возведение матрицы source в степень num

int Matrix::powerMatrix(Matrix *source, int num)
{
    if(source->rows != source->columns){
        cout << "Матрица не квадратная, невозможно возвести ее в степень\n";
        return 1;
    }
    if(num == 0) { //Возведение матрицы в нулевую степень дает единичную матрицу того-же порядка
        generateIdentityMatrix(source->rows);
        return 0;
    }
    generateCopyMatrix(source); //Делаем наш объект копией изначальной матрицы (с выделением памяти)
    Matrix *temp = new Matrix;

    for(--num; num; num--){
        temp->multiplMatrix(this, source);
        copyElements(this, temp); //Копируем в наш объект получившийся результат
    }
    temp->~Matrix();
    delete temp;
    return 0;
}

Matrix& Matrix::operator*=(const Matrix &A)
{
//    if(this->columns != A.rows || !initialised || !A.initialised) {
//        cout << "иди нафиг, что ты мне тут подсовываешь\n";
//        return *this; //Потом придумаю, что делать
//    }
    if(columns != A.rows) {
        printf("УМНОЖЕНИЕ: A.columns = %d, B.rows = %d\n;", columns, A.rows);
        cout << "иди нафиг, что ты мне тут подсовываешь\n";
        return *this; //Потом придумаю, что делать
    }
    if(!initialised){
        cout << "Левый неинициализирован\n иди нафиг, что ты мне тут подсовываешь\n";
        return *this; //Потом придумаю, что делать
    }
    if(!A.initialised){
        cout << "Правый неинициализирован\n иди нафиг, что ты мне тут подсовываешь\n";
        return *this; //Потом придумаю, что делать
    }
    Matrix temp(this->rows, A.columns); //Создаем временную переменную и инициализируем ее соответствующими значениями типа
    register int i, j, k;
    for(i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){ //Каждый элемент таблицы
            temp.matrix[i][j] = 0;
            for(k = 0; k < columns; k++) //Суммирование элементов
                temp.matrix[i][j] += matrix[i][k]
                               *A.matrix[k][j];
        }
    }
    *this = temp;
//    temp.~Matrix();
    return *this;
}

Matrix& Matrix::operator+=(const Matrix &A)
{
    if(columns != A.columns || rows != A.rows || !initialised || !A.initialised) {
        cout << "иди нафиг, что ты мне тут подсовываешь\n";
        return *this; //Потом придумаю, что делать
    }
    register int i, j;
    for(i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){ //Каждый элемент таблицы
            matrix[i][j] += A.matrix[i][j]; //Суммирование элементов
        }
    }
    return *this;
}
/*
inline complex& complex::operator+=(complex a)
      {
      re += a.re;
      im += a.im;
      return *this;
      }

matrix& matrix::operator*=(const matrix& a)
{

return *this;
}

matrix operator*(const matrix& a, const matrix& b)
{
matrix prod = a;
prod *= b;
return prod;
}
*/

void Matrix::transpose(const Matrix &M)
{
    if(initialised) this->~Matrix(); //Если массив уже использовался - очистить его
    initMatrix(M.columns, M.rows);// Меняем местами строки и столбцы

    int i, j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            matrix[i][j] = M.matrix[j][i];

} //Надо сделать не перевыделение памяти, а выделение одним массивом и просто перестановку указателей выполнить, а еще сделать копию, чтобы можно было транспонировать матрицу саму в себя
//Matrix Matrix::transpose2()
//{

//}

#define cort(a, b, c) matrix[0][a-1] * matrix[1][b-1] * matrix[2][c-1]
#define ord(i, j) (((i) + (j)) & 1 ? -1 : 1)

float Matrix::full_det() const
{
    if(rows != columns){
        cout << "DET: Матрица не квадратная" << endl;
        return 0;
    }
    if(!rows){
        cout << "DET: Матрица пуста\n";
        return 0;
    }
    if(rows == 1) return matrix[0][0];
    float determ;
    if(rows == 2){
        determ = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        cout << matrix[0][0] << " * " << matrix[1][1] << " + " << matrix[0][1] << " * " << matrix[1][0]
                << " = " << determ << endl;
//        printf("%f * %f - %f * %f = %f\n", matrix[0][0], matrix[1][1], matrix[0][1], matrix[1][0],
//                matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
        return determ;
    }
    if(rows == 3){
        determ = cort(1, 2, 3) + cort(3, 1, 2) + cort(2, 3, 1) - cort(3, 2, 1) - cort(2, 1, 3) - cort(1, 3, 2);
        cout << cort(1, 2, 3) << " + " << cort(3, 1, 2) << " + " << cort(2, 3, 1) <<
                " - " << cort(3, 2, 1) << " - " << cort(2, 1, 3) << " - " << cort(1, 3, 2) <<
                " = " << determ << endl;
//        printf("%f + %f + %f - %f - %f - %f = %f\n",
//               cort(1, 2, 3), cort(3, 1, 2), cort(2, 3, 1), cort(3, 2, 1), cort(2, 1, 3), cort(1, 3, 2), determ);
        return determ;
    }

    cout << "DET: Размерность больше 3х3" << endl;
    float res = 0.0;
    Matrix MinorM;
    for(int i = 0; i < columns; ++i){
        MinorM.Minor(*this, 0, i);
        printf("(%d) * %f * \n", ord(0, i), matrix[0][i]);
        MinorM.printMatrix();
        res += matrix[0][i]*ord(0, i) * MinorM.full_det();
        printf("RES = %f\n", res);
        MinorM.~Matrix();
    }
    return res;
}

float Matrix::det() const
{
    if(rows != columns){
        cout << "DET: Матрица не квадратная" << endl;
        return 0;
    }
    if(rows == 2){
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    if(rows == 3){
        return cort(1, 2, 3) + cort(3, 1, 2) + cort(2, 3, 1) - cort(3, 2, 1) - cort(2, 1, 3) - cort(1, 3, 2);
    }
    if(rows == 1) return matrix[0][0];

    float res = 0.0;
    Matrix MinorM;
    for(int i = 0; i < columns; ++i){
        MinorM.Minor(*this, 0, i);
        res += matrix[0][i]*ord(0, i) * MinorM.det();
        MinorM.~Matrix();
    }
    return res;
}

Matrix& Matrix::Minor(const Matrix &Source, int i, int j)
{
    initMatrix(Source.rows - 1, Source.columns - 1);
    int k, l;
    for(k = 0; k < i; k++){
        for (l = 0; l < j; l++){ //Левый верхний квадрант
            matrix[k][l] = Source.matrix[k][l];
        }
        for (/*l = j*/; l < columns; ++l ){ //Правый верхний
            matrix[k][l] = Source.matrix[k][l+1];
        }
    }

    for (/*k = i*/; k < rows; ++k ){
        for (l = 0; l < j; l++){ //Левый нижний квадрант
            matrix[k][l] = Source.matrix[k+1][l];
        }
        for (/*l = j*/; l < columns; ++l ){ //Правый нижний
            matrix[k][l] = Source.matrix[k+1][l+1];
        }
    }
    return *this;
}

Matrix& Matrix::reverse(Matrix const &Source) //Обратная матрица методом транспонированной матрицы алгебраических дополнений
{
    float determ = Source.det();
    if(determ == 0.0){
        printf("Определитель нулевой: невозможно взять обратную матрицу\n");
        return *this;
    }
    printf("det(M) = %f\n", determ);
    if(initialised) this->~Matrix(); //Если массив уже использовался - очистить его
    initMatrix(Source.columns, Source.rows);
    Matrix A, MinorA, AT;
    A.initMatrix(rows, rows);
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            MinorA.Minor(Source, i, j);
            A.matrix[i][j] = ord(i, j) * MinorA.det();
            printf("[%d][%d] = (%d) * %f\n", i, j, ord(i, j), MinorA.det());
            MinorA.printMatrix();
            MinorA.~Matrix();
        }
    }
    A.printMatrix("Aлгебраические дополнения");
    AT.transpose(A);
    AT.printMatrix("Алгебраические дополнения: транспонированная");

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            matrix[i][j] = AT.matrix[i][j] / determ;
        }
    }
    printMatrix("Итоговая");
    return *this;
}

void Matrix::Add_Ident()
{
    Matrix Temp;
    Temp.generateCopyMatrix(this);
    int m = rows, n = columns;
    this->~Matrix();
    initMatrix(m, n+m);
    int i, j;
    for(j = 0; j < n; ++j){
        for(i = 0; i < rows; ++i){
            matrix[i][j] = Temp.matrix[i][j]; //Заполнение левой части идентично старой матрице
        }
    }
    for(; j < columns; ++j){
        for(i = 0; i < rows; ++i){
            matrix[i][j] = (i == (j - n)) ? 1 : 0;
        }
    }
//    printMatrix();
}

void Matrix::Add_String(int from, int to, float lambda)
{
    if(from >= rows || to >= rows){
        cout << "Пардон, таких строк нету, есть только с 1 по" << rows+1 << endl;
        return;
    }
    for(int i = 0; i < columns; ++i){
        matrix[to][i] += lambda * matrix[from][i];
    }
}

//void Matrix::Gauss()  //Работает с точностью до перестановки строк
//{
//    int i, j, k;
//    for(i = 0; i < rows; ++i){
//        for(j = 0; j < columns && matrix[i][j] == 0; ++j); //Ищем ненулевой элемент в строке
//        for(k = i+1; k < rows; ++k){
//            printf("%d к %d, l = %f\n", i, k, -matrix[k][j]/matrix[i][j]);
//            Add_String(i, k, -matrix[k][j]/matrix[i][j]);
//            printMatrix();
//        }
//    }
//}

void Matrix::Gauss()
{
    int i, j, k;
    for(i = 0; i < rows; ++i){
        for(j = 0; j < columns && matrix[i][j] == 0; ++j); //Ищем ненулевой элемент в строке
        for(k = i+1; k < rows; ++k){
            printf("%d к %d, l = %f\n", i, k, -matrix[k][j]/matrix[i][j]);
            Add_String(i, k, -matrix[k][j]/matrix[i][j]);
            printMatrix();
        }
    }
}

void Matrix::StringSwap(int i, int j)
{
    if(i >= rows || j >= rows){
        cout << "Пардон, таких строк нету, есть только с 1 по " << rows+1 << endl;
        return;
    }
    float temp;
    for(int k = 0; k < columns; ++k){
        temp = matrix[i][k];
        matrix[i][k] = matrix[j][k];
        matrix[j][k] = temp;
    }
}

void Matrix::MulString(int i, float lambda)
{
    for(int j = 0; j < columns; ++j){
        matrix[i][j] *= lambda;
    }
}
