#ifndef BINTREE_H
#define BINTREE_H
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
#endif // BINTREE_H
