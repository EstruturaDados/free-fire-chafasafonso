/*
  mochila.c
  Sistema comparativo: vetor (lista sequencial) vs lista encadeada
  - Item: nome, tipo, quantidade
  - Operações: inserir, remover, listar, buscar sequencial (ambas)
  - Vetor: ordenar (selection sort) e buscar binária (após ordenação)
  - Contadores de comparações para cada tipo de busca + para a ordenação
  - Medição de tempo (clock) para comparação empírica
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20

/* ---------------------------
   Definições de tipos
   --------------------------- */
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No *proximo;
} No;

/* ---------------------------
   Utilitários
   --------------------------- */
// Limpa o buffer do stdin (útil após scanf)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Imprime uma linha separadora */
void separador() {
    printf("--------------------------------------------------\n");
}

/* ---------------------------
   Funções para VETOR (lista sequencial)
   --------------------------- */
void inserirItemVetor(Item vetor[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("⚠ Mochila (vetor) cheia. Remova itens antes de inserir.\n");
        return;
    }
    Item novo;
    printf("Digite o nome do item: ");
    fgets(novo.nome, TAM_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Digite o tipo do item (arma, municao, cura, etc.): ");
    fgets(novo.tipo, TAM_TIPO, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Entrada invalida para quantidade.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    vetor[*total] = novo;
    (*total)++;
    printf("✅ Item \"%s\" inserido no vetor (mochila).\n", novo.nome);
}

void removerItemVetor(Item vetor[], int *total) {
    if (*total == 0) {
        printf("⚠ A mochila (vetor) está vazia.\n");
        return;
    }
    char nome[TAM_NOME];
    printf("Digite o nome do item a remover: ");
    fgets(nome, TAM_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int pos = -1;
    for (int i = 0; i < *total; i++) {
        if (strcmp(vetor[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("❌ Item \"%s\" nao encontrado no vetor.\n", nome);
        return;
    }
    for (int i = pos; i < *total - 1; i++) {
        vetor[i] = vetor[i + 1];
    }
    (*total)--;
    printf("🗑️ Item \"%s\" removido do vetor.\n", nome);
}

void listarVetor(const Item vetor[], int total) {
    separador();
    printf("Itens na mochila (vetor) — total: %d\n", total);
    if (total == 0) {
        printf("(vazia)\n");
        separador();
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
    separador();
}

/* busca sequencial no vetor
   - incrementa *comparacoes a cada comparação de nome (strcmp)
   - retorna índice se encontrado, -1 caso contrário
*/
int buscarSequencialVetor(const Item vetor[], int total, const char *nome, long *comparacoes) {
    for (int i = 0; i < total; i++) {
        (*comparacoes)++;
        if (strcmp(vetor[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* ordena vetor por nome (selection sort) e conta comparações de nome.
   - *compOrdenacao é incrementado a cada strcmp realizada
*/
void ordenarVetorSelection(Item vetor[], int total, long *compOrdenacao) {
    for (int i = 0; i < total - 1; i++) {
        int min = i;
        for (int j = i + 1; j < total; j++) {
            (*compOrdenacao)++;
            if (strcmp(vetor[j].nome, vetor[min].nome) < 0) {
                min = j;
            }
        }
        if (min != i) {
            Item tmp = vetor[i];
            vetor[i] = vetor[min];
            vetor[min] = tmp;
        }
    }
}

/* busca binária no vetor (vetor deve estar ordenado)
   - *comparacoes é incrementado a cada strcmp (uma comparação por iteração)
   - retorna índice se encontrado, -1 caso contrário
*/
int buscarBinariaVetor(const Item vetor[], int total, const char *nome, long *comparacoes) {
    int left = 0, right = total - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        (*comparacoes)++;
        int cmp = strcmp(nome, vetor[mid].nome);
        if (cmp == 0) return mid;
        else if (cmp < 0) right = mid - 1;
        else left = mid + 1;
    }
    return -1;
}

/* ---------------------------
   Funções para LISTA ENCADEADA
   --------------------------- */
void inicializarLista(No **head) {
    *head = NULL;
}

void inserirLista(No **head) {
    Item novo;
    printf("Digite o nome do item: ");
    fgets(novo.nome, TAM_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Digite o tipo do item (arma, municao, cura, etc.): ");
    fgets(novo.tipo, TAM_TIPO, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Entrada invalida para quantidade.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    No *novoNo = (No*) malloc(sizeof(No));
    if (!novoNo) {
        printf("Erro de alocacao.\n");
        return;
    }
    novoNo->dados = novo;
    novoNo->proximo = NULL;

    if (*head == NULL) {
        *head = novoNo;
    } else {
        No *aux = *head;
        while (aux->proximo != NULL) aux = aux->proximo;
        aux->proximo = novoNo;
    }
    printf("✅ Item \"%s\" inserido na lista encadeada.\n", novo.nome);
}

void removerLista(No **head) {
    if (*head == NULL) {
        printf("⚠ A mochila (lista) está vazia.\n");
        return;
    }
    char nome[TAM_NOME];
    printf("Digite o nome do item a remover: ");
    fgets(nome, TAM_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No *atual = *head, *anterior = NULL;
    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) {
        printf("❌ Item \"%s\" nao encontrado na lista.\n", nome);
        return;
    }
    if (anterior == NULL) { // remover o primeiro
        *head = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    free(atual);
    printf("🗑️ Item \"%s\" removido da lista.\n", nome);
}

void listarLista(const No *head) {
    separador();
    printf("Itens na mochila (lista encadeada):\n");
    if (head == NULL) {
        printf("(vazia)\n");
        separador();
        return;
    }
    int i = 1;
    for (const No *aux = head; aux != NULL; aux = aux->proximo) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i++, aux->dados.nome, aux->dados.tipo, aux->dados.quantidade);
    }
    separador();
}

/* busca sequencial na lista encadeada
   - *comparacoes incrementado por cada strcmp
   - retorna ponteiro para o nó encontrado (NULL se nao encontrado)
*/
No* buscarSequencialLista(const No *head, const char *nome, long *comparacoes) {
    const No *aux = head;
    while (aux != NULL) {
        (*comparacoes)++;
        if (strcmp(aux->dados.nome, nome) == 0) {
            // cast necessário para retornar No* não-const; chamador deve ter cuidado
            return (No*) aux;
        }
        aux = aux->proximo;
    }
    return NULL;
}

/* libera toda a lista (uso ao finalizar programa ou ao descartar) */
void liberarLista(No **head) {
    No *aux = *head;
    while (aux != NULL) {
        No *prox = aux->proximo;
        free(aux);
        aux = prox;
    }
    *head = NULL;
}

/* ---------------------------
   Menus e comparação empírica
   --------------------------- */

/* Menu específico para operações sobre o VETOR */
void menuVetor(Item vetor[], int *total) {
    int op;
    int ordenado = 0; // flag indicando se vetor está ordenado por nome
    do {
        printf("\n[ MENU - VETOR ]\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar (sequencial)\n");
        printf("5 - Ordenar vetor (selection sort)\n");
        printf("6 - Buscar (binaria) - exige vetor ordenado\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        if (scanf("%d", &op) != 1) { limparBuffer(); op = -1; }
        limparBuffer();

        switch(op) {
            case 1:
                inserirItemVetor(vetor, total);
                ordenado = 0;
                break;
            case 2:
                removerItemVetor(vetor, total);
                ordenado = 0;
                break;
            case 3:
                listarVetor(vetor, *total);
                break;
            case 4: {
                if (*total == 0) { printf("Mochila vazia.\n"); break; }
                char nome[TAM_NOME];
                printf("Nome a buscar (sequencial): ");
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                long comps = 0;
                clock_t t0 = clock();
                int idx = buscarSequencialVetor(vetor, *total, nome, &comps);
                clock_t t1 = clock();
                double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC;
                if (idx >= 0) {
                    printf("🔍 Encontrado: %s | Tipo: %s | Quantidade: %d (indice %d)\n",
                           vetor[idx].nome, vetor[idx].tipo, vetor[idx].quantidade, idx);
                } else printf("❌ Nao encontrado.\n");
                printf("Comparacoes (sequencial): %ld | Tempo: %.6fs\n", comps, tempo);
                break;
            }
            case 5: {
                if (*total <= 1) { printf("Nada para ordenar (0 ou 1 item).\n"); break; }
                long compsOrden = 0;
                clock_t t0 = clock();
                ordenarVetorSelection(vetor, *total, &compsOrden);
                clock_t t1 = clock();
                double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC;
                ordenado = 1;
                printf("✅ Vetor ordenado. Comparacoes durante ordenacao: %ld | Tempo: %.6fs\n",
                       compsOrden, tempo);
                break;
            }
            case 6: {
                if (*total == 0) { printf("Mochila vazia.\n"); break; }
                if (!ordenado) {
                    printf("O vetor nao esta ordenado. Vou ordenar antes da busca binaria.\n");
                    long compsOrden = 0;
                    ordenarVetorSelection(vetor, *total, &compsOrden);
                    printf("(ordenacao automática realizada, comparacoes: %ld)\n", compsOrden);
                    ordenado = 1;
                }
                char nome[TAM_NOME];
                printf("Nome a buscar (binaria): ");
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                long compsBin = 0;
                clock_t t0 = clock();
                int idx = buscarBinariaVetor(vetor, *total, nome, &compsBin);
                clock_t t1 = clock();
                double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC;
                if (idx >= 0) {
                    printf("🔍 Encontrado (binaria): %s | Tipo: %s | Quantidade: %d (indice %d)\n",
                           vetor[idx].nome, vetor[idx].tipo, vetor[idx].quantidade, idx);
                } else printf("❌ Nao encontrado (binaria).\n");
                printf("Comparacoes (binaria): %ld | Tempo: %.6fs\n", compsBin, tempo);
                break;
            }
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

/* Menu para operações sobre a LISTA ENCADEADA */
void menuLista(No **head) {
    int op;
    do {
        printf("\n[ MENU - LISTA ENCADEADA ]\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar (sequencial)\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        if (scanf("%d", &op) != 1) { limparBuffer(); op = -1; }
        limparBuffer();

        switch(op) {
            case 1:
                inserirLista(head);
                break;
            case 2:
                removerLista(head);
                break;
            case 3:
                listarLista(*head);
                break;
            case 4: {
                if (*head == NULL) { printf("Mochila (lista) vazia.\n"); break; }
                char nome[TAM_NOME];
                printf("Nome a buscar (sequencial na lista): ");
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                long comps = 0;
                clock_t t0 = clock();
                No *achado = buscarSequencialLista(*head, nome, &comps);
                clock_t t1 = clock();
                double tempo = (double)(t1 - t0) / CLOCKS_PER_SEC;
                if (achado != NULL) {
                    printf("🔍 Encontrado: %s | Tipo: %s | Quantidade: %d\n",
                           achado->dados.nome, achado->dados.tipo, achado->dados.quantidade);
                } else printf("❌ Nao encontrado na lista.\n");
                printf("Comparacoes (sequencial - lista): %ld | Tempo: %.6fs\n", comps, tempo);
                break;
            }
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);
}

/* Comparacao empírica de desempenho:
   - pede um nome a ser buscado e numero de repeticoes (N)
   - executa N buscas sequenciais no vetor, N buscas sequenciais na lista,
     e N buscas binarias no vetor (ordenando antes, se necessário)
   - mede tempo total e soma de comparacoes, mostra medias e totais
*/
void compararDesempenho(Item vetor[], int totalVetor, No *head) {
    if (totalVetor == 0 && head == NULL) {
        printf("Ambas as estruturas estao vazias. Insira itens antes de comparar.\n");
        return;
    }
    char nome[TAM_NOME];
    int N;
    printf("Nome a ser buscado para o teste empírico: ");
    fgets(nome, TAM_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    printf("Numero de repeticoes (sugestao 1000 - 100000, dependendo do desempenho): ");
    if (scanf("%d", &N) != 1) { limparBuffer(); printf("Entrada invalida.\n"); return; }
    limparBuffer();
    if (N <= 0) { printf("Numero de repeticoes debe ser > 0.\n"); return; }

    printf("\nIniciando testes com N = %d repeticoes.\n", N);

    /* 1) Busca sequencial no vetor */
    if (totalVetor > 0) {
        long compsTotal = 0;
        clock_t t0 = clock();
        for (int i = 0; i < N; i++) {
            long comps = 0;
            buscarSequencialVetor(vetor, totalVetor, nome, &comps);
            compsTotal += comps;
        }
        clock_t t1 = clock();
        double secs = (double)(t1 - t0) / CLOCKS_PER_SEC;
        printf("\n[VETOR - busca sequencial] Total comparacoes: %ld | Media comparacoes: %.3f | Tempo total: %.6fs | Media por busca: %.9fs\n",
               compsTotal, (double)compsTotal / N, secs, secs / N);
    } else {
        printf("\n[VETOR - busca sequencial] vetor vazio (pule).\n");
    }

    /* 2) Busca sequencial na lista */
    if (head != NULL) {
        long compsTotal = 0;
        clock_t t0 = clock();
        for (int i = 0; i < N; i++) {
            long comps = 0;
            buscarSequencialLista(head, nome, &comps);
            compsTotal += comps;
        }
        clock_t t1 = clock();
        double secs = (double)(t1 - t0) / CLOCKS_PER_SEC;
        printf("\n[LISTA - busca sequencial] Total comparacoes: %ld | Media comparacoes: %.3f | Tempo total: %.6fs | Media por busca: %.9fs\n",
               compsTotal, (double)compsTotal / N, secs, secs / N);
    } else {
        printf("\n[LISTA - busca sequencial] lista vazia (pule).\n");
    }

    /* 3) Busca binaria no vetor (ordena primeiro) */
    if (totalVetor > 0) {
        // Criar uma copia local do vetor para ordenar sem perder a ordem original do usuario
        Item copia[MAX_ITENS];
        for (int i = 0; i < totalVetor; i++) copia[i] = vetor[i];
        long compsOrden = 0;
        ordenarVetorSelection(copia, totalVetor, &compsOrden);
        // executar N buscas binarias
        long compsTotal = 0;
        clock_t t0 = clock();
        for (int i = 0; i < N; i++) {
            long comps = 0;
            buscarBinariaVetor(copia, totalVetor, nome, &comps);
            compsTotal += comps;
        }
        clock_t t1 = clock();
        double secs = (double)(t1 - t0) / CLOCKS_PER_SEC;
        printf("\n[VETOR - busca binaria] (apos ordenacao com %ld comparacoes) Total comparacoes (buscas): %ld | Media comparacoes por busca: %.3f | Tempo total (buscas): %.6fs | Media por busca: %.9fs\n",
               compsOrden, compsTotal, (double)compsTotal / N, secs, secs / N);
    } else {
        printf("\n[VETOR - busca binaria] vetor vazio (pule).\n");
    }

    printf("\nTeste de desempenho concluido.\n");
}

/* ---------------------------
   MAIN - menu principal
   --------------------------- */
int main() {
    Item vetor[MAX_ITENS];
    int totalVetor = 0;
    No *head = NULL;
    inicializarLista(&head);

    int opc;
    do {
        
        printf("1 - Trabalhar com VETOR (lista sequencial)\n");
        printf("2 - Trabalhar com LISTA ENCADEADA\n");
        printf("3 - Comparar \n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) { limparBuffer(); opc = -1; }
        limparBuffer();

        switch(opc) {
            case 1:
                menuVetor(vetor, &totalVetor);
                break;
            case 2:
                menuLista(&head);
                break;
            case 3:
                compararDesempenho(vetor, totalVetor, head);
                break;
            case 0:
                printf("Saindo... liberando recursos.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opc != 0);

    liberarLista(&head);
    return 0;
}
