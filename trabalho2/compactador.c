/* INF1010 - Trabalho 2 */

/* Carlos Eduardo Pimentel Bernardo - 2510866 */
/* Rodrigo Carvalho de Castro Leal - 2510091 */


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

/* Função para construir a árvore de Huffman a partir da lista de ocorrências organizada */
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
        pai->ocorrencias = esquerdo->ocorrencias + direito->ocorrencias; // soma das ocorrências dos filhos 
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

/* Função para compactar um arquivo usando a árvore de Huffman */
void compacta(t_no* raiz, char* arquivo_entrada, char* arquivo_saida) {

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
    unsigned char placeholder = 0;
    fwrite(&placeholder, 1, 1, saida); // reserva espaço para o cabeçalho

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
    unsigned char bits_uteis = (bits_no_buffer == 0) ? 8 : bits_no_buffer;
    fseek(saida, 0, SEEK_SET);
    fwrite(&bits_uteis, 1, 1, saida);

    fclose(saida);
}

/* Função para descompactar um arquivo usando a árvore de Huffman*/
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

    unsigned char bits_uteis_ultimo_byte;
    fread(&bits_uteis_ultimo_byte, 1, 1, entrada);

    // Para saber o tamanho total do arquivo (sem o cabeçalho):
    fseek(entrada, 0, SEEK_END);
    long tamanho = ftell(entrada) - 1; // -1 pelo byte de cabeçalho
    fseek(entrada, 1, SEEK_SET);       // volta para depois do cabeçalho

    t_no* atual = raiz; // começa na raiz da árvore
    unsigned char byte;
    
    long bytes_lidos = 0;
    while (fread(&byte, 1, 1, entrada) == 1) {
        bytes_lidos++;
        int bits_neste_byte = (bytes_lidos == tamanho) ? bits_uteis_ultimo_byte : 8;
        
        for (int i = 7; i > (7 - bits_neste_byte); i--) { // só processa os bits úteis
            int bit = (byte >> i) & 1;
            if (bit == 0)
                atual = atual->esquerdo;
            else
                atual = atual->direito;
            if (atual->esquerdo == NULL && atual->direito == NULL) {
                fputc(atual->caractere, saida);
                atual = raiz;
            }
        }
    }
    fclose(entrada);
    fclose(saida);
}

/* Função que realiza todo o processo de compactação e descompactação e exibir etapas*/
void realiza_processo (char* arquivo_entrada, char* arquivo_saida_compactado, char* arquivo_saida_descompactado) {

    /* Lendo o arquivo e criando a lista de ocorrências desordenada */
    t_no* lista = cria_lista_ocorrencias(arquivo_entrada);
    if (lista != NULL) {
        imprime_lista(lista);
    }

    /* Organizando a lista por ocorrências */
    lista = organiza_lista_ordenada(lista);
    if (lista != NULL) {
        printf("\nLista organizada por ocorrencias:\n");
        imprime_lista(lista);
    }

    /* Construindo a árvore de Huffman com base na lista organizada */
    printf("\nArvore de Huffman:\n");
    t_no* raiz = controi_arvore(lista);

    int codigo[256] = {0}; // tamanho máximo possível de um código

    printf("\nCaractere | Codigo | Tamanho\n");
    printf("--------------------------------\n");

    /* Gerando os códigos de Huffman para cada caractere */
    gera_codigos(raiz, codigo, 0);

    /* Compactando o arquivo */
    compacta(raiz, arquivo_entrada, arquivo_saida_compactado);

    /* Descompactando o arquivo */
    descompacta(raiz, arquivo_saida_compactado, arquivo_saida_descompactado);

}

int main (void)
{
    realiza_processo("teste1.txt", "teste1_compactado.txt", "teste1_descompactado.txt");

    realiza_processo("textoCompactadorDescompactador.txt", "textoCompactadorDescompactador_compactado.txt", "textoCompactadorDescompactador_descompactado.txt");

    return 0;
}