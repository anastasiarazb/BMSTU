#include <stdio.h>
#include <stdlib.h>

struct Elem {
    // "Тэг", описывающий тип значения в "голове" списка
    enum {
        INTEGER,
        FLOAT,
        LIST
    } tag;

    // Само значение в "голове" списка
    union{
        int i;
        float f;
        struct Elem *list;
    } value;

    // Указатель на "хвост" (следующий элемент) списка. Если элемент является последним, то *tail = NULL
    struct Elem *tail;
};

struct Elem *searchlist(struct Elem *list, int k);
#define TEST2

#ifdef TEST1
int main(void)
{
    struct Elem a, b, c, d, e;

    a.tag = INTEGER;
    a.value.i = 10;
    a.tail = &b;

    b.tag = FLOAT;
    b.value.f = 3.14;
    b.tail = &c;

    c.tag = INTEGER;
    c.value.i = 20;
    c.tail = &d;

    d.tag = INTEGER;
    d.value.i = 30;
    d.tail = &e;

    e.tag = INTEGER;
    e.value.i = 50;
    e.tail = NULL; //Элемент является последним в списке

    if (searchlist(&a, 50) == &e) {
        printf("CORRECT\n");
    } else {
        printf("WRONG\n");
    }
    system("pause");
    return 0;
}
#endif
#ifdef TEST2
int main()
{
    struct Elem e9;
    e9.tag = INTEGER;
    e9.value.i = 50;
    e9.tail = NULL;

    struct Elem e7;
    e7.tag = LIST;
    e7.value.list = NULL;
    e7.tail = &e9;

    struct Elem e6;
    e6.tag = FLOAT;
    e6.value.f = 3.14;
    e6.tail = &e7;

    struct Elem e5;
    e5.tag = FLOAT;
    e5.value.f = 10.5e-5;
    e5.tail = &e6;

    struct Elem e4;
    e4.tag = INTEGER;
    e4.value.i = 70;
    e4.tail = &e5;

    struct Elem e17;
    e17.tag = INTEGER;
    e17.value.i = 40;
    e17.tail = NULL;

    struct Elem e16;
    e16.tag = LIST;
    e16.value.list = NULL;
    e16.tail = &e17;

    struct Elem e15;
    e15.tag = FLOAT;
    e15.value.f = 10.5e-5;
    e15.tail = &e16;

    struct Elem e14;
    e14.tag = INTEGER;
    e14.value.i = 20;
    e14.tail = &e15;

    struct Elem e13;
    e13.tag = INTEGER;
    e13.value.i = 40;
    e13.tail = &e14;

    struct Elem e12;
    e12.tag = FLOAT;
    e12.value.f = 10.5e-5;
    e12.tail = &e13;

    struct Elem e11;
    e11.tag = FLOAT;
    e11.value.f = 2.5;
    e11.tail = &e12;

    struct Elem e3;
    e3.tag = LIST;
    e3.value.list = &e4;
    e3.tail = &e11;

    struct Elem e2;
    e2.tag = INTEGER;
    e2.value.i = 80;
    e2.tail = &e3;

    struct Elem e1;
    e1.tag = INTEGER;
    e1.value.i = 60;
    e1.tail = &e2;

    struct Elem e0;
    e0.tag = INTEGER;
    e0.value.i = 40;
    e0.tail = &e1;

    struct Elem *x = searchlist(&e0, 70);
    if (x == &e4) {
        printf("CORRECT\n");
    } else {
        /* Если функция searchlist работает правильно,
        сюда никогда не будет передано управление! */
        printf("WRONG\n");
    }
    return 0;
}
#endif
//Функции передается указатель на первый элемент в списке
//и искомое число
struct Elem *searchlist(struct Elem *list, int k){
    struct Elem *temp = list;  //создаем переменную temp типа "указатель на элемент структуры Elem"
                               //и присваием ей значение из аргумента функции
    struct Elem *temp2 = NULL;
    while (temp) {        //Пока не встретится нулевой указатель, т.е идентификатор последнего элемента
        switch (temp->tag){
        case INTEGER:
            if (temp->value.i == k)
                return temp; //Вернуть указатель на искомый элемент
            break;
        case LIST:
            temp2 = searchlist(temp->value.list, k);
            if (temp2) return temp2;
        case FLOAT:;
        }
        temp = temp->tail;  //Присваеваем переменной значение, хранящееся в элементе tail, т.е. указатель на следующий символ
    }
    return NULL;
}



