/*
 * DESAFIO NÍVEL 1: DETECTIVE QUEST (MAPA DA MANSÃO)
 * LINGUAGEM: C
 *
 * DESCRIÇÃO: Simula o mapa da mansão como uma Árvore Binária.
 * Foco: Criação de uma árvore estática (hard-coded) e
 * navegação interativa (esquerda/direita) até um nó-folha.
 *
 * Operações:
 * 1. Navegar (e)squerda
 * 2. Navegar (d)ireita
 * 3. (s)air
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estrutura da Sala (Nó da Árvore) ---
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// --- Protótipos das Funções ---
void limparBuffer();
Sala* criarSala(const char* nome);
void explorarSalas(Sala* salaInicial);
void liberarMapa(Sala* raiz);

// --- Função Principal ---
int main() {
    // Nível 1: Montagem manual do mapa (árvore binária)
    printf("Montando o mapa da mansao...\n");
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Biblioteca");
    
    hall->esquerda->esquerda = criarSala("Cozinha");
    hall->esquerda->direita = criarSala("Sala de Jantar");
    
    hall->direita->esquerda = criarSala("Escritorio");
    hall->direita->direita = criarSala("Jardim de Inverno");
    
    // Um nó-folha (sem saída)
    hall->esquerda->esquerda->esquerda = criarSala("Despensa");

    // Nível 1: Inicia a exploração
    explorarSalas(hall);

    // Libera a memória alocada para o mapa
    liberarMapa(hall);
    printf("Mapa liberado da memoria. Jogo encerrado.\n");
    
    return 0;
}

// --- Implementação das Funções ---

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Nível 1: criarSala()
 * Aloca dinamicamente (malloc) uma nova sala e define seu nome.
 */
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro fatal: Falha ao alocar memoria para a sala!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * Nível 1: explorarSalas()
 * Permite a navegação interativa do jogador pela árvore.
 */
void explorarSalas(Sala* salaInicial) {
    Sala* atual = salaInicial;
    char escolha = ' ';

    while (escolha != 's') {
        printf("\n----------------------------------------\n");
        printf("Voce esta em: %s\n", atual->nome);

        // Nível 1: Verifica se é um nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este e um comodo sem saida. (Fim do caminho)\n");
        }

        // Mostra opções
        if (atual->esquerda != NULL) {
            printf("(e) Ir para a Esquerda (%s)\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf("(d) Ir para a Direita (%s)\n", atual->direita->nome);
        }
        printf("(s) Sair da exploracao\n");
        printf("Escolha: ");

        scanf(" %c", &escolha); // O espaço antes de %c ignora newlines
        limparBuffer();

        switch (escolha) {
            case 'e':
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("Caminho bloqueado. Nao ha sala a esquerda.\n");
                }
                break;
            case 'd':
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("Caminho bloqueado. Nao ha sala a direita.\n");
                }
                break;
            case 's':
                printf("Voce decidiu encerrar a exploracao...\n");
                break;
            default:
                printf("Opcao invalida. Tente 'e', 'd' ou 's'.\n");
        }
    }
}

/**
 * Libera a memória da árvore (mapa) usando percurso pós-ordem.
 */
void liberarMapa(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarMapa(raiz->esquerda); // Libera sub-árvore esquerda
    liberarMapa(raiz->direita);  // Libera sub-árvore direita
    free(raiz);                  // Libera o nó atual
}
