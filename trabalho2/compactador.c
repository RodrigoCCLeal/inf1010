/*2º. Trabalho de EDA – Compactação e descompactação de arquivos texto
Descrição: O algoritmo de Huffman permite comprimir informação sem perda através da
recodificação dos seus bytes. O algoritmo consiste de uma série de passos nos quais é possível
construir a codificação (fluxo de bits comprimido) que substitui o fluxo de bits original.
O princípio do algoritmo é de gerar códigos mais curtos para caracteres mais frequentes e
códigos maiores para os caracteres menos frequentes.
Um primeiro algoritmo de compactação poderia ser implementado da seguinte forma:
onde:
e assim sucessivamente, ...
O caractere corresponderia ao código de 5 bits
O caractere de corresponderia ao código de 5 bits
Desta forma, este algoritmo de compactação iria ganhar 3 bits para cada caractere de 8 bits,
onde um arquivo original de 1K bytes, seria comprimido para 640 bytes.
Assim, o texto teria a representação em bits como:
0100000101000010010100100100000101000011010000010100010001000001010000100101
A B R A C A D A B
00100100000100100001 - totalizando 12 x 8 = 96 bits
R A !
E o arquivo comprimido teria a seguinte representação em bits:
000000000110001000000001000000000110000000001100010000011111-totalizando 60 bits
A B R A C A D A B R A !
Um segundo algoritmo, mais eficiente na compactação, foi proposto por Huffman. Este
algoritmo permite comprimir informação, sem perda, também através da recodificação dos seus
bytes. O algoritmo consiste de uma série de passos nos quais é possível construir a codificação
(fluxo de bits comprimido) que substitui o fluxo de bits original. O princípio do algoritmo é de
gerar códigos mais curtos para caracteres mais frequentes e códigos maiores para os
caracteres menos frequentes (pesquisar sobre árvores Trie ou árvore de prefixos).
De forma geral, o algoritmo consiste em construir um histograma dos caracteres utilizados no
texto em ordem de frequência. Vamos supor que o texto seja e o histograma
Assim, o histograma seria:
ocorre 5 vezes
ocorre 2 vezes
ocorre 2 vezes
ocorre 1 vez
ocorre 1 vez
Caractere ocorre 1 vez
e (aqui representado como trocar caracter a caracter o código ASC estendido, de 8 bits
Huffman propôs usar uma árvore de prefixos, onde os prefixos da representação em bits são
únicos.
Exemplo da árvore de prefixos e respectiva representação dos caracteres em bits:
Desta forma, a representação interna do arquivo comprimido fica:
0111110010110100011111001010 - totalizando 28 bits
A B RA CA DA B RA !
O trabalho de EDA consiste em implementar um compactador e um descompactador de
arquivos de texto com base no algoritmo de Huffman.
Para teste do algoritmo, pode ser usado um arquivo contendo a seguinte frase: “AS
ESTRUTURAS DE DADOS SAO FUNDAMENTAIS PARA A ORGANIZACAO E A MANIPULACAO
EFICIENTE DAS INFORMACOES”.
Exiba a tabela obtida. Esta saída deve fazer parte do relatório.
Para compactar e descompacatar o arquivo implemente uma tabela contendo o caractere e o
número de ocorrências do caractere no texto e outra contendo o caractere, o código binário
correspondente e o seu tamanho em bits.
Siga as orientações de entrega de laboratórios descrita no EAD.
Atenção: Trabalhos entregues com atraso sofrerão perda de 10% de sua nota por cada dia após
o prazo de entrega.
O arquivo a ser compactado e descompactado para verificação da correção do seu programa
está no EAD.*/

/* Carlos Eduardo Pimentel Bernardo - 2510866 */
/* Rodrigo Carvalho de Castro Leal - */


#include <stdio.h>
#include <stdlib.h>

/* Definindo o tipo para os nós da árvore de Huffman */
typedef struct no t_no;
struct no {
    char caractere;
    int ocorrencias;
    t_no* esquerdo;   // filho esquerdo (bit 0)
    t_no* direito;    // filho direito (bit 1)
    t_no* proximo;    // para a lista de ocorrências
};

typedef struct {
    char codigo[256]; // código binário correspondente
    int tamanho;      // tamanho do código em bits
} t_codigo;

t_codigo* tabela_codigos[256]; // tabela de códigos para cada caractere

