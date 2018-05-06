#include <stdio.h>
#include <stdlib.h>

#define POW2(i) (1 << (i))

typedef int my_type;
typedef my_type (* bin_op)(my_type, my_type);

void fenwick_tree_build(int nel, bin_op op, bin_op rev_op);
int fenwick_tree_query(int r, int l, int n, bin_op op);
void fenwick_tree_update(int i, int val, int n, bin_op op);
void menu(int n, bin_op op);

my_type *mass;
//int mass[10] = {2, 3, 5, 1, 0, 4, 7, 6, 2, 3}; //В слайдах опечатка, там посчитано для 9-го элемента, равного 3
my_type *fenwick_tree;

/*-------Бинарные операции-----------*/
int sum(int a, int b)
{
    return a + b;
}
int max(int a, int b)
{
    return (a > b) ? a : b;
}


#define OPERATION max

int main(void)
{
    int n;
    scanf("%d", &n);
    mass = (int *)malloc(n*sizeof(int));
    int i;
    for(i = 0; i < n; ++i){
        scanf("%d", mass+i);
    }
    fenwick_tree_build(n, OPERATION);
    menu(n, OPERATION);
//    fenwick_tree_build(10, sum);
//    out("I'M HERE?\n");
//    out("sum[3:7]= %d\n", fenwick_tree_query(10, 3, 7, sum));
//    fenwick_tree_update(3, 0, 10, sum);
//    fenwick_tree_query(10, 3, 7, sum);
//    out("after?\n");
    free(mass);
    free(fenwick_tree);
    return 0;
}

#define COMMAND(str) if(!strcmp(command, str))
void menu(int n, bin_op op)
{
    int m, i;
    char command[4]; //Команда состоит фиксированно из 3-х букв.
    scanf("%d", &m);
    int arg1, arg2;
    for(i = 0; i < m; ++i){
        scanf("%3s %d %d", command, &arg1, &arg2);
        COMMAND("UPD"){
            fenwick_tree_update(arg1, arg2, n, op);
        }
        COMMAND("MAX"){
            printf("%d\n", fenwick_tree_query(arg1, arg2, n, op));
        }
    }
}
#define PARAMETRES nel, op, rev_op
   /*------------Построение дерева----------*/
void fen_build_help(int current, int l, int r, const int nel, bin_op op, bin_op rev_op, int counter)
{
    int sum = 0;
    int bound = (r < n) ? r : n;
    while(l < bound){
        m = (l+r)/2;
        sum = op(sum, fen_build_help(current, l, r, PARAMETRES, counter+1));
    }
    out("current = %d, [%d:%d] = %d\n", current, a, b, fen_tree[current]);

}

void fenwick_tree_build(int nel, bin_op op, bin_op rev_op)
{
    out("FENWICK TREE BUILD:\n");
    fen_tree = (my_type *)malloc(n*sizeof(my_type));
    int r;
    for(r = 0; POW2(r) <= n; ++r);
    fen_build_help(0, 0, r-1, PARAMETRES, 0);
}


      /*----------------Обращение к дереву-----------------*/
int fen_query_help(int l, int r, int current, int a, int b, bin_op op, int counter)
{
    SHOW_RECURSION_DEPTH(counter);
    out("current = %d, l:r = [%d:%d] m = %d a:b = [%d:%d]\n",
        current, l, r, (a+b)/2, a, b);


}
int fenwick_tree_query(int l, int r, int n, bin_op op)
{
    out("FENWICK TREE QUERY: [%d:%d]\n", l, n);
    return fen_query_help(l, r, 0, 0, n-1, op, 0);
}


     /*-----------------Обновление дерева----------------------*/

void fen_upd_help(int current, int a, int b, const int i, const int val, bin_op op, int counter)
{

}

void fenwick_tree_update(int i, int val, int n, bin_op op)
{
    out("FENWICK TREE UPDATE: val = %d, i = %d\n", val, i);
    fen_upd_help(0, 0, n-1, i, val, op, 0);
}
