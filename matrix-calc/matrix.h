#ifndef MATRIX_H
#define MATRIX_H
//вырезать все функции, в которых дополнительно параллельно выделяется память во избежание ошибок, переписать scan (или просто удалить)
//Переписать генерейты с проверкой на выделение и приписать во фри коут с предупреждением
class Matrix
{
public:
    float **matrix;
    int rows;    //количество строк
    int columns; //количество столбцов
    bool initialised;
//public:
   /*---------------------------------------------------------------------------------*
    *   Операции выделения-освобождения памяти, инициализации матриц, ввода-вывода
    *--------------------------------------------------------------------------------*/
    //Matrix();
    Matrix(int m, int n);

    void initMatrix(int m, int n); //Конструктор. Инициализация rows и columns значениями m n соответственно, initialised устанавливает в true
                                   //Выделение памяти под матрицу (m x n)
    void scanMatrix();   //Считывание матрицы из потока ввода
    void fillMatrix(); //Заполнение массива
    void printMatrix(std::string name) const; //Вывод матрицы
    void printMatrix() const;
    int howLongIsTheLongestNumber() const; //Возвращает количество знаков в самом длинном числе

    Matrix& generateIdentityMatrix(int rank);  //Создание единичной матрицы порядка rank
    void generateCopyMatrix(Matrix *source); //Создание матрицы, равной данной (с выделением соответствующей памяти)
    //Создание противоположной матрицы (-А)
    int copyElements(Matrix *to, Matrix *from); //Копирование элементов матриц из from в to (память в to уже должна быть выделена)

    void freeMatrix(); //Деструктор. Освобождение памяти, выделенной в initMatrix(), initialised устанавливает в false
    ~Matrix();

    /*---------------------------------------------------------------------------------*
     *                       Операции над матрицами
     * (int func()возвращают 0, если операция выполнена успешно
     *          и 1, если это сделать невозможно)
     *--------------------------------------------------------------------------------*/
    int multiplMatrix(Matrix const *first, Matrix const *second); //Произведение матриц (с проверкой на выделение памяти)
    int full_mul_matrix(Matrix const *first, Matrix const *second);
    void operator=(const Matrix &M);
    Matrix& operator*=(const Matrix &A);
    Matrix& operator+=(const Matrix &A);
    int squareMatrix(Matrix *source); //Возведение матрицы в квадрат с проверкой на выделение памяти

    int powerMatrix(Matrix *source, int num); //Возведение матрицы source в степень num
    int fast_powerMatrix(Matrix *source, int num); //Возведение матрицы source в степень num более быстрым способом
    void transpose(const Matrix &M);
    Matrix transpose2();
    float det() const;
    float full_det() const;
    Matrix& Minor(const Matrix &Source, int i, int j);
    Matrix& reverse(const Matrix &Source); //Обратная матрица

    void Add_Ident();  //Приписывание справа единичной матрицы
    void Gauss();       //Приведение к ступенчатому виду методом Гаусса, не работает для матриц типа
    // 1 1 1
    // 0 0 1
    // 0 1 0  работает с точность до перестановки строк

    //Элементарные преобразования
    void Add_String(int from, int to, float lambda);
    void StringSwap(int i, int j);
    void MulString(int i, float lambda);
    //Сумма матриц
    //Разность матриц
    //Умножение на число
    //транспонирование
  //Преобразования матриц
    //Умножение строки на число
    //Перестановка двух строк
    //Добавление к i-й строке ее k-й строки с коэффицентом l (Посмотреть, буду ли то же самое со столбцами)
    //Приведение к ступенчатому виду


};
    Matrix& operator+(const Matrix &A, const Matrix &B);

#endif // MATRIX_H
