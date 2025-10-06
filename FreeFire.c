/*
  Torre de Resgate - Módulo de Priorização e Montagem
  - Cadastro de até 20 componentes (nome, tipo, prioridade)
  - Ordenações:
      * Bubble sort por nome (string) -> bubbleSortNome
      * Insertion sort por tipo (string) -> insertionSortTipo
      * Selection sort por prioridade (int) -> selectionSortPrioridade
  - Contagem de comparações e tempo de execução (clock)
  - Busca binária por nome (aplicável apenas após ordenação por nome)
  - Menu interativo e exibição formatada
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMP 20
#define MAX_NOME 30
#define MAX_TIPO 20

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; // 1..10
} Componente;

/* ---------- Utilitários ---------- */

// remove newline final de strings lidas por fgets
void trim_newline(char *s) {
    size_t L = strlen(s);
    if (L == 0) return;
    if (s[L-1] == '\n') s[L-1] = '\0';
}

// imprime vetor de componentes formatado
void mostrarComponentes(Componente comps[], int n) {
    printf("\n%-3s %-30s %-20s %-10s\n", "i", "NOME", "TIPO", "PRIORIDADE");
    printf("-----------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d %-30s %-20s %-10d\n", i, comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
    printf("\n");
}

/* ---------- Ordenações (cada função atualiza *comparacoes) ---------- */

/*
  Bubble Sort por nome (lexicográfico)
  - Conta comparações de string (cada strcmp conta como 1 comparação)
*/
void bubbleSortNome(Componente v[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (strcmp(v[j].nome, v[j+1].nome) > 0) {
                Componente tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
                trocou = 1;
            }
        }
        // otimização: se não trocou, já está ordenado
        if (!trocou) break;
    }
}

/*
  Insertion Sort por tipo (lexicográfico)
  - Conta comparações de string (cada strcmp no while conta como 1)
*/
void insertionSortTipo(Componente v[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente key = v[i];
        int j = i - 1;
        // enquanto j>=0 e v[j].tipo > key.tipo
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(v[j].tipo, key.tipo) > 0) {
                v[j+1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j+1] = key;
    }
}

/*
  Selection Sort por prioridade (menor prioridade primeiro -> prioridade 1 é mais urgente)
  - Conta comparações de inteiros (cada comparação v[j].prioridade < v[min].prioridade conta como 1)
*/
void selectionSortPrioridade(Componente v[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (v[j].prioridade < v[min].prioridade) {
                min = j;
            }
        }
        if (min != i) {
            Componente tmp = v[i];
            v[i] = v[min];
            v[min] = tmp;
        }
    }
}

/* ---------- Busca Binária por nome (após ordenação por nome) ---------- */
/*
  Retorna índice se encontrado, -1 se não.
  Conta comparações em *comparacoes.
  IMPORTANTE: pressupõe ordem lexicográfica crescente por nome.
*/
int buscaBinariaPorNome(Componente v[], int n, const char chave[], long *comparacoes) {
    int l = 0, r = n - 1;
    *comparacoes = 0;
    while (l <= r) {
        int m = l + (r - l) / 2;
        (*comparacoes)++;
        int cmp = strcmp(chave, v[m].nome);
        if (cmp == 0) return m;
        else if (cmp < 0) r = m - 1;
        else l = m + 1;
    }
    return -1;
}

/* ---------- Medição de tempo genérica ---------- */
/*
  Tipo de função de ordenação: void func(Componente[], int, long*)
  medirTempo recebe o ponteiro da função, um vetor (cópia), n e ponteiro onde grava o número de comparações e o tempo em ms.
*/
typedef void (*AlgSort)(Componente[], int, long*);

