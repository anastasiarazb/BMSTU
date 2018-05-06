#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 10

typedef char boolean;
typedef int (*cmp_void)(int, int);
typedef char val_type[MAX_LEN];

typedef struct Node
{
    char *value;
    int key;
    int count;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} *node;

typedef struct Tree
{
    node Head;
    cmp_void compare;

} *tree;

//-------------------Инициализация/удаление дерева/его элементов

tree init_tree(cmp_void cmp)
{
    tree New_tree = (tree)malloc(sizeof(struct Tree));
    New_tree->Head = NULL;
    New_tree->compare = cmp;
    return New_tree;
}

node init_node()
{
    node New_node = (node)malloc(sizeof(struct Node)+MAX_LEN);
    New_node->left = New_node->right = New_node->parent = NULL;
    New_node->value = (char *)(New_node+1);
    New_node->count = 1;
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

#ifndef SHOW_RECURSION_DEPTH
#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
#endif
void show_nodes(node Head, int counter)
{
    if(Head){
        SHOW_RECURSION_DEPTH(counter);
        printf("[%d] %s counter = %d\n", Head->key, Head->value, Head->count);

        SHOW_RECURSION_DEPTH(counter);
        printf("right   [%d] %s counter = %d:\n", Head->key, Head->value, Head->count);
        show_nodes(Head->right, counter+1);
        putchar('\n');
        SHOW_RECURSION_DEPTH(counter);
        printf("left:   [%d] %s counter = %d:\n", Head->key, Head->value, Head->count);
        show_nodes(Head->left, counter+1);
        putchar('\n');
    }
}

void show_tree(tree T)
{
    printf("SHOW TREE:\n");
    show_nodes(T->Head, 0);
}

//--------------------------Вставка нового элемента--------------------------

//Благодаря принципу построения бинарного дерева в левом поддереве расположены все элементы, меньшие
//его и только они, поэтому ранг можно вычислять по высоте левого поддерева, если оно существует
#define rank(x) (x->left ? x->left->count : 0)


//Добавляет новое значение в дерево идентификаторов, вычисляет его номер и возвращает указатель на него.
node insert(tree T, node New_elem) //insertavl 73
{
    if(!T->Head) {
        T->Head = New_elem;
        return New_elem;
    }
    node Temp = T->Head;
//     out("Temp[%d] %s, Value %s, flag = %d\n", Temp->key, Temp->value, New_elem->value, strcmp(Temp->value, New_elem->value));
    int flag;
    while(1){
        flag = T->compare(New_elem->key, Temp->key);
//        out("Temp[%d] %s, Value %s, flag = %d\n", Temp->key, Temp->value, New_elem->value, flag);
        if(!flag){ //Такой элемент уже есть, добавлять ничего не надо
            printf("INSERT: elem is already exist %s %d\n", New_elem->value, Temp->key);
            free(New_elem);
            return Temp;
        }
        ++Temp->count; //Увеличиваем высоту этого поддерева, так как в него будет добавлен элемент
        if(flag == -1){
            if(!Temp->left){
                Temp->left = New_elem;
                New_elem->parent = Temp;
                New_elem->count += rank(New_elem);
                return New_elem;
            } else {
                Temp = Temp->left;
            }
        } else {
            if(!Temp->right){
                Temp->right = New_elem;
                New_elem->parent = Temp;
                New_elem->count += rank(New_elem);
                return New_elem;
            } else {
                Temp = Temp->right;
            }
        }
    }
    return NULL; //Сюда не будет передано управление
}

node search_by_rank(tree T, int index)
{
    node Temp = T->Head;
    if(!Temp){
        printf("SEARCH BY RANK: tree is empty.\n");
        return NULL;
    }
//    printf("SEARCH_by_rank index %d, ", index);
    while(rank(Temp) != index){
        while(rank(Temp) > index){
            Temp = Temp->left;     //Искомый индекс попадает в множество элементов левого подмассива
        }
        while(rank(Temp) < index){ //Искомый индекс расположен в правом подмассиве. Мы можем инкрементировать индексы настолько, чтобы считать, будто левого поддерева (включая текущий элемент) и не было
            index -= rank(Temp) + 1;
            Temp = Temp->right;
        }
    }
    return Temp;
}

//----------------------------------------------------------

tree replace_node(tree T, const node x, node y)
{
    if(x == T->Head){
        T->Head = y;
        y->parent = NULL;
        return T;
    }
    node p = x->parent;
    if(y){
        y->parent = p;
    }
    if(p->left == x){
        p->left = y;
    } else {
        p->right = y;
    }
    return T;
}

node descend(tree T, int key)
{
    node Temp = T->Head;
    while(Temp && T->compare(Temp->key, key)){
        if(T->compare(Temp->key, key) == 1){
            Temp = Temp->left;
        } else {
            Temp = Temp->right;
        }
    }
    if(!Temp){
        printf("DESCEND: object %d does not exist\n", key);
    }
    return Temp;
}

node minimum(node Head){
    if(!Head){
        printf("MINIMUM: tree is empty\n");
        return NULL;
    } else {
        node Temp = Head;
        while(Temp->left) {
            Temp = Temp->left;
        }
        return Temp;
    }
}

//Операция получения следующей словарной пары
node succ(node x)
{
    if(x->right){
        return minimum(x->right);
    }
    node y = x->parent;
    while(y && x == y->right){
        x = y;
        y = y->parent;
    }
    return y;
}

//Уменьшить на единицу высоту всех узлов, в поддеревья которых входил данный элемент (при удалении)
void decrease_count(node x)
{
    node Temp = x->parent;
    while(Temp){
        --Temp->count;
        Temp = Temp->parent;
    }
}

tree delete_node(tree T, int key)
{
    node x = descend(T, key);
//    show_tree(T);
    if(!x){
        printf("DELETE: object %d does not exist\n", key);
        return T;
    }
    if(x->left && x->right){ //У вершины есть оба потомка - заменяем на следующий по величине ключа
        node y = succ(x);
        decrease_count(y);   //Уменьшить на единицу высоту всех узлов выше заменяющего
        replace_node(T, y, y->right);   //Вынуть заменяющий элемент со своего места
        // Прикрепить дочерние элементы x к y
        x->left->parent = y;
        y->left = x->left;
        if(x->right){
            x->right->parent = y;
        }
        y->right = x->right;
        y->count = x->count;
        //Прикрепить y к родительскому элементу x
        replace_node(T, x, y);
        free(x);
        return T;
    }
    decrease_count(x);       //Уменьшить на единицу высоту всех узлов выше удаляемого
    if(!x->left && !x->right) {    //У вершины нет потомков
        replace_node(T, x, NULL);
        free(x);
        return T;
    }
    if(!x->left){        //Левой вершины нет, достаточно заменить на правую
        replace_node(T, x, x->right);
        free(x);
        return T;
    }
    if(!x->right){
        replace_node(T, x, x->left);
        free(x);
        return T;
    }
    return NULL;  //Сюда не будет передано управление
}

//-----------------------------------------------------------

node scan_elem()
{
    node New = init_node();
    scanf("%d %s", &New->key, New->value);
    return New;
}

int compare(int a, int b)
{
    if(a == b) return 0;
    return a > b ? 1 : -1;
}

#define NAME_LEN 100 //Длина имени команды
#define COMMAND(str) if(!strcmp(command, #str))
void menu(tree T, int n)
{
    int  i;
    char command[NAME_LEN];
    node Temp = NULL;
    int arg;
    for(i = 0; i < n; ++i, Temp = NULL){
//        if(!(i % 1000)) show_tree(T);
        scanf("%s", command);
        COMMAND(INSERT){  //INSERT k v (добавить в дерево словарную пару, в которой ключ k – целое число, значение v – строка, составленная из латинских букв)
            Temp = init_node();
            scanf("%d %s", &Temp->key, Temp->value);
            insert(T, Temp);
            continue;
        }
        COMMAND(LOOKUP){ //LOOKUP k (вывести строку, связанную с ключом k)
            scanf("%d", &arg);
            Temp = descend(T, arg);
            printf("%s\n", Temp->value);
            continue;
        }
        COMMAND(DELETE){  // DELETE k (удалить строку, связанную с ключом k)
            scanf("%d", &arg);
            delete_node(T, arg);
            continue;
        }
        COMMAND(SEARCH){ //SEARCH x (вывести строку, связанную с ключом, имеющим порядковый номер x)
            scanf("%d", &arg);
            Temp = search_by_rank(T, arg);
            printf("%s\n", Temp->value);
        }
    }
}

int main(void)
{
    tree T = init_tree(compare);
    int n;
    scanf("%d", &n);
    menu(T, n);
//    show_tree(T);
    delete_tree(T);
    return 0;
}


//int main(void)
//{
//    tree T = init_tree(compare);
//    node temp = NULL;
//    int i;
//    for(i = 0; i < 6; ++i){
//        temp = scan_elem();
//        insert(T, temp);
//    }
//    SHOW_TREE(T);
//    search_by_rank(T, 0);
//    search_by_rank(T, 2);
//    delete_node(T, 5);
//    SHOW_TREE(T);
//    search_by_rank(T, 2);
//    delete_tree(T);
//    return 0;
//}

/*
 *
Ранги вершин бинарного дерева поиска
Условие
Баллы: 3

Операция SearchByRank : A × ℕ → P для ассоциативного массива возвращает словарную пару с заданным номером в отсортированной последовательности входящих в ассоциативный массив словарных пар.

Модифицируйте представление и реализацию бинарного дерева поиска, чтобы операция SearchByRank для него работала за время O(h), где h – высота дерева.

Составьте программу ranktree.c, демонстрирующую работоспособность реализованной операции.
Формат входных данных

Первая строка, считываемая со стандартного потока ввода, содержит общее количество выполняемых операций n (0 < n ≤ 100000). Каждая из следующих n строк содержит описание операции.

Операция либо имеет форму INSERT k v (добавить в дерево словарную пару, в которой ключ k – целое число, значение v – строка, составленная из латинских букв; -1000000000 < k < 1000000000, len(v) < 10), либо форму LOOKUP k (вывести строку, связанную с ключом k), либо форму DELETE k (удалить строку, связанную с ключом k), либо форму SEARCH x (вывести строку, связанную с ключом, имеющим порядковый номер x).

Можно считать, что последовательность операций составлена правильно.

Формат результата работы программы

Для каждой операции LOOKUP вывести в стандартный поток вывода строку, связанную с ключом k. Для каждой операции SEARCH вывести в стандартный поток вывода строку, связанную с ключом, имеющим порядковый номер x.

Пример работы программы


Входные данные


Выходные данные


7
INSERT 10 qwerty
INSERT 15 asdf
INSERT 5 zxc
SEARCH 2
LOOKUP 10
DELETE 10
SEARCH 1


asdf
qwerty
asdf

valgrind ~/Qt/3-module/10-rank-tree-Debug/10-rank-tree < /home/nastasia/Qt/3-module/10-rank-tree/10test.txt > /home/nastasia/Qt/3-module/10-rank-tree/10out-my.txt


Указание.
    Представление бинарного дерева поиска нужно модифицировать следующим образом: в каждую вершину нужно добавить поле count, содержащее размер поддерева с корнем в данной вершине. Размер поддерева – это количество вершин в нём.

6
INSERT -10 fbeaaae
INSERT -4 df
INSERT 8 aa
LOOKUP -4
DELETE -10
DELETE -4

df



7
INSERT 5 d
INSERT -5 dc
INSERT -1 cfabdec
LOOKUP 5
DELETE 5
SEARCH 1
INSERT -3 aacaec

d
cfabdec

10 qwerty
15 asdf
5 zxc
-10 fbeaaae
-4 df
8 aa


14
INSERT 7 fcdcbbafe
INSERT 2 cdbdb
INSERT -1 bbbeacfb
LOOKUP 7
DELETE -1
DELETE 2
SEARCH 0
INSERT -9 ecdedd
LOOKUP -9
DELETE -9
SEARCH 0
INSERT 6 e
INSERT -3 bfcff
LOOKUP 7


fcdcbbafe
fcdcbbafe
ecdedd
fcdcbbafe
fcdcbbafe



 *
 *
 *http://pastebin.com/wKh3z2a9*/
