#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * Lab 5 INF1010 - TAD Conjunto (Set) como vetor de bits
 *
 * Representacao seguindo os slides:
 *   - int max   : numero maximo de elementos do conjunto;
 *   - int *vector: vetor de ints alocado dinamicamente;
 *   - vector[0] guarda os bits 0..31, vector[1] os bits 32..63...
 *   - o elemento i corresponde ao bit (i % bits) do
 *     inteiro vector[i / bits], com bits = sizeof(int)*8.
 * ============================================================ */

struct bitvector {
    int  max;      /* numero maximo de elementos      */
    int *vector;   /* vetor[0] = bits menos signif.    */
};
typedef struct bitvector BitVector;

/* numero de ints necessarios para 'max' bits */
static int num_ints(int max) {
    int bits = sizeof(int) * 8;          /* bits por int (= 32) */
    return ((max - 1) / bits) + 1;
}

/* cria um conjunto vazio (mesma ideia do bvInit dos slides) */
static BitVector *set_cria(int max) {
    int i, num = num_ints(max);
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    bv->max = max;
    bv->vector = (int *) malloc(num * sizeof(int));
    for (i = 0; i < num; i++)
        bv->vector[i] = 0;
    return bv;
}

/* faz uma copia de s (para nao corromper a entrada) */
static BitVector *set_copia(BitVector *s) {
    int i, num = num_ints(s->max);
    BitVector *bv = set_cria(s->max);
    for (i = 0; i < num; i++)
        bv->vector[i] = s->vector[i];
    return bv;
}

static void set_libera(BitVector *s) {
    free(s->vector);
    free(s);
}

/* mostra o conjunto em hexadecimal, em grupos de 4 digitos */
static void set_imprime(char *titulo, BitVector *s) {
    int k, num = num_ints(s->max);
    printf("%-22s = ", titulo);
    for (k = num - 1; k >= 0; k--)
        printf("%04X %04X ", (s->vector[k] >> 16) & 0xFFFF,
                              s->vector[k] & 0xFFFF);
    printf("\n");
}

/* ============================================================
 *                    funcoes pedidas
 * ============================================================ */

/* a) remove o i-esimo elemento do conjunto s
      (desliga o bit: AND com o complemento da mascara) */
BitVector *set_remove(BitVector *s, int i) {
    int bits = sizeof(int) * 8;
    BitVector *t = set_copia(s);          /* nao corrompe s */
    t->vector[i / bits] &= ~(1 << (i % bits));
    return t;
}

/* b) inclui o i-esimo elemento no conjunto s
      (liga o bit: OR com a mascara) */
BitVector *set_inclui(BitVector *s, int i) {
    int bits = sizeof(int) * 8;
    BitVector *t = set_copia(s);          /* nao corrompe s */
    t->vector[i / bits] |= (1 << (i % bits));
    return t;
}

/* c) diferenca: elementos de r que NAO estao em s
      (interseccao de r com o complemento de s: r & ~s) */
BitVector *set_diferenca(BitVector *r, BitVector *s) {
    int i, minab = (r->max < s->max) ? r->max : s->max;
    int num = num_ints(minab);
    BitVector *t = set_cria(minab);
    for (i = 0; i < num; i++)
        t->vector[i] = r->vector[i] & ~s->vector[i];
    return t;
}

/* d) uniao: elementos de r OU de s  (r | s) */
BitVector *set_uniao(BitVector *r, BitVector *s) {
    int i, minab = (r->max < s->max) ? r->max : s->max;
    int num = num_ints(minab);
    BitVector *t = set_cria(minab);
    for (i = 0; i < num; i++)
        t->vector[i] = r->vector[i] | s->vector[i];
    return t;
}

/* ============================================================
 *                         teste
 * ============================================================ */

int main(void) {
    BitVector *s, *r, *a, *b, *c, *d, *tmp;

    /* s = 0000 00FA 11FF AABB CCDD EEFF */
    s = set_cria(80);
    s->vector[2] = 0x000000FA;
    s->vector[1] = 0x11FFAABB;
    s->vector[0] = 0xCCDDEEFF;

    /* r = 05EE 00F1 0011 4455 0000 3377 */
    r = set_cria(80);
    r->vector[2] = 0x05EE00F1;
    r->vector[1] = 0x00114455;
    r->vector[0] = 0x00003377;

    printf("Conjuntos iniciais:\n");
    set_imprime("s", s);
    set_imprime("r", r);

    /* (a) remover as posicoes 30, 39 e 73 de s */
    printf("\n(a) set_remove - remover 30, 39 e 73 de s:\n");
    tmp = set_remove(s, 30);
    a   = set_remove(tmp, 39); set_libera(tmp);
    tmp = a;
    a   = set_remove(tmp, 73); set_libera(tmp);
    set_imprime("s sem 30,39,73", a);
    set_imprime("s (intacto)", s);

    /* (b) reincluir 30, 39 e 73 no resultado de (a) */
    printf("\n(b) set_inclui - reincluir 30, 39 e 73:\n");
    tmp = set_inclui(a, 30);
    b   = set_inclui(tmp, 39); set_libera(tmp);
    tmp = b;
    b   = set_inclui(tmp, 73); set_libera(tmp);
    set_imprime("apos reinclusao", b);

    /* (c) diferenca s - r */
    printf("\n(c) set_diferenca - s menos r:\n");
    c = set_diferenca(s, r);
    set_imprime("s - r", c);

    /* (d) uniao r com s */
    printf("\n(d) set_uniao - r unido com s:\n");
    d = set_uniao(r, s);
    set_imprime("r U s", d);

    set_libera(s); set_libera(r);
    set_libera(a); set_libera(b); set_libera(c); set_libera(d);
    return 0;
}
