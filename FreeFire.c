#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10     // capacidade máxima da mochila
#define TAM_NOME 30
#define TAM_TIPO 20

// ----------------------------------------------------------
// Estrutura que representa um item da mochila do jogador
// ----------------------------------------------------------
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;

// ----------------------------------------------------------
// Função: inserirItem
// Objetivo: adiciona um novo item à mochila, se houver espaço.
// ----------------------------------------------------------
void inserirItem(Item mochila[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\n Mochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;
    getchar(); // limpar buffer
    printf("Digite o nome do item: ");
    fgets(novo.nome, TAM_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // remover \n

    printf("Digite o tipo do item (arma, munição, cura, etc.): ");
    fgets(novo.tipo, TAM_TIPO, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    mochila[*total] = novo;
    (*total)++;

    printf("\n Item \"%s\" adicionado à mochila com sucesso!\n", novo.nome);
}

// ----------------------------------------------------------
// Função: removerItem
// Objetivo: remove um item da mochila com base no nome informado.
// ----------------------------------------------------------
void removerItem(Item mochila[], int *total) {
    if (*total == 0) {
        printf("\n A mochila está vazia. Nada para remover.\n");
        return;
    }

    char nome[TAM_NOME];
    getchar();
    printf("Digite o nome do item que deseja remover: ");
    fgets(nome, TAM_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < *total; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n Item \"%s\" não encontrado na mochila.\n", nome);
        return;
    }

    for (int i = encontrado; i < *total - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    (*total)--;
    printf("\n Item \"%s\" removido com sucesso!\n", nome);
}

// ----------------------------------------------------------
// Função: listarItens
// Objetivo: exibe todos os itens atualmente armazenados na mochila.
// ----------------------------------------------------------
void listarItens(Item mochila[], int total) {
    printf("\n=== ITENS NA MOCHILA ===\n");

    if (total == 0) {
        printf("(mochila vazia)\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// ----------------------------------------------------------
// Função: buscarItem
// Objetivo: realiza uma busca sequencial pelo nome e exibe seus dados.
// ----------------------------------------------------------
void buscarItem(Item mochila[], int total) {
    if (total == 0) {
        printf("\n A mochila está vazia. Não há itens para buscar.\n");
        return;
    }

    char nome[TAM_NOME];
    getchar();
    printf("Digite o nome do item que deseja buscar: ");
    fgets(nome, TAM_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < total; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("\n🔍 Item encontrado!\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("\n O item \"%s\" não foi encontrado na mochila.\n", nome);
}

// ----------------------------------------------------------
// Função principal (main)
// ----------------------------------------------------------
int main() {
    Item mochila[MAX_ITENS];
    int total = 0;
    int opcao;

    do {
        printf("\n==============================\n");
        printf("  SISTEMA DE INVENTÁRIO  \n");
        printf("==============================\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("==============================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem(mochila, &total);
                listarItens(mochila, total);
                break;
            case 2:
                removerItem(mochila, &total);
                listarItens(mochila, total);
                break;
            case 3:
                listarItens(mochila, total);
                break;
            case 4:
                buscarItem(mochila, total);
                break;
            case 0:
                printf("\n Saindo do sistema... Até a próxima!\n");
                break;
            default:
                printf("\n Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}