/* Protótipos das funções */
t_no* cria_no(char caractere);
t_no* cria_lista_ocorrencias(char* nome_arquivo);
void imprime_lista(t_no* lista);


/* Função para criar um novo nó da árvore de Huffman */
t_no* cria_no(char caractere) {
    t_no* novo = (t_no*)malloc(sizeof(t_no));
    novo->caractere = caractere;
    novo->ocorrencias = 1;
    novo->esquerdo = NULL;
    novo->direito = NULL;
    return novo;
}


/* Função para criar a lista de ocorrências dos caracteres no texto sem ordem ainda */
t_no* cria_lista_ocorrencias(char* nome_arquivo) {
    t_no* lista = NULL;
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }
    char caractere_atual;
    while ((caractere_atual = fgetc(arquivo)) != EOF) {
        t_no* atual = lista;
        int encontrado = 0;
        while (atual != NULL) {
            if (atual->caractere == caractere_atual) {
                atual->ocorrencias++;
                encontrado = 1;
                break;
            }
            atual = atual->proximo;
        }
        if (!encontrado) {
            t_no* novo = cria_no(caractere_atual);
            novo->proximo = lista;
            lista = novo;
        }
    }
    fclose(arquivo);
    return lista;
}

/* Função para organizar a lista de ocorrências em ordem crescente de ocorrências */
t_no* organiza_lista_ordenada(t_no* lista) {
    if (lista == NULL || lista->proximo == NULL) {
        return lista;
    }
    t_no* nova_lista = NULL;
    t_no* atual = lista;
    while (atual != NULL) {
        t_no* proximo = atual->proximo;
        if (nova_lista == NULL || atual->ocorrencias < nova_lista->ocorrencias) {
            atual->proximo = nova_lista;
            nova_lista = atual;
        } else {
            t_no* temp = nova_lista;
            while (temp->proximo != NULL && temp->proximo->ocorrencias < atual->ocorrencias) {
                temp = temp->proximo;
            }
            atual->proximo = temp->proximo;
            temp->proximo = atual;
        }
        atual = proximo;
    }
    return nova_lista;
}

/* Função para imprimir a lista de ocorrências */
void imprime_lista(t_no* lista) {
    t_no* atual = lista;
    printf("Caractere | Ocorrencias\n");
    printf("-------------------------\n");
    while (atual != NULL) {
        printf("   %c     |     %d\n", atual->caractere, atual->ocorrencias);
        atual = atual->proximo;
    }
}

t_no* controi_arvore(t_no* lista) {
    while (lista != NULL && lista->proximo != NULL) {
        // 1. Retira o primeiro nó (menor frequência)
        t_no* esquerdo = lista;
        lista = lista->proximo;
        esquerdo->proximo = NULL;

        // 2. Retira o segundo nó
        t_no* direito = lista;
        lista = lista->proximo;
        direito->proximo = NULL;

        // 3. Cria o nó pai
        t_no* pai = (t_no*)malloc(sizeof(t_no));
        pai->caractere = '\0';
        pai->ocorrencias = esquerdo->ocorrencias + direito->ocorrencias;
        pai->esquerdo = esquerdo;
        pai->direito = direito;
        pai->proximo = NULL;

        // 4. Insere o nó pai de volta na lista ordenada
        if (lista == NULL || pai->ocorrencias < lista->ocorrencias) {
            pai->proximo = lista;
            lista = pai;
        } else {
            t_no* temp = lista;
            while (temp->proximo != NULL && temp->proximo->ocorrencias < pai->ocorrencias) {
                temp = temp->proximo;
                
            }
            pai->proximo = temp->proximo;
            temp->proximo = pai;
        }
    }
    return lista;
}

void gera_codigos(t_no* raiz, int* codigo, int tamanho) {
    if (raiz == NULL) return;

    // Se é folha (tem caractere), imprime o código
    if (raiz->esquerdo == NULL && raiz->direito == NULL) {
        printf("  %c  |  ", raiz->caractere);
        tabela_codigos[raiz->caractere] = (t_codigo*)malloc(sizeof(t_codigo));
        tabela_codigos[raiz->caractere]->tamanho = tamanho;
        for (int i = 0; i < tamanho; i++) {
            printf("%d", codigo[i]);
            tabela_codigos[raiz->caractere]->codigo[i] = codigo[i];
        }
        printf("  |  %d bits\n", tamanho);
        return;
    }

    // Vai para a esquerda: adiciona 0
    codigo[tamanho] = 0;
    gera_codigos(raiz->esquerdo, codigo, tamanho + 1);

    // Vai para a direita: adiciona 1
    codigo[tamanho] = 1;
    gera_codigos(raiz->direito, codigo, tamanho + 1);
}

