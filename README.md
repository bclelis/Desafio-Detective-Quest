# Detective Quest - Desafios de Árvores e Hash em C

Este repositório contém uma série de desafios de programação em C focados na implementação de estruturas de dados avançadas (Árvores Binárias, BST, Tabela Hash), utilizando a temática de um jogo de investigação, o "Detective Quest".

## 🚀 Sobre os Desafios

O projeto é dividido em três níveis, cada um construindo sobre a lógica do anterior, todos localizados na pasta `/src`.

### Nível 1: O Mapa da Mansão (Árvore Binária)

**Local:** `src/Nivel1_MapaBinario/`

Implementação da navegação básica pela mansão, representada como uma Árvore Binária estática.

* **Estrutura:** `struct Sala` (Árvore Binária).
* **Conceitos:** `malloc`, `structs`, ponteiros, navegação em árvore (acesso a `->esquerda` e `->direita`).
* **Funcionalidade:** O jogador inicia no "Hall de Entrada" (raiz) e pode navegar para a esquerda (e) ou direita (d) até decidir sair (s) ou chegar a um nó-folha (sala sem saída).

### Nível 2: Coleta de Pistas (BST)

**Local:** `src/Nivel2_BST_Pistas/`

Expande o Nível 1, adicionando um sistema de coleta de pistas.

* **Estruturas:**
    1.  `struct Sala` (Árvore do Mapa): Agora contém um campo `pista` (string).
    2.  `struct PistaNode` (Árvore Binária de Busca - BST): Uma nova árvore é criada para armazenar as pistas coletadas.
* **Conceitos:** Árvore Binária de Busca (BST), inserção recursiva, `strcmp` para ordenação, percurso *in-order* (em-ordem).
* **Funcionalidade:** Ao visitar uma sala, o jogador coleta automaticamente a pista associada. Essa pista é inserida na BST. Ao final do jogo, o programa exibe todas as pistas coletadas em **ordem alfabética**, graças ao percurso *in-order* da BST.

### Nível 3: O Julgamento Final (Tabela Hash)

**Local:** `src/Nivel3_Hash_Suspeitos/`

O desafio mestre, que integra todas as estruturas para permitir um julgamento final.

* **Estruturas:**
    1.  Árvore do Mapa (como no Nível 2).
    2.  BST de Pistas (como no Nível 2).
    3.  **Tabela Hash:** Uma nova estrutura (`struct HashNode`) é usada para mapear Pistas (Chave) a Suspeitos (Valor). Usa encadeamento (chaining) para tratar colisões.
* **Conceitos:** Implementação de Tabela Hash (função `hash`, `inserir`, `buscar`), integração de múltiplas estruturas de dados.
* **Funcionalidade:**
    1.  O jogo inicializa a Tabela Hash com as associações (ex: "Cigarrilha" -> "Mordomo").
    2.  O jogador explora a mansão e coleta pistas na BST (como no Nível 2).
    3.  Ao final, o programa exibe as pistas coletadas e pede ao jogador que acuse um suspeito.
    4.  O sistema então percorre a **BST de pistas coletadas** e, para cada pista, consulta a **Tabela Hash** para ver a quem ela pertence.
    5.  Se o número de pistas que apontam para o suspeito acusado for 2 ou mais, a acusação é sustentada.

## 🛠️ Como Compilar e Executar

Cada nível é um programa independente e deve ser compilado separadamente. Use um compilador C como o `gcc`.

## 🧠 Conceitos Praticados

Structs e Ponteiros: Base de todas as estruturas de dados.

Alocação Dinâmica: malloc() e free() para criar e destruir nós.

Árvore Binária: Modelagem de dados hierárquicos (mapa).

Árvore Binária de Busca (BST): Armazenamento e recuperação de dados ordenados (pistas).

Recursividade: Essencial para inserção e travessia das árvores (exibirPistas, liberarMapa, verificarSuspeitoFinal).

Tabela Hash: Mapeamento eficiente de chave-valor (Pista -> Suspeito) com tratamento de colisão (encadeamento).

Modularização: Separação da lógica em funções com responsabilidades claras.
