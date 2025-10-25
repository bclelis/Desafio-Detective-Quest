/*
 * DESAFIO NÍVEL 2: DETECTIVE QUEST (COLETA DE PISTAS)
 * LINGUAGEM: C
 *
 * DESCRIÇÃO: Expande o Nível 1.
 * 1. Árvore de Mapa (Binária): Salas agora contêm pistas (strings).
 * 2. Árvore de Pistas (BST): Pistas coletadas são inseridas em uma
 * Árvore Binária de Busca (BST).
 * 3. Exibição: Ao final, exibe as pistas em ordem alfabética.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estrutura da Sala (Árvore do Mapa) ---
typedef struct Sala {
    char nome[50];
    char pista[100]; // Nível 2: Pista associada à sala
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// --- Estrutura da Pista (Árvore de Pistas - BST) ---
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// --- Protótipos (Mapa) ---
void limparBuffer();
Sala* criarSala(const char* nome, const char* pista);
void explorarSalasComPistas(Sala* salaInicial, PistaNode** raizPistas);
void liberarMapa(Sala* raiz);

// --- Protótipos (Pistas - BST) ---
PistaNode* criarPistaNode(const char* pista);
void inserirPista(PistaNode** raiz, const char* pista);
void exibirPistas(PistaNode* raiz);
void liberarPistas(PistaNode* raiz);

// --- Função Principal ---
int main() {
    // Nível 2: Raiz da Árvore de Pistas (BST)
    PistaNode* pistasColetadas = NULL;

    printf("Montando o mapa da mansao (com pistas)...\n");
    // Nível 2: Montagem do mapa com pistas
    Sala* hall = criarSala("Hall de Entrada", "");
    hall->esquerda = criarSala("Sala de Estar", "Cigarrilha");
    hall->direita = criarSala("Biblioteca", "Carta Rasgada");
    
    hall->esquerda->esquerda = criarSala("Cozinha", "Frasco de Veneno");
    hall->esquerda->direita = criarSala("Sala de Jantar", "Taca Quebrada");
    
    hall->direita->esquerda = criarSala("Escritorio", "Testamento Aberto");
    hall->direita->direita = criarSala("Jardim de Inverno", "Terra Removida");
    
    // Pista duplicada para testar a BST (não deve inserir)
    hall->esquerda->esquerda->esquerda = criarSala("Despensa", "Frasco de Veneno");

    // Nível 2: Inicia a exploração
    explorarSalasComPistas(hall, &pistasColetadas);

    // Nível 2: Exibir pistas coletadas em ordem
    printf("\n============================================\n");
    printf("PISTAS COLETADAS (EM ORDEM ALFABETICA):\n");
    if (pistasColetadas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasColetadas);
    }
    printf("============================================\n");

    // Libera a memória das duas árvores
    liberarMapa(hall);
    liberarPistas(pistasColetadas);
    printf("Jogo encerrado. Memoria liberada.\n");
    
    return 0;
}

// --- Funções Auxiliares ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Funções do Mapa (Árvore Binária) ---

/**
 * Nível 2: criarSala()
 * Aloca dinamicamente uma sala com nome e pista.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) { exit(1); }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista); // Copia a pista
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * Nível 2: explorarSalasComPistas()
 * Navega pela mansão e chama inserirPista() quando encontra uma.
 */
void explorarSalasComPistas(Sala* salaInicial, PistaNode** raizPistas) {
    Sala* atual = salaInicial;
    char escolha = ' ';

    while (escolha != 's') {
        printf("\n----------------------------------------\n");
        printf("Voce esta em: %s\n", atual->nome);

        // Nível 2: Verifica e coleta a pista
        if (strlen(atual->pista) > 0) {
            printf("PISTA ENCONTRADA: %s\n", atual->pista);
            inserirPista(raizPistas, atual->pista);
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este e um comodo sem saida.\n");
        }
        
        // Menu de navegação
        if (atual->esquerda != NULL) printf("(e) Esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("(d) Direita (%s)\n", atual->direita->nome);
        printf("(s) Sair da exploracao\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);
        limparBuffer();

        switch (escolha) {
            case 'e':
                if (atual->esquerda != NULL) atual = atual->esquerda;
                else printf("Caminho bloqueado.\n");
                break;
            case 'd':
                if (atual->direita != NULL) atual = atual->direita;
                else printf("Caminho bloqueado.\n");
                break;
            case 's':
                printf("Voce decidiu encerrar a exploracao...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    }
}

void liberarMapa(Sala* raiz) {
    if (raiz == NULL) return;
    liberarMapa(raiz->esquerda);
    liberarMapa(raiz->direita);
    free(raiz);
}

// --- Funções das Pistas (BST) ---

PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo == NULL) { exit(1); }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/**
 * Nível 2: inserirPista()
 * Insere uma pista na BST (recursivamente).
 * Não insere se a pista já existir (strcmp == 0).
 */
void inserirPista(PistaNode** raiz, const char* pista) {
    // Caso base: Árvore (ou sub-árvore) está vazia
    if (*raiz == NULL) {
        *raiz = criarPistaNode(pista);
        printf("(Pista adicionada a BST.)\n");
        return;
    }

    int cmp = strcmp(pista, (*raiz)->pista);

    if (cmp < 0) {
        // Inserir na sub-árvore esquerda
        inserirPista(&((*raiz)->esquerda), pista);
    } else if (cmp > 0) {
        // Inserir na sub-árvore direita
        inserirPista(&((*raiz)->direita), pista);
    } else {
        // Pista já existe (cmp == 0), não faz nada.
        printf("(Pista duplicada, ignorando.)\n");
    }
}

/**
 * Nível 2: exibirPistas()
 * Imprime as pistas em ordem alfabética (Percurso In-Order).
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    exibirPistas(raiz->esquerda);     // Visita esquerda
    printf("  - %s\n", raiz->pista);  // Visita (imprime) raiz
    exibirPistas(raiz->direita);      // Visita direita
}

void liberarPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}
