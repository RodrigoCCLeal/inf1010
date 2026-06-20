#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * Lab 5 INF1010 - TAD Conjunto (Set) como vetor de bits
 *
 * Convencao adotada (a mais simples):
 *   - o elemento i corresponde ao bit i, contado a partir do
 *     bit menos significativo (LSB);
 *   - bit i fica na palavra v[i/32], na posicao (i%32);
 *   - v[0] guarda os bits 0..31 (palavra menos significativa).
 *
 * Conjunto de 80 bits -> 3 palavras de 32 bits (96 bits de
 * armazenamento; os bits acima de 79 servem so de folga).
 * ============================================================ */

#define NBITS  80                          /* tamanho logico    */
#define WBITS  32                          /* bits por palavra  */
#define NW     ((NBITS + WBITS - 1)/WBITS) /* nro de palavras=3 */

struct bitvector {
    unsigned int v[NW];   /* v[0] = palavra menos significativa */
};
typedef struct bitvector BitVector;

/* ---- auxiliares (necessarias para criar/testar) ---- */

static BitVector * set_cria(void) {          /* conjunto vazio */
    BitVector *s = malloc(sizeof(BitVector));
    int k;
    for (k = 0; k < NW; k++) s->v[k] = 0u;
    return s;
}

static BitVector * set_copia(BitVector *s) { /* copia de s */
    BitVector *c = malloc(sizeof(BitVector));
    int k;
    for (k = 0; k < NW; k++) c->v[k] = s->v[k];
    return c;
}

static void set_libera(BitVector *s) { free(s); }

static void set_imprime(const char *nome, BitVector *s) {
    int k;
    printf("%-22s = ", nome);
    for (k = NW - 1; k >= 0; k--)            /* palavra mais signif. 1o */
        printf("%04X %04X ", (s->v[k] >> 16) & 0xFFFF, s->v[k] & 0xFFFF);
    printf("\n");
}

/* ============================================================
 *                    funcoes pedidas
 * ============================================================ */

/* a) remove o i-esimo elemento do conjunto s */
BitVector * set_remove(BitVector *s, int i) {
    BitVector *t = set_copia(s);             /* nao corrompe s  */
    if (i >= 0 && i < NBITS)
        t->v[i / WBITS] &= ~(1u << (i % WBITS));
    return t;
}

/* b) inclui o i-esimo elemento no conjunto s */
BitVector * set_inclui(BitVector *s, int i) {
    BitVector *t = set_copia(s);             /* nao corrompe s  */
    if (i >= 0 && i < NBITS)
        t->v[i / WBITS] |= (1u << (i % WBITS));
    return t;
}

/* c) diferenca: elementos de r que NAO estao em s  (t = r & ~s) */
BitVector * set_diferenca(BitVector *r, BitVector *s) {
    BitVector *t = set_cria();
    int k;
    for (k = 0; k < NW; k++)
        t->v[k] = r->v[k] & ~s->v[k];
    return t;
}

/* d) uniao: elementos de r OU de s  (t = r | s) */
BitVector * set_uniao(BitVector *r, BitVector *s) {
    BitVector *t = set_cria();
    int k;
    for (k = 0; k < NW; k++)
        t->v[k] = r->v[k] | s->v[k];
    return t;
}

/* ============================================================
 *                         teste
 * ============================================================ */

int main(void) {
    BitVector *s, *r, *a, *b, *c, *d, *tmp;

    /* s = 0000 00FA 11FF AABB CCDD EEFF */
    s = set_cria();
    s->v[2] = 0x000000FAu;
    s->v[1] = 0x11FFAABBu;
    s->v[0] = 0xCCDDEEFFu;

    /* r = 05EE 00F1 0011 4455 0000 3377 */
    r = set_cria();
    r->v[2] = 0x05EE00F1u;
    r->v[1] = 0x00114455u;
    r->v[0] = 0x00003377u;

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

    /* (b) incluir de volta 30, 39 e 73 no resultado de (a) */
    printf("\n(b) set_inclui - reincluir 30, 39 e 73:\n");
    tmp = set_inclui(a, 30);
    b   = set_inclui(tmp, 39); set_libera(tmp);
    tmp = b;
    b   = set_inclui(tmp, 73); set_libera(tmp);
    set_imprime("apos reinclusao", b);

    /* (c) diferenca s - r (elementos de s que nao estao em r) */
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
