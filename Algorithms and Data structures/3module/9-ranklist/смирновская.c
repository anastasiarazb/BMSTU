#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Struct
typedef struct List list;

struct List {
    int k;
    list **next;
    char v[10]; //len(v) < 10
    int *span;
};

// Global variables
int m;
list *l;

// Functions
list *InitList();
char *Lookup(int k);
void Insert(int k, char *str);
void Delete(int k);
int Rank(int k);

// Commands
char in[7] = "INSERT", d[7] = "DELETE", r[5] = "RANK", look[7] = "LOOKUP";
char *cmd;

//int main(int argc, char **argv) {
//    int n, k, i;
//    cmd = (char*)malloc(128*sizeof(char));
//    scanf("%d", &n);
//    m = log2((double)n);
//    l = InitList();
//    for(i = 0; i < n; i++) {
//        scanf("%s", cmd);
//        if(strcmp(cmd, in) == 0) {
//            scanf("%d", &k);
//            scanf("%s", cmd);
//            Insert(k, cmd);
//        }
//        if(strcmp(cmd, look) == 0) {
//            scanf("%d", &k);
//            strcpy(cmd, Lookup(k));
//            printf("%s\n", cmd);
//        }
//        if(strcmp(cmd, d) == 0) {
//            scanf("%d", &k);
//            Delete(k);
//        }
//        if(strcmp(cmd, r) == 0) {
//            scanf("%d", &k);
//            printf("%d\n", Rank(k));
//        }
//    }
//    list *x, *y = l;
//    k = 0;
//    while(y->next[k] != NULL) {
//        x = y;
//        y = y->next[0];
//        free(x->next);
//        free(x->span);
//        free(x);
//    }
//    free(y->next);
//    free(y->span);
//    free(y);
//    free(cmd);
//    return 0;
//}

list *InitList() {
    int i = 0;
    list *lst = (list*)malloc(sizeof(list));
    lst->next = (list**)malloc(m*sizeof(list*));
    lst->span = (int*)malloc(m*sizeof(int));
    while(i < m) {
      lst->next[i] = NULL;
      lst->span[i] = 0;
      ++i;
      }
    return lst;
}

list *Succ(list *x) {
    return x->next[0];
}

void Skip(int k, list **p) {
    list *x = l;
    int i = m - 1;
    while(i >= 0) {
        while(x->next[i] != 0 && x->next[i]->k < k)
            x = x->next[i];
        p[i] = x;
        i--;
    }
}

char *Lookup(int k) {
    list **p = (list**)malloc(m*sizeof(list*));
    Skip(k, p);
    list *x = Succ(p[0]);
    free(p);
    if(x->v < 0)
       return -1;
    return x->v;
}
void Insert(int k, char *str) {
    list **p = (list**)malloc(m*sizeof(list*));
    Skip(k, p);
    if (p[0]->next[0] != 0 && p[0]->next[0]->k == k)
        return;
    list *x = InitList();
    strcpy(x->v, str);
    x->k = k;
    int r, i, span = 0, j, s = 0;
    for(i = 0, r = rand()*2; i < m && r % 2 == 0; i++, r/=2) {
        x->next[i] = p[i]->next[i];
        p[i]->next[i] = x;
    }
    for(i; i < m; i++)
        x->next[i] = 0;
    int *spansb = (int*)malloc(m*sizeof(int));
    int *spansa = (int*)malloc(m*sizeof(int));
    for(i = 0; i < m; i++)
      spansb[i] = 0;
    for(i = m-1; i > 0; i--) {
       if(p[i]->next[i] != x) {
           ++p[i]->span[i];
         }
       else {
           j=i;
           list *y = p[i];
loop:      while(y->next[j] != x) {
               spansb[j+1] += y->span[j] + 1;
               y = y->next[j];
           }
           if(j != 0) {
              --j;
              goto loop;
           }
           else
              break;
          }
      }
    for(j = i, i = 1; i <= j; i++) {
        s += spansb[i];
        spansa[i] = s;
        x->span[i] = p[i]->span[i] - spansa[i];
        p[i]->span[i] = spansa[i];
    }
    free(p);
    free(spansb);
    free(spansa);
}

int Rank(int k) {
    list *x = l;
    int i, rank;
    for(i = m-1, rank=-1; i >= 0; i--)
        while(x->next[i] != 0 && x->next[i]->k <= k){
            rank += x->span[i] + 1;
            x = x->next[i];
         }
    if(rank < 0)
       return 0;
    return rank;
}

void Delete(int k) {
  list **p = (list**)malloc(m*sizeof(list*));
  Skip(k, p);
  list *x = Succ(p[0]);
  if(x->k != k || x == 0)
      return;
  int i;
  for(i = 0; i < m && p[i]->next[i] == x; i++) {
      p[i]->next[i] = x->next[i];
      p[i]->span[i] += x->span[i];
  }
  for(i; i < m; i++)
      --p[i]->span[i];
  free(x->next);
  free(x->span);
  free(x);
  free(p);
}


