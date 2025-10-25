/*
 * DESAFIO NÍVEL 3: DETECTIVE QUEST (JULGAMENTO)
 * LINGUAGEM: C
 *
 * DESCRIÇÃO: Sistema completo integrando 3 estruturas:
 * 1. Árvore de Mapa (Binária): Para navegação.
 * 2. Árvore de Pistas (BST): Para coletar pistas em ordem.
 * 3. Tabela Hash: Para associar pistas (Chave) a suspeitos (Valor).
 *
 * Foco: Ao final, pedir uma acusação e verificar na BST
 * e Hash se há evidências (>= 2 pistas) contra o acusado.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estrutura 1: Sala (Árvore do Mapa) ---
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// --- Estrutura 2: PistaNode (Árvore de Pistas - BST) ---
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// --- Estrutura 3: HashNode (Tabela Hash) ---
#define HASH_SIZE 10 // Tamanho da Tabela Hash

typedef struct HashNode {
    char pista[100];   // Chave
    char suspeito[50]; // Valor
    struct HashNode* proximo; // Para tratamento de colisão (chaining)
} HashNode;

// A Tabela Hash é um vetor de ponteiros
HashNode* tabelaHash[HASH_SIZE];

// --- Protótipos (Geral) ---
void limparBuffer();

// --- Protótipos (Mapa) ---
Sala* criarSala(const char* nome, const char* pista);
void explorarSalas(Sala* salaInicial, PistaNode** raizPistas);
void liberarMapa(Sala* raiz);

// --- Protótipos (Pistas - BST) ---
void inserirPista(PistaNode** raiz, const char* pista);
void exibirPistas(PistaNode* raiz);
void liberarPistas(PistaNode* raiz);

// --- Protótipos (Hash) ---
unsigned int hash(const char* pista);
void inicializarHash();
void inserirNaHash(const char* pista, const char* suspeito);
const char* encontrarSuspeito(const char* pista);
void liberarHash();

// --- Protótipos (Julgamento) ---
int verificarSuspeitoFinal(PistaNode* raizPistas, const char* suspeitoAcusado);


// --- Função Principal ---
int main() {
    PistaNode* pistasColetadas = NULL;
    
    // Nível 3: Inicializa e popula a Tabela Hash
    inicializarHash();
    printf("Registrando associacoes Pista -> Suspeito...\n");
    inserirNaHash("Cigarrilha", "Mordomo");
    inserirNaHash("Carta Rasgada", "Condessa");
    inserirNaHash("Frasco de Veneno", "Mordomo"); // 2ª pista do Mordomo
    inserirNaHash("Taca Quebrada", "Sobrinho");
    inserirNaHash("Testamento Aberto", "Condessa"); // 2ª pista da Condessa
    inserirNaHash("Terra Removida", "Jardineiro");
    inserirNaHash("Botao Solto", "Sobrinho"); // 2ª pista do Sobrinho

    // Nível 3: Monta o mapa (igual Nível 2)
    printf("Montando o mapa da mansao...\n");
    Sala* hall = criarSala("Hall de Entrada", "");
    hall->esquerda = criarSala("Sala de Estar", "Cigarrilha");
    hall->direita = criarSala("Biblioteca", "Carta Rasgada");
    hall->esquerda->esquerda = criarSala("Cozinha", "Frasco de Veneno");
    hall->esquerda->direita = criarSala("Sala de Jantar", "Taca Quebrada");
    hall->direita->esquerda = criarSala("Escritorio", "Testamento Aberto");
    hall->direita->direita = criarSala("Jardim de Inverno", "Terra Removida");
    hall->direita->esquerda->esquerda = criarSala("Quarto de Hospedes", "Botao Solto");

    // Nível 3: Exploração (igual Nível 2)
    explorarSalas(hall, &pistasColetadas);

    // Nível 3: Fase de Julgamento
    printf("\n============================================\n");
    printf("            FASE DE JULGAMENTO\n");
    printf("============================================\n");
    printf("PISTAS COLETADAS NA INVESTIGACAO:\n");
    if (pistasColetadas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasColetadas);
    }
    printf("--------------------------------------------\n");
    printf("SUSPEITOS: Mordomo, Condessa, Sobrinho, Jardineiro\n");
    
    char acusado[50];
    printf("Quem voce acusa? ");
    // Usamos %49s para evitar overflow do buffer char[50]
    scanf("%49s", acusado);
    limparBuffer();

    // Nível 3: Verifica as evidências
    int numEvidencias = verificarSuspeitoFinal(pistasColetadas, acusado);

    printf("\n--- VEREDITO ---\n");
    printf("Voce acusou: %s\n", acusado);
    printf("Numero de pistas encontradas contra esta pessoa: %d\n", numEvidencias);

    // Requisito: Pelo menos 2 pistas
    if (numEvidencias >= 2) {
        printf("ACUSACAO SUSTENTADA! As evidencias sao fortes.\n");
        printf("O caso esta encerrado. Parabens, Detetive!\n");
    } else {
        printf("ACUSACAO FALHOU! Nao ha evidencias suficientes.\n");
        printf("O verdadeiro culpado escapou...\n");
    }
    
    // Libera TODAS as estruturas
    liberarMapa(hall);
    liberarPistas(pistasColetadas);
    liberarHash();
    printf("\nJogo encerrado. Memoria liberada.\n");
    
    return 0;
}

// --- Funções Auxiliares ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Funções do Mapa (Árvore Binária) ---

Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) { exit(1); }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

void explorarSalas(Sala* salaInicial, PistaNode** raizPistas) {
    Sala* atual = salaInicial;
    char escolha = ' ';
    while (escolha != 's') {
        printf("\n----------------------------------------\n");
        printf("Voce esta em: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("PISTA ENCONTRADA: %s\n", atual->pista);
            inserirPista(raizPistas, atual->pista);
        }
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este e um comodo sem saida.\n");
        }
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

void inserirPista(PistaNode** raiz, const char* pista) {
    if (*raiz == NULL) {
        *raiz = criarPistaNode(pista);
        printf("(Pista %s adicionada a BST.)\n", pista);
        return;
    }
    int cmp = strcmp(pista, (*raiz)->pista);
    if (cmp < 0) {
        inserirPista(&((*raiz)->esquerda), pista);
    } else if (cmp > 0) {
        inserirPista(&((*raiz)->direita), pista);
    } // else: Pista duplicada, não insere
}

void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("  - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

void liberarPistas(PistaNode* raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// --- Funções da Tabela Hash ---

/**
 * Função Hash simples (soma dos valores ASCII)
 */
