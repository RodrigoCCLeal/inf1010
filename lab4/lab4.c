#include <stdio.h>
#include <stdlib.h>
#define MAX 4
#define MIN 2
typedef struct no t_no;
struct no{
    int ndesc; // Numero de chaves dentro
    int chave[MAX];
    t_no* ramo[MAX+1];};

void intervalo(t_no *arv, int lim_inf, int lim_sup);

int main (void){
    return 0;
}

void intervalo(t_no *arv, int lim_inf, int lim_sup){
    return;
}

t_no* no_novo(int ndesc){}