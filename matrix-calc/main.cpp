#include <iostream>
#include "matrix.h"
#include "matrix.cpp"

using namespace std;

void menu();
void multy();
void sum();
void AddIdent();
void AddString();
void StringSwap();
void MulString();
void Gauss();
void powMatr();
void det();
void Minor();
void reverse();
void transpose();

int main()
{
    menu();
    return 0;
}



void menu()
{
    int choose;
    while(1){
        cout << "Что будем делать?\n"
                "1. Умножение.\n"
                "2. Возведение матрицы в степень.\n"
                "3. Вычисление определителя\n"
                "4. Вычленение минора\n"
                "5. Нахождение обратной\n"
                "6. Транспонирование\n"
                "7. Метод Гаусса\n"
                "8. Выход.\n";
        cin >> choose;
        switch (choose) {
        case 1:
            multy();
            break;
        case 2:
            powMatr();
            break;
        case 3:
            det();
            break;
        case 4:
            Minor();
            break;
        case 5:
            reverse();
            break;
        case 6:
            transpose();
            break;
        case 7:
            Gauss();
            break;
        case 8:
            return; //Выход из программы
        default:
            cout << "Вы ввели что-то не то, попробуйте еще раз.\n";
            break;
        }
    };
}

//Унарные операции
#define BREAK(i) \
    if(!i){ \
      cout << "Сорян, операция отменена, едем дальше\n"; \
      return; \
    }

#define WELCOME2ARGS(name) \
    cout << name << "\nВведите размер матрицы A (m x n):\n"; \
    int m, n; \
    scanf("%d", &m); \
    BREAK(m); \
    scanf("%d", &n); \
    BREAK(n); \
    Matrix A(m, n); \
    cout << "Введите элементы:\n"; \
    A.fillMatrix(); \
    A.printMatrix("A");

#define WELCOME1ARG(name) \
    cout << name << "\nВведите порядок матрицы A:\n"; \
    int n; \
    scanf("%d", &n); \
    BREAK(n) \
    Matrix A(n, n); \
    cout << "Введите элементы:\n"; \
    A.fillMatrix(); \
    A.printMatrix("A");

void Gauss()
{
    WELCOME2ARGS("МЕТОД ГАУССА");
    A.Gauss();
    A.printMatrix("A'");
}

void StringSwap()
{
    WELCOME2ARGS("ПЕРЕСТАНОВКА СТРОК");
    cout << "Введите номера переставляемых строк (нумерация с 1):\n";
    scanf("%d %d", &n, &m);
    A.StringSwap(n-1, m-1);
    A.printMatrix("I <-> J");
}

void AddIdent()
{
    WELCOME2ARGS("ПРИПИСЫВАНИЕ ЕДИНИЧНОЙ МАТРИЦЫ СПРАВА");
    A.Add_Ident();
    A.printMatrix("A|E");
}

void AddString()
{
    WELCOME2ARGS("ДОБАВЛЕНИЕ К СТРОКЕ ДРУГОЙ СТРОКИ С КОЭФФИЦЕНТОМ");
    cout << "Введите номер строки-приемника, строки-источника (нумерация с 1) и коэффицент:\n";
    int from, to;
    float l;
    scanf("%d %d %f", &to, &from, &l);
    A.Add_String(from-1, to-1, l);
    A.printMatrix("i + lambda*j");
}

void MulString()
{
    WELCOME2ARGS("УМНОЖЕНИЕ СТРОКИ НА ЧИСЛО");
    cout << "Введите номер строки (нумерация с 1)  коэффицент:" << endl;
    float l;
    scanf("%d %f", &n, &l);
    A.MulString(n-1, l);
    A.printMatrix("lambda*i");
}

void det()
{
    WELCOME1ARG("ВЫЧИСЛЕНИЕ ОПРЕДЕЛИТЕЛЯ");
    cout << A.full_det() << endl;
}