void compacta(t_no* raiz, char* arquivo_entrada, char* arquivo_saida) {

    // Gera a tabela de códigos a partir da árvore de Huffman
    gera_codigos(raiz, (int*)malloc(256 * sizeof(int)), 0);

    // Abre o arquivo de saída em modo binário ("wb")
    FILE* saida = fopen(arquivo_saida, "wb");
    if (saida == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        return;
    }
    // Lê o arquivo de entrada caractere a caractere
    FILE* entrada = fopen(arquivo_entrada, "r");
    if (entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        fclose(saida);
        return;
    }

    unsigned char buffer = 0;
    int bits_no_buffer = 0;

    while (!feof(entrada)) {
        char caractere = fgetc(entrada);
        if (caractere == EOF) break;

        // Para cada caractere, pega o código da tabela
        t_codigo* codigo = tabela_codigos[caractere];
        if (codigo == NULL) {
            printf("Caractere %c não encontrado na tabela de códigos.\n", caractere);
            continue;
        }

        // Para cada bit '0' ou '1' do código:
        for (int i = 0; i < codigo->tamanho; i++) {
            int bit = codigo->codigo[i];
        buffer = (buffer << 1) | bit;  // empurra o bit para dentro
        bits_no_buffer++;
        
        if (bits_no_buffer == 8) {
            fwrite(&buffer, 1, 1, saida); // escreve 1 byte no arquivo
            buffer = 0;
            bits_no_buffer = 0;
            }
        }
    }
    fclose(entrada);
    // Se sobrou bits no buffer, preenche com zeros e escreve
    if (bits_no_buffer > 0) {
        buffer <<= (8 - bits_no_buffer); // preenche os bits restantes com zeros
        fwrite(&buffer, 1, 1, saida);
    }
    fclose(saida);
}

void descompacta(t_no* raiz, char* arquivo_entrada, char* arquivo_saida) {
    // Abre o arquivo de entrada em modo binário ("rb")
    FILE* entrada = fopen(arquivo_entrada, "rb");
    if (entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return;
    }
    // Abre o arquivo de saída em modo texto ("w")
    FILE* saida = fopen(arquivo_saida, "w");
    if (saida == NULL) {
        printf("Erro ao abrir o arquivo de saída.\n");
        fclose(entrada);
        return;
    }

    t_no* atual = raiz; // começa na raiz da árvore
    unsigned char byte;
    
    while (fread(&byte, 1, 1, entrada) == 1) { // lê um byte do arquivo
        for (int i = 7; i >= 0; i--) { // processa cada bit do byte
            int bit = (byte >> i) & 1; // extrai o bit
            if (bit == 0) {
                atual = atual->esquerdo; // vai para a esquerda
            } else {
                atual = atual->direito; // vai para a direita
            }
            if (atual->esquerdo == NULL && atual->direito == NULL) { // se é folha
                fputc(atual->caractere, saida); // escreve o caractere no arquivo de saída
                atual = raiz; // volta para a raiz
            }
        }
    }
    fclose(entrada);
    fclose(saida);
}

int main (void)
{
    t_no* lista = cria_lista_ocorrencias("teste1.txt");
    if (lista != NULL) {
        imprime_lista(lista);
    }
    lista = organiza_lista_ordenada(lista);
    if (lista != NULL) {
        printf("\nLista organizada por ocorrencias:\n");
        imprime_lista(lista);
    }
    printf("\nArvore de Huffman:\n");
    t_no* raiz = controi_arvore(lista);
    int codigo[256] = {0}; // tamanho máximo possível de um código
    printf("\nCaractere | Codigo | Tamanho\n");
    printf("--------------------------------\n");
    gera_codigos(raiz, codigo, 0);
    compacta(raiz, "teste1.txt", "teste1.huff");
    descompacta(raiz, "teste1.huff", "teste1_descompactado.txt");
    return 0;
}