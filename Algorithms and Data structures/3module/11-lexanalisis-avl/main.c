#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

typedef char boolean;
//typedef int (*key_type)();
typedef int (*cmp_void)(const char *, const char *);
typedef char val_type[MAX_LEN];

typedef struct Node
{
//    char value[MAX_LEN];
    char *value;
    int key;
    int balance;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} *node;

typedef struct Tree
{
    node Head;
    int last_index;
    cmp_void compare;

} *tree;

//-------------------Инициализация/удаление дерева/его элементов

tree init_tree(cmp_void cmp)
{
    tree New_tree = (tree)malloc(sizeof(struct Tree));
    New_tree->Head = NULL;
    New_tree->last_index = 0;
    New_tree->compare = cmp;
    return New_tree;
}

node init_node()
{
    node New_node = (node)malloc(sizeof(struct Node)+MAX_LEN);
    New_node->left = New_node->right = New_node->parent = NULL;
    New_node->value = (char *)(New_node+1);
    New_node->balance = 0;
    return New_node;
}

void delete_nodes_after(node Head)
{
    if(Head){
        delete_nodes_after(Head->left);
        delete_nodes_after(Head->right);
        free(Head);
        Head = NULL;
    }
}

void delete_tree(tree T)
{
    delete_nodes_after(T->Head);
    free(T);
    T = NULL;
}

#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
void show_nodes(node Head, int counter)
{
    if(Head){
        SHOW_RECURSION_DEPTH(counter);
//        if(Head->parent){
//            printf("parent = %d %s ", Head->parent->key, Head->parent->value);
//        }
        printf("[%d] %s\n", Head->key, Head->value);
        show_nodes(Head->right, counter+1);
        show_nodes(Head->left, counter+1);
    }
}

void show_tree(tree T)
{
    printf("SHOW TREE:\n");
    show_nodes(T->Head, 0);
}

//--------------------------Вставка нового элемента--------------------------

//Добавляет новое значение в дерево идентификаторов, вычисляет его номер и возвращает указатель на него.
node insert(tree T, node New_elem) //insertavl 73
{
    if(!T->Head) {
        T->Head = New_elem;
        New_elem->key = 0;
        T->last_index = 1;
        return New_elem;
    }
    node Temp = T->Head;
//     printf("Temp[%d] %s, Value %s, flag = %d\n", Temp->key, Temp->value, New_elem->value, strcmp(Temp->value, New_elem->value));
    int flag;
    while(1){
        flag = T->compare(Temp->value, New_elem->value);
//        printf("Temp[%d] %s, Value %s, flag = %d\n", Temp->key, Temp->value, New_elem->value, flag);
        if(!flag){ //Такой элемент уже есть, добавлять ничего не надо
//            printf("HURREY %s %d\n", New_elem->value, Temp->key);
            free(New_elem);
            return Temp;
        }
        if(flag == -1){
            if(!Temp->left){
                Temp->left = New_elem;
                New_elem->parent = Temp;
                New_elem->key = T->last_index;
                ++T->last_index;
                return New_elem;
            } else {
                Temp = Temp->left;
            }
        } else {
            if(!Temp->right){
                Temp->right = New_elem;
                New_elem->parent = Temp;
                New_elem->key = T->last_index;
                ++T->last_index;
                return New_elem;
            } else {
                Temp = Temp->right;
            }
        }
    }
    return NULL; //Сюда не будет передано управление
}

void rotate_left(tree T, node leaf);
void rotate_right(tree T, node leaf);

node insert_avl(tree T, node elem)
{
    node a = insert(T, elem);
    node x = a->parent;
    a->balance = 0;
    while(1){
        x = a->parent;
        if(!x){
            return a;
        }
        if(a == x->left){ //Левая вершина от родителя
            --x->balance;
            if(!x->balance){
                break;
            }
            if(x->balance == -2){
                if(a->balance == 1){
                    rotate_left(T, a); // ////////////////////
                }
                rotate_right(T, x);
                break;
            } //if(x->balance == -1)
        } //(a = x->left)
        else {     //Правая вершина от родителя
            ++x->balance;
            if(!x->balance){
                break;
            }
            if(x->balance == 2){
                if(a->balance == -1) {
                    rotate_right(T, a);
                }
                rotate_left(T, x);
                break;
            }
        } //else
//        a = x;
    }
    return a;
}
//-----------------------Повороты-------------------------

void replace_node_destructive(tree T, node x, node y)
{
    if(x == T->Head){
        delete_nodes_after(T->Head);
        T->Head = y;
        return;
    }
    node p = x->parent;
    if(y){
        //Открепляем вершину y от родительского узла, если он существует
        if(y->parent){
            if(y == y->parent->left){
                y->parent->left = NULL;
            } else if(y == y->parent->right){
                    y->parent->right = NULL;
            }
        }
        y->parent = x->parent;
    }
    if(p->left == x) {
        p->left = y;
    } else {
        p->right = y;
    }
    delete_nodes_after(x);  //Удаляем поддерево, порожденное вершиной х
}

void rotate_left(tree T, node leaf)
{
    node x = leaf;
    node y = x->right;
    node parent = x->parent;

    if(!y) {
        printf("ROTATE_LEFT: Operation is incorrect, right child is NULL.\n");
        return;
    }

    if (x != T->Head)
    {
        if (parent->left == x)
        {
            parent->left = y;
        }
        else
        {
            parent->right = y;
        }
    }
    else
    {
        T->Head = y;
    }
    y->parent = parent;
    x->parent = y;
    x->right = y->left;

    if (x->right)
    {
        x->right->parent = x;
    }

    y->left = x;

        --x->balance;
        if(y->balance > 0){
            x->balance -= y->balance;
        }
        --y->balance;
        if(x->balance < 0){
            y->balance += x->balance;
        }
}