void Minor()
{
    WELCOME2ARGS("НАХОЖДЕНИЕ МИНОРА");
    cout << "Введите координаты элемента (пусть индексация столбцов/строк начинается с 1):\n";
    scanf("%d %d", &m, &n);
    Matrix MinorMatrix;
    MinorMatrix.Minor(A, m-1, n-1);
    MinorMatrix.printMatrix("Minor\n");
}

void reverse()
{
    WELCOME1ARG("НАХОЖДЕНИЕ ОБРАТНОЙ");
    Matrix rev;
    rev.reverse(A);
}

void transpose()
{
    WELCOME2ARGS("ТРАНСПОНИРОВАНИЕ");
    Matrix B;
    B.transpose(A);
    B.printMatrix("Транспонированная");
}

void powMatr()
{
    WELCOME1ARG("ВОЗВЕДЕНИЕ В СТЕПЕНЬ");
    cout << "Введите степень, в которую будет возводится матрица:\n";
    cin >> n;
    Matrix AxA;
//    AxA.fast_powerMatrix(&A, n);
    AxA.powerMatrix(&A, n);
    AxA.printMatrix("A^n");
}


//Бинарные

void sum()
{
    cout << "СУММА МАТРИЦ\nВведите размер матриц A и B (m x n):\n";
    int m, n;
    scanf("%d %d", &m, &n);
    if(! (m && n)){
        cout << "Сорян, операция отменена, едем дальше\n"; \
       return;
    }
    Matrix A(m, n), B(m, n);
    cout << "Введите элементы матрицы А:\n";
    A.fillMatrix();
    A.printMatrix("A");
    cout << "Введите элементы матрицы B:\n";
    B.fillMatrix();
    B.printMatrix("B");

    A += B;
    B.printMatrix("A+B");
}

void multy()
{
    Matrix A, B, AxB;

    cout << "УМНОЖЕНИЕ МАТРИЦ.\nВведите размер матрицы A (m x n):\n";
    int m, n;
    cin >> m;
    BREAK(m);
    cin >> n;
    BREAK(n);
//    while (!(m && n)){ //Если какой-то из элементов равен 0
//        cout << "Матрица не может иметь 0 строк или столбцов. Попробуйте снова.\n";
//        cin >> m >> n;
//    }

//    while(cin.get() != '\n'); //Что это вообще было? Оно не работает при вбивании лишних аргументов в размерность
    A.initMatrix(m, n);
    cout << "Введите элементы:\n";
    A.fillMatrix();
    A.printMatrix("A");

//    cout << "Введите размер матрицы B (n x q):\n";
//    int x, q;
//    cin >> x >> q;
//    while (!q || x != n){
//        cout << "Или вы ввели q = 0, или матрицы A и B несогласованы. Они должны иметь размеры (m x n) и (n x q)" << endl;
//        cout << "Введите размер матрицы B (n x q) (подсказка: n = "<< n <<"):\n";
//        cin >> x >> q;
//    }
//    B.initMatrix(x, q);

    cout << "Введите второй параметр размерности матрицы B (" << n << " x q):\n";
    int q;
    cin >> q;
//    while (!q || x != n){
//        cout << "Или вы ввели q = 0, или матрицы A и B несогласованы. Они должны иметь размеры (m x n) и (n x q)" << endl;
//        cout << "Введите размер матрицы B (n x q) (подсказка: n = "<< n <<"):\n";
//        cin >> x >> q;
//    }
    B.initMatrix(n, q);
    cout << "Введите элементы:\n";
    B.fillMatrix();
    B.printMatrix("B");


    AxB.initMatrix(m, q);
    AxB.full_mul_matrix(&A, &B);
    AxB.printMatrix("AxB");
}

/* valgrind --leak-check=full --leak-resolution=med ~/Qt/Matrix-with-classes-Debug/Matrix-with-classes
 * 2 2 0 1 1 0 2 2 1 1 0 1
 * valgrind --leak-check=full --show-reachable=yes --track-origins=yes ~/Qt/Matrix-with-classes-Debug/Matrix-with-classes
 */