double medirTempo(AlgSort algoritmo, Componente arr[], int n, long *comparacoes) {
    // fazer cópia local para que a medição não modifique o original
    Componente *tmp = malloc(sizeof(Componente) * n);
    if (!tmp) {
        fprintf(stderr, "Erro de alocacao\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) tmp[i] = arr[i];

    clock_t inicio = clock();
    algoritmo(tmp, n, comparacoes);
    clock_t fim = clock();
    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

    // copiar ordenado de volta para arr (opcional): o chamador talvez queira ver os componentes ordenados.
    for (int i = 0; i < n; i++) arr[i] = tmp[i];

    free(tmp);
    return tempo_ms;
}

/* ---------- Cadastro de componentes ---------- */
int cadastrarComponentes(Componente comps[]) {
    int n = 0;
    char buffer[100];

    printf("Quantos componentes deseja cadastrar? (1-%d): ", MAX_COMP);
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    n = atoi(buffer);
    if (n < 1) n = 0;
    if (n > MAX_COMP) n = MAX_COMP;

    for (int i = 0; i < n; i++) {
        printf("\nComponente %d:\n", i+1);

        printf("  Nome (ex: chip central): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) buffer[0] = '\0';
        trim_newline(buffer);
        strncpy(comps[i].nome, buffer, MAX_NOME-1);
        comps[i].nome[MAX_NOME-1] = '\0';

        printf("  Tipo (ex: controle, suporte, propulsao): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) buffer[0] = '\0';
        trim_newline(buffer);
        strncpy(comps[i].tipo, buffer, MAX_TIPO-1);
        comps[i].tipo[MAX_TIPO-1] = '\0';

        printf("  Prioridade (1-10) (1 = mais urgente): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) buffer[0] = '\0';
        comps[i].prioridade = atoi(buffer);
        if (comps[i].prioridade < 1) comps[i].prioridade = 1;
        if (comps[i].prioridade > 10) comps[i].prioridade = 10;
    }
    return n;
}

/* ---------- Menu e fluxo principal ---------- */
int main() {
    Componente comps[MAX_COMP];
    Componente backup[MAX_COMP]; // cópia original
    int n = 0;
    int ordenado_por_nome = 0; // flag para permitir busca binária
    srand((unsigned) time(NULL));

    printf("=== Torre de Resgate: Sistema de Priorizacao e Montagem ===\n\n");

    // cadastrar
    n = cadastrarComponentes(comps);
    if (n <= 0) {
        printf("Nenhum componente cadastrado. Saindo.\n");
        return 0;
    }

    // guardar backup do estado inicial
    for (int i = 0; i < n; i++) backup[i] = comps[i];

    int opcao;
    do {
        printf("\nMENU:\n");
        printf(" 1 - Mostrar componentes cadastrados\n");
        printf(" 2 - Ordenar por NOME (Bubble Sort)\n");
        printf(" 3 - Ordenar por TIPO (Insertion Sort)\n");
        printf(" 4 - Ordenar por PRIORIDADE (Selection Sort)\n");
        printf(" 5 - Busca binaria por NOME (requer ordenacao por nome)\n");
        printf(" 6 - Restaurar ordem original\n");
        printf(" 0 - Sair\n");
        printf("Escolha: ");

        char buf[32];
        if (!fgets(buf, sizeof(buf), stdin)) break;
        opcao = atoi(buf);

        if (opcao == 1) {
            mostrarComponentes(comps, n);
        } else if (opcao == 2) {
            long compsCount = 0;
            // medir tempo e ordenar (com cópia interna a medirTempo e cópia de volta)
            double t = medirTempo(bubbleSortNome, comps, n, &compsCount);
            printf("\n-- Resultado: Bubble Sort por NOME --\n");
            printf("Comparacoes (strcmp): %ld\n", compsCount);
            printf("Tempo: %.3f ms\n", t);
            mostrarComponentes(comps, n);
            ordenado_por_nome = 1;
        } else if (opcao == 3) {
            long compsCount = 0;
            double t = medirTempo(insertionSortTipo, comps, n, &compsCount);
            printf("\n-- Resultado: Insertion Sort por TIPO --\n");
            printf("Comparacoes (strcmp): %ld\n", compsCount);
            printf("Tempo: %.3f ms\n", t);
            mostrarComponentes(comps, n);
            ordenado_por_nome = 0;
        } else if (opcao == 4) {
            long compsCount = 0;
            double t = medirTempo(selectionSortPrioridade, comps, n, &compsCount);
            printf("\n-- Resultado: Selection Sort por PRIORIDADE --\n");
            printf("Comparacoes (int): %ld\n", compsCount);
            printf("Tempo: %.3f ms\n", t);
            mostrarComponentes(comps, n);
            ordenado_por_nome = 0;
        } else if (opcao == 5) {
            if (!ordenado_por_nome) {
                printf("\nAviso: para usar a busca binaria por nome, a lista deve estar ordenada por NOME.\n");
                printf("Execute a opção 2 (Bubble Sort por nome) antes.\n");
            } else {
                char chave[100];
                printf("Digite o NOME do componente chave a procurar: ");
                if (!fgets(chave, sizeof(chave), stdin)) chave[0] = '\0';
                trim_newline(chave);
                long compsCount = 0;
                int idx = buscaBinariaPorNome(comps, n, chave, &compsCount);
                printf("\n-- Busca Binaria por NOME --\n");
                printf("Comparacoes (strcmp): %ld\n", compsCount);
                if (idx >= 0) {
                    printf("Componente encontrado no indice %d:\n", idx);
                    printf("  Nome: %s\n  Tipo: %s\n  Prioridade: %d\n", comps[idx].nome, comps[idx].tipo, comps[idx].prioridade);
                    printf("\n>>> Componente-chave presente: ativacao possível!\n");
                } else {
                    printf("Componente '%s' NAO encontrado.\n", chave);
                    printf("\n>>> Componente-chave ausente: verifique cadastro ou critério de ordenacao.\n");
                }
            }
        } else if (opcao == 6) {
            // restaurar original
            for (int i = 0; i < n; i++) comps[i] = backup[i];
            printf("Ordem original restaurada.\n");
            ordenado_por_nome = 0;
        } else if (opcao == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
