#include <iostream>
using namespace std;

#define SQUARE(n) (n)*(n)
long int fastExpt(int x, int y) //Рекурсивный алгоритм - логарифмический рост кол-ва действий и расходуемой памяти
{
    if (y == 0) return 1;
    if (! (y %2 )) {
        int t = SQUARE(fastExpt(x, y/2));
        cout << t << endl;
        return t;
    } else {
        return x * fastExpt(x, --y);
    }
}

//Через циклы - количество итераций в 2 раза меньше обычного
//Пользуемся тем, что A^num = (A^(num/2))^2 для четного num и A^num = А*(A^(num-1)) для нечетного.
long fastExpt1(int x, int y)
{
    if (y == 0) return 1;
    //if (y < 0) return 1/fastExpt1(x, -y);
    int i = y/2, result = 1;
    for (; i > 0; result *= x*x, i--) cout << result << ' ' << i<< endl;
    return y%2 ? result*x : result;
}

long fastExpt2(int x, int y) //Хотела как через рекурсию, только через итерации - не работает
{
    if (y == 0) return 1;
//    if (y < 0) return 1/fastExpt1(x, -y);
    int i = (y-1)/2, result = x;
    for (; i > 0; result *= result, i/=2) cout << result << ' ' << i << endl;
    return y%2 ? result*x : result;
}

int simpleExpt(int x, int y){
    int result;
    if (y == 0) return 1;
    for (result = 1; y; y--, result *= x);
    return result;
}

void showExpt(){
    cout <<endl << fastExpt(3, 15) << endl << endl;
    cout << fastExpt1(3, 15) << endl <<endl;
    cout << fastExpt2(3, 15) << endl;
    cout << simpleExpt(3, 15) << endl;
}
