

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct leaf_t leaf_t;

//#define DEBUG
#ifdef DEBUG
#define exec_db(x) x
#else
#define exec_db(x)
#endif

typedef int key_t;
typedef struct
{
    key_t key;
    char value[10];
} pair_t;

struct leaf_t
{
    int     count;
    pair_t  pair;
    leaf_t* parent;
    leaf_t* right;
    leaf_t* left;
};

typedef struct
{
    leaf_t* root;
} binarytree_t;

void binarytreeInit(binarytree_t* bintree)
{
    memset(bintree, 0, sizeof(binarytree_t));
}

leaf_t* binarytreeDensert(binarytree_t* bintree, key_t key)
{
    leaf_t* x = bintree->root;
    while (x != NULL && x->pair.key != key)
    {
        if (x->pair.key > key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    return x;
}

const leaf_t* binarytreeLookUp(binarytree_t* bintree, key_t key)
{
    return binarytreeDensert(bintree, key);
}

void binarytreeReplaceNode(binarytree_t* bintree, leaf_t* x, leaf_t* y)
{
    if (bintree->root == x)
    {
        bintree->root = y;
        return;
    }

    leaf_t* p = x->parent;
    if (y != NULL)
    {
        y->parent = p;
    }
    if (p->left == x)
    {
        p->left = y;
    }
    else
    {
        p->right = y;
    }
}

#define SHOW_RECURSION_DEPTH(i) {int k; for(k = 0; k < i; ++k) printf("  |");}
void show_nodes(leaf_t *Head, int counter)
{
    if(Head){
        SHOW_RECURSION_DEPTH(counter);
//        if(Head->parent){
//            printf("parent = %d %s\n", Head->parent->key, Head->parent->value);
//            SHOW_RECURSION_DEPTH(counter);
//        }
        printf("[%d] %s counter = %d\n", Head->pair.key, Head->pair.value, Head->count+1);

        SHOW_RECURSION_DEPTH(counter);
        printf("right   [%d] %s counter = %d:\n", Head->pair.key, Head->pair.value, Head->count+1);
        show_nodes(Head->right, counter+1);
        putchar('\n');
        SHOW_RECURSION_DEPTH(counter);
        printf("left:   [%d] %s counter = %d:\n", Head->pair.key, Head->pair.value, Head->count+1);
        show_nodes(Head->left, counter+1);
        putchar('\n');
    }
}

void show_tree(binarytree_t *T)
{
    printf("SHOW TREE:\n");
    show_nodes(T->root, 0);
}

void binarytreeDecreaseHeight(leaf_t* leaf)
{
    for (; leaf != NULL; leaf = leaf->parent)
    {
        leaf->count--;
        exec_db(printf("DECREASE %p\n", leaf));
    }
}

void binarytreeIncreaseHeight(leaf_t* leaf)
{
    for (; leaf != NULL; leaf = leaf->parent)
    {
        leaf->count++;
    }
}

leaf_t* binarytreeInsert(binarytree_t* bintree, pair_t pair)
{
    leaf_t* leaf = calloc(1, sizeof(leaf_t));
    memcpy(&leaf->pair, &pair, sizeof(pair_t));

    if (bintree->root == NULL)
    {
        bintree->root = leaf;
        return leaf;
    }

    leaf_t* x = bintree->root;
    for(;;)
    {
        if (x->pair.key == pair.key)
        {
            return NULL;
        }

        if (x->pair.key > pair.key)
        {
            if (x->left == NULL)
            {
                x->left = leaf;
                leaf->parent = x;
                binarytreeIncreaseHeight(leaf->parent);
                break;
            }
            x = x->left;
        }
        else
        {
            if (x->right == NULL)
            {
                x->right = leaf;
                leaf->parent = x;
                binarytreeIncreaseHeight(leaf->parent);
                break;
            }
            x = x->right;
        }
    }
    return leaf;
}

leaf_t* minimal(leaf_t* x)
{
    if (x == NULL) return NULL;
    leaf_t* search = x;

    while (search->left != NULL)
        search=search->left;
    return search;
}

leaf_t* binarytreeSucc(leaf_t* x)
{
    if (x->right != NULL)
        return minimal(x->right);

    leaf_t* y = x->parent;
    while(y != NULL && x == y->right)
    {
        x = y;
        y = y->parent;
    }

    return y;
}

leaf_t* binarytreeSearchByRank(binarytree_t* bintree, int rank)
{
    leaf_t* ptr = bintree->root;
    for(;;)
    {
        int lrank = (ptr->left)? ptr->left->count + 1 : 0;
        if (rank - lrank == 0)
        {
            break;
        }

        if (ptr->left && ptr->left->count >= rank)
        {
            ptr = ptr->left;
            continue;
        }

        rank = rank - lrank - 1;
        if (ptr->right)
        {
            ptr = ptr->right;
        }
        else
        {
            break;
        }
    }
    return ptr;
}

void binarytreeDelete(binarytree_t* bintree, key_t key)
{
    leaf_t* x = binarytreeDensert(bintree, key);

    if (x == NULL) return;

    if (x->left == NULL && x->right == NULL)
    {
        binarytreeDecreaseHeight(x->parent);
        binarytreeReplaceNode(bintree, x, NULL);
    }
    else if (x->right == NULL)
    {
        binarytreeDecreaseHeight(x->parent);
        binarytreeReplaceNode(bintree, x, x->left);
    }
    else if (x->left == NULL)
    {
        binarytreeDecreaseHeight(x->parent);
        binarytreeReplaceNode(bintree, x, x->right);
    }
    else
    {
        leaf_t* y = binarytreeSucc(x);
        binarytreeDecreaseHeight(y->parent);
        binarytreeReplaceNode(bintree, y, y->right);

        x->left->parent = y;
        y->left = x->left;
        if (x->right != NULL)
        {
            x->right->parent = y;
        }
        y->right = x->right;
        y->count = x->count;
        binarytreeReplaceNode(bintree, x, y);
    }

    exec_db(printf("DELETE %p\n",x));
    free(x);
    if (bintree->root) bintree->root->parent = NULL;
}

int main(void)
{
    binarytree_t binarytree;
    binarytreeInit(&binarytree);

    int n;
    scanf("%i", &n);

    char command[10];
    int i = 0;
    pair_t pair;

    for (i = 0; i < n; i++)
    {
//        if(! (i%1000))show_tree(&binarytree);
        scanf("%s", command);
        if (command[0] == 'I') //insert
        {
            scanf("%i %s", &pair.key, pair.value);
            binarytreeInsert(&binarytree, pair);
        }
        else if (command[0] == 'L') // Lookup
        {
            scanf("%i", &pair.key);
            printf("%s\n", (binarytreeLookUp(&binarytree, pair.key)->pair.value));
        }
        else if (command[0] == 'D') // Delete
        {
            scanf("%i", &pair.key);
            binarytreeDelete(&binarytree, pair.key);
        }
        else if (command[0] == 'S')
        {
            int val;
            scanf("%i", &val);
            printf("%s\n", (binarytreeSearchByRank(&binarytree, val)->pair.value));
        }
        fflush(stdout);
    }
//    show_tree(&binarytree);

    while(binarytree.root != NULL)
    {
        binarytreeDelete(&binarytree, binarytree.root->pair.key);
    }
    return 0;
}

//valgrind ~/Qt/3-module/10-rank-tree/anton-Debug/anton