unsigned int hash(const char* pista) {
    unsigned long hash = 5381;
    int c;
    while ((c = *pista++))
        hash = ((hash << 5) + hash) + c; // djb2 hash algorithm
    return hash % HASH_SIZE;
}

void inicializarHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        tabelaHash[i] = NULL;
    }
}

/**
 * Nível 3: inserirNaHash()
 * Insere a associação Pista (Chave) -> Suspeito (Valor) na hash.
 * Usa encadeamento (chaining) para colisões.
 */
void inserirNaHash(const char* pista, const char* suspeito) {
    unsigned int index = hash(pista);
    
    // Cria o novo nó
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    if (novo == NULL) { exit(1); }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    
    // Insere no início da lista encadeada (encadeamento)
    novo->proximo = tabelaHash[index];
    tabelaHash[index] = novo;
}

/**
 * Nível 3: encontrarSuspeito()
 * Busca uma pista na Tabela Hash e retorna o suspeito associado.
 * Retorna "Desconhecido" se a pista não for encontrada.
 */
const char* encontrarSuspeito(const char* pista) {
    unsigned int index = hash(pista);
    
    HashNode* temp = tabelaHash[index];
    // Percorre a lista encadeada naquele índice
    while (temp != NULL) {
        if (strcmp(temp->pista, pista) == 0) {
            return temp->suspeito; // Encontrou
        }
        temp = temp->proximo;
    }
    return "Desconhecido"; // Não encontrou
}

void liberarHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
}

// --- Função de Julgamento ---

/**
 * Nível 3: verificarSuspeitoFinal()
 * Função RECURSIVA que percorre a BST de pistas COLETADAS,
 * consulta a Tabela Hash para cada pista e conta quantas
 * batem com o 'suspeitoAcusado'.
 * Retorna o número total de evidências.
 */
int verificarSuspeitoFinal(PistaNode* raizPistas, const char* suspeitoAcusado) {
    // Caso base da recursão
    if (raizPistas == NULL) {
        return 0;
    }

    int contador = 0;
    
    // Consulta a Hash para a pista ATUAL
    const char* suspeitoDaPista = encontrarSuspeito(raizPistas->pista);
    
    // Compara o suspeito da pista com o suspeito acusado
    if (strcmp(suspeitoDaPista, suspeitoAcusado) == 0) {
        contador = 1; // Encontrou 1 evidência neste nó
    }

    // Soma as evidências deste nó com as das sub-árvores
    return contador + 
           verificarSuspeitoFinal(raizPistas->esquerda, suspeitoAcusado) +
           verificarSuspeitoFinal(raizPistas->direita, suspeitoAcusado);
}
