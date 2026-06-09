#include <stdio.h>
#include <stdlib.h>
#define MAX 4
#define MIN 2

typedef struct no t_no;

struct no{
    int ndesc; // Numero de chaves dentro
    int chave[MAX];
    t_no* ramo[MAX+1];
};

void intervalo(t_no *arv, int lim_inf, int lim_sup);
t_no* cria_arvore_na_mao();

int main (void)
{
    t_no *raiz = cria_arvore_na_mao(); // Cria a árvore manualmente
    intervalo(raiz, 10, 150); // Chama a função para imprimir as chaves dentro do intervalo [50, 150]
    return 0;
}

void intervalo(t_no *arv, int lim_inf, int lim_sup){
    
    if (arv == NULL) // Verifica se o ponteiro que recebe é nulo
    {
        return;
    }

    for(int i = 0; i < arv->ndesc; i++) // Percorre o vetor de chaves do nó, verificando se cada chave está dentro do intervalo
    {
        intervalo(arv->ramo[i], lim_inf, lim_sup); // Chama a função recursivamente para o ramo à esquerda da chave

        if (arv->chave[i] > lim_inf && arv->chave[i] < lim_sup) // Verifica se a chave está dentro do intervalo
        {
            printf("%d ", arv->chave[i]); // Imprime a chave
        }
    }

    intervalo(arv->ramo[arv->ndesc], lim_inf, lim_sup); // Chama a função recursivamente para o ramo à direita da última chave

    return;
}

t_no* cria_arvore_na_mao()
{
    t_no *raiz = (t_no *)malloc(sizeof(t_no));
    raiz->ndesc = 1;
    raiz->chave[0] = 100;

    t_no *ramo_1 = (t_no *)malloc(sizeof(t_no));
    ramo_1->ndesc = 2;
    ramo_1->chave[0] = 50;
    ramo_1->chave[1] = 75;
    raiz->ramo[0] = ramo_1;

    t_no *ramo_2 = (t_no *)malloc(sizeof(t_no));
    ramo_2->ndesc = 2;
    ramo_2->chave[0] = 120;
    ramo_2->chave[1] = 200;
    raiz->ramo[1] = ramo_2;

    t_no *ramo_3 = (t_no *)malloc(sizeof(t_no));
    ramo_3->ndesc = 2;
    ramo_3->chave[0] = 10;
    ramo_3->chave[1] = 40;
    ramo_1->ramo[0] = ramo_3;

    t_no *ramo_4 = (t_no *)malloc(sizeof(t_no));
    ramo_4->ndesc = 2;
    ramo_4->chave[0] = 60;
    ramo_4->chave[1] = 70;
    ramo_1->ramo[1] = ramo_4;

    t_no *ramo_5 = (t_no *)malloc(sizeof(t_no));
    ramo_5->ndesc = 2;
    ramo_5->chave[0] = 80;
    ramo_5->chave[1] = 90;
    ramo_1->ramo[2] = ramo_5;

    t_no *ramo_6 = (t_no *)malloc(sizeof(t_no));
    ramo_6->ndesc = 2;
    ramo_6->chave[0] = 110;
    ramo_6->chave[1] = 115;
    ramo_2->ramo[0] = ramo_6;

    t_no *ramo_7 = (t_no *)malloc(sizeof(t_no));
    ramo_7->ndesc = 4;
    ramo_7->chave[0] = 130;
    ramo_7->chave[1] = 135;
    ramo_7->chave[2] = 140;
    ramo_7->chave[3] = 170;
    ramo_2->ramo[1] = ramo_7;

    t_no *ramo_8 = (t_no *)malloc(sizeof(t_no));
    ramo_8->ndesc = 4;
    ramo_8->chave[0] = 220;
    ramo_8->chave[1] = 230;
    ramo_8->chave[2] = 240;
    ramo_8->chave[3] = 250;
    ramo_2->ramo[2] = ramo_8;
    return raiz;
}