void rotate_right(tree T, node leaf)
{
    node x = leaf;
    node y = x->left;
    node parent = x->parent;

    if(!y) {
        printf("ROTATE_LEFT: Operation is incorrect, left child is NULL.\n");
        return;
    }

    if (x != T->Head)
    {
        if (parent->right == x)
        {
            parent->right = y;
        }
        else
        {
            parent->left = y;
        }
    }
    else
    {
        T->Head = y;
    }
    y->parent = parent;
    x->parent = y;
    x->left = y->right;

    if (x->left)
    {
        x->left->parent = x;
    }

    y->right = x;
    ++x->balance;
    if(y->balance < 0){
        x->balance -= y->balance;
    }
    ++y->balance;
    if(x->balance > 0){
        y->balance += x->balance;
    }
}


//-------------------------------Лексический анализ---------------------

int spec_symbol(int c)
{
    switch(c){
    case '+' : return 0;
    case '-' : return 1;
    case '*' : return 2;
    case '/' : return 3;
    case '(' : return 4;
    case ')' : return 5;
    default : return -1;
    }

}

void get_ident(char *buffer, int bufsize, int *global_counter){
    int c;
    int len;
    for(len = 0; (len < (bufsize)-1) && (isalpha(c = getchar()) || isdigit(c)); ++len){
        buffer[len] = (char)c;
        *global_counter += 1;
    }
    ungetc(c, stdin);
    buffer[len] = '\0';
}

void scaner(tree Idents)
{
    int c;
    node New_ident = NULL;
    int i, n;
    scanf("%d ", &n);
    while((c = getchar()) != '\n' && c != EOF){
        if(isspace(c)){
            continue;
        }
        if(isdigit(c)){
            ungetc(c, stdin);
            scanf("%d", &c);
            printf("CONST %d\n", c);
            continue;
        }
        if(isalpha(c)){
            New_ident = init_node();
            New_ident->value[0] = c; //Чтобы не возвращать лишний раз символ в поток ввода, запишем его сразу в строку
            get_ident(New_ident->value+1, MAX_LEN-1, &i);
            New_ident = insert_avl(Idents, New_ident); //Вставить новый экземпляр
            printf("IDENT %d\n", New_ident->key);
            continue;
        }
        else {
            printf("SPEC %d\n", spec_symbol(c));
        }
    }
}

int main(void)
{
    tree Identifiers = init_tree(strcmp);
//    node one = init_node();
//    one->value = "abc";
//    node second = init_node();
//    second->value = "second";
//    one = insert(Identifiers, one);
//    second = insert(Identifiers, second);
    scaner(Identifiers);
//    show_tree(Identifiers);
//    replace_node(Identifiers, one, second);
//    rotate_left(Identifiers, Identifiers->Head->left);
    show_tree(Identifiers);
    delete_tree(Identifiers);
    return 0;
}

/*Лексический анализ
Условие
Баллы: 3

Пусть константа – это непустая последовательность десятичных цифр.

Пусть специальный знак – это один из следующих символов: + - * / ( ).

Пусть идентификатор – это непустая последовательность латинских букв и десятичных цифр, начинающаяся с буквы.

Пусть лексема – это либо константа, либо специальный знак, либо идентификатор.

Известно, что в некоторой строке записаны лексемы и пробелы. Лексемы не обязательно разделены пробелами за исключением случая, когда непосредственно после константы идёт идентификатор. Назовём такую строку предложением.

Лексический анализ предложения заключается в выделении из него последовательности записанных в нём лексем. При этом для каждой лексемы вычисляется пара ⟨tag,value⟩, где tag – тип лексемы (CONST для констант, SPEC для специальных знаков и IDENT для идентификаторов), а value – значение лексемы.

Значение лексемы – это неотрицательное целое число, смысл которого зависит от типа лексемы.

Константу мы будем считать десятичной записью её значения.

Значением специального знака пусть будет его порядковый номер в списке + - * / ( ) (нумерация осуществляется, начиная с нуля).

Значение идентификатора определяется следующим образом: если выписать все идентификаторы в том порядке, в каком они входят в предложение, и оставить только первые вхождения каждого идентификатора, то значением идентификатора будет являться его порядковый номер в получившейся последовательности (нумерация осуществляется, начиная с нуля).

Например, если дано предложение

    alpha + x1 (beta alpha) x1 y

то значением идентификатора alpha является число 0, значением идентификатора x1 – число 1, значением beta – число 2, а значением y – число 3.

Составьте программу lexavl.c, выполняющую лексический анализ предложения.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит размер предложения n. Следующая строка содержит само предложение.

Формат результата работы программы

Для каждой лексемы, выделенной из предложения, программа должна выводить в стандартный поток вывода её тип и значение.

Пример работы программы


Входные данные


Выходные данные


18
(a1+10)*(b2-a1)/b2


SPEC 4
IDENT 0
SPEC 0
CONST 10
SPEC 5
SPEC 2
SPEC 4
IDENT 1
SPEC 1
IDENT 0
SPEC 5
SPEC 3
IDENT 1

18
840582  i  j  (


CONST 840582
IDENT 0
IDENT 1
SPEC 4

32
   j   *i 370956 b2  k  41332/b2
IDENT 0
SPEC 2
IDENT 1
CONST 370956
IDENT 2
IDENT 3
CONST 41332
SPEC 3
IDENT 2

Указание.
    В процессе лексического анализа необходимо использовать АВЛ-дерево, хранящее отображение идентификаторов в их значения.

    http://pastebin.com/aSr97Zuc

    valgrind ~/Qt/3-module/11-lexanalisis-avl-Debug/11-lexanalisis-avl
*/
