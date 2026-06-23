# Documentacao do Projeto: Mercadinho — Sistema de Gerenciamento

**Disciplina:** Estrutura de Dados  
**Periodo:** 3o Periodo — 2026  
**Instituicao:** Universidade do Vale do Itajai (UNIVALI)  
**Linguagem:** C (padrao C99)  
**Biblioteca Grafica:** Raylib 5.5  
**Plataforma-alvo:** Windows 64-bit (MinGW-w64)

---

## Sumario

1. [Visao Geral do Projeto](#1-visao-geral-do-projeto)
1.5. [Conceitos de C Essenciais](#15-conceitos-de-c-essenciais)
2. [Estrutura de Arquivos](#2-estrutura-de-arquivos)
3. [Diagrama de Dependencias](#3-diagrama-de-dependencias)
4. [types.h — Tipos Compartilhados](#4-typesh--tipos-compartilhados)
5. [hash.h/c — Tabela Hash (Estoque)](#5-hashhc--tabela-hash-encadeamento-externo)
6. [fila.h/c — Fila Dinamica (Clientes)](#6-filahc--fila-dinamica-fifo)
7. [pilha.h/c — Pilha Dinamica (Carrinho)](#7-pilhahc--pilha-dinamica-lifo)
8. [bst.h/c — Arvore Binaria de Busca (Historico)](#8-bsthc--arvore-binaria-de-busca-bst)
9. [sort.h/c — Quick Sort](#9-sorthc--quick-sort)
10. [dados.h/c — Geracao e Leitura de Arquivos](#10-dadoshc--geracao-e-leitura-de-arquivos)
11. [estado.h — Estado Global da Aplicacao](#11-estadoh--estado-global-da-aplicacao)
12. [ui.h/c — Componentes Visuais (Raylib)](#12-uihc--componentes-visuais-raylib)
13. [main.c — Ponto de Entrada e Loop Principal](#13-mainc--ponto-de-entrada-e-loop-principal)
14. [tela_menu.c — Dashboard / Menu Principal](#14-tela_menuc--dashboard--menu-principal)
15. [tela_cadastro.c — Cadastro de Produtos](#15-tela_cadastroc--cadastro-de-produtos)
16. [tela_caixa.c — Frente de Caixa](#16-tela_caixac--frente-de-caixa)
17. [tela_relatorios.c — Relatorios Gerenciais](#17-tela_relatorioc--relatorios-gerenciais)
18. [tela_auditoria.c — Auditoria / Consulta de Vendas](#18-tela_auditoriac--auditoria--consulta-de-vendas)
19. [Fluxo Completo de uma Venda](#19-fluxo-completo-de-uma-venda)
20. [Gerenciamento de Memoria](#20-gerenciamento-de-memoria)
21. [Como Compilar e Executar](#21-como-compilar-e-executar)
22. [Conceitos de Estrutura de Dados — Resumo](#22-conceitos-de-estrutura-de-dados--resumo)

---

## 1. Visao Geral do Projeto

O **Mercadinho** e um sistema de gerenciamento de supermercado desenvolvido em linguagem C com interface grafica baseada na biblioteca **Raylib**. O sistema foi projetado como trabalho pratico da disciplina de Estrutura de Dados e implementa cinco estruturas de dados classicas.

**Restricao importante do enunciado:** e proibido o uso de colecoes nativas da linguagem ou arrays estaticos globais de tamanho fixo para as colecoes principais. Toda a logica de ponteiros e gerenciamento de memoria e feita manualmente, conforme detalhado na Secao 20.

Estruturas implementadas:

| Estrutura | Aplicacao |
|-----------|-----------|
| **Tabela Hash** com encadeamento externo | Armazenamento e consulta rapida do estoque de produtos |
| **Fila Dinamica (FIFO)** | Gerenciamento da fila de clientes aguardando atendimento |
| **Pilha Dinamica (LIFO)** | Carrinho de compras do cliente em atendimento |
| **Arvore Binaria de Busca (BST)** | Historico de vendas realizadas |
| **Quick Sort** | Ordenacao do ranking de faturamento |

### Funcionalidades

- **Dashboard** com indicadores numericos (clientes na fila, produtos em estoque, vendas realizadas)
- **Frente de Caixa** com atendimento por senha (fila), bipe de produtos (pilha) e finalizacao de venda
- **Cadastro de Produtos** com insercao e consulta na tabela hash
- **Relatorios Gerenciais** com listagem em ordem de ID (in-order da BST) e ranking de faturamento (Quick Sort)
- **Auditoria** com consulta de venda por ID e exibicao de cupom fiscal detalhado

---

## 1.5. Conceitos de C Essenciais

Esta secao explica os conceitos basicos de C que voce precisa entender para acompanhar o projeto. Se voce nunca viu ponteiros ou `malloc`, comeco por aqui.

---

### 1.5.1 O que e uma `struct`?

Uma `struct` (estrutura) e um tipo de dado que **agrupa varias variaveis relacionadas** em um so pacote.

Imagine que voce quer representar um produto. Sem struct, voce teria variaveis soltas:

```c
int   codigo;
char  nome[100];
float preco;
```

Com struct, voce junta tudo:

```c
typedef struct {
    int   codigo;
    char  nome[100];
    float preco;
} Produto;
```

Agora `Produto` vira um tipo novo. Voce pode declarar variaveis dele:

```c
Produto p;
p.codigo = 1001;
p.preco  = 25.90f;
```

O operador `.` (ponto) acessa os campos internos. Quando voce tem um **ponteiro** para struct, usa `->` (seta) no lugar do ponto.

---

### 1.5.2 O que sao ponteiros?

Um **ponteiro** e uma variavel que **guarda um endereco de memoria**, nao um valor direto.

```c
int  x = 10;   /* x guarda o VALOR 10               */
int *p = &x;   /* p guarda o ENDERECO onde o 10 esta */
```

- `&x` significa "endereco de x"
- `*p` significa "conteudo do endereco que p aponta"

**Por que usar ponteiros?**
1. Para evitar copiar dados grandes (passar o endereco e mais rapido que copiar a struct inteira)
2. Para modificar variaveis dentro de funcoes
3. Para criar estruturas dinamicas (listas, arvores, etc.)

Exemplo:

```c
void dobrar(int *n) {
    *n = (*n) * 2;  /* modifica o valor ORIGINAL */
}

int main() {
    int a = 5;
    dobrar(&a);     /* passa o endereco de a */
    printf("%d", a); /* imprime 10 */
}
```

---

### 1.5.3 Ponteiro de ponteiro (`Tipo**`)

Algumas estruturas usam `Tipo**` (ponteiro para ponteiro).

```c
HashNode **tabela;
```

Pense assim:
- `int *p` = ponteiro para um inteiro (p aponta para onde tem um `int`)
- `int **pp` = ponteiro para um ponteiro de inteiro (pp aponta para onde tem um `int*`)

No caso da Tabela Hash:
- `tabela` e um **vetor de ponteiros**
- Cada posicao `tabela[i]` e um ponteiro (`HashNode*`) que aponta para uma lista ligada
- Para representar esse "vetor de ponteiros", usamos `HashNode**` (ponteiro para o primeiro elemento do vetor)

Na pratica:

```c
/* calloc aloca um vetor de 1009 ponteiros e zera tudo */
HashNode **tabela = (HashNode**) calloc(1009, sizeof(HashNode*));
```

Agora `tabela[0]` e um `HashNode*` (ponteiro para no), `tabela[1]` e outro, etc.

---

### 1.5.4 malloc, calloc e free — Alocacao Dinamica

Normalmente, variaveis ficam na **pilha de memoria** (stack) e sao destruidas quando a funcao termina. Para dados que precisam **sobreviver a funcao** ou ter tamanho variavel, usamos o **monte** (heap).

| Funcao | O que faz |
|--------|-----------|
| `malloc(tamanho)` | Aloca `tamanho` bytes no heap e retorna o endereco (lixo na memoria) |
| `calloc(n, tamanho)` | Aloca `n` elementos de `tamanho` bytes cada e **zera** tudo |
| `free(ptr)` | Libera a memoria alocada de volta para o sistema |

Exemplo:

```c
/* aloca espaco para 1 produto */
Produto *p = (Produto*) malloc(sizeof(Produto));
if (p == NULL) { /* erro: sem memoria */ }

p->codigo = 2000;
p->preco  = 15.90f;

/* quando nao precisar mais */
free(p);   /* p ainda existe, mas o conteudo foi liberado */
p = NULL;  /* boa pratica: evita usar o ponteiro solto */
```

**Por que sizeof?** O C precisa saber quantos bytes cada tipo ocupa. `sizeof(Produto)` calcula automaticamente.

**Regra de ouro:** Todo `malloc`/`calloc` deve ter um `free` correspondente. Senao, o programa **vaza memoria** (consome RAM sem devolver).

---

### 1.5.5 O loop principal do Raylib

Raylib e uma biblioteca grafica que abre uma janela e desenha nela. O coracao de qualquer programa Raylib e o **loop principal**:

```c
while (!WindowShouldClose()) {   /* enquanto o usuario nao fechar a janela */
    BeginDrawing();               /* comeca a desenhar o frame atual       */

    /* codigo de desenho: textos, botoes, retangulos, etc */
    DrawText("Ola!", 100, 100, 20, WHITE);

    EndDrawing();                 /* finaliza o frame e mostra na tela     */
}
CloseWindow();                    /* fecha a janela                        */
```

Esse loop roda **60 vezes por segundo** (ou o FPS configurado). Em cada volta (frame):
1. O mouse e teclado sao lidos
2. A logica do programa atualiza os dados
3. O desenho e renderizado na tela
4. `EndDrawing()` troca o buffer (evita piscar)

No nosso projeto, o loop principal de `main.c` segue esse padrao, mas em vez de desenhar sempre a mesma coisa, ele chama a funcao da tela atual (`tela_menu_desenhar`, `tela_caixa_desenhar`, etc.) dependendo do valor de `app->tela_atual`.

---

## 2. Estrutura de Arquivos

```
m3-mercado/
├── build.bat                     # Script de compilacao (Windows/MinGW)
├── instalar_e_compilar.bat       # Script que baixa dependencias e compila
├── mercadinho.exe                # Executavel compilado
├── data/
│   ├── produtos.txt              # 1000 produtos gerados (formato CSV)
│   └── clientes.txt              # 1000 clientes gerados (formato CSV)
├── DOCUMENTACAO.md               # Este arquivo
└── src/
    ├── main.c                    # Ponto de entrada, inicializacao e game loop
    ├── types.h                   # Definicoes de tipos compartilhados
    ├── estado.h                  # Estado global da aplicacao
    ├── estruturas/
    │   ├── hash.c / hash.h       # Tabela Hash (encadeamento externo)
    │   ├── fila.c / fila.h       # Fila Dinamica (FIFO)
    │   ├── pilha.c / pilha.h     # Pilha Dinamica (LIFO)
    │   ├── bst.c / bst.h         # Arvore Binaria de Busca (BST)
    │   ├── sort.c / sort.h       # Quick Sort
    │   └── dados.c / dados.h     # Geracao e leitura de arquivos
    └── interface/
        ├── ui.c / ui.h           # Componentes visuais (botoes, campos, etc.)
        ├── tela_menu.c / tela_menu.h
        ├── tela_cadastro.c / tela_cadastro.h
        ├── tela_caixa.c / tela_caixa.h
        ├── tela_relatorios.c / tela_relatorios.h
        └── tela_auditoria.c / tela_auditoria.h
```

---

## 3. Diagrama de Dependencias

O diagrama abaixo mostra as relacoes de inclusao entre os arquivos do projeto. Uma seta `A -> B` significa que o arquivo A inclui (importa) o arquivo B.

```
                    main.c
                   /   |   \
                  /    |    \
                 /     |     \
          estado.h  dados.h   ui.h
          /   |   \     |        \
         /    |    \    |         \
    types.h  hash.h  fila.h  pilha.h  bst.h
                |
           types.h (compartilhado por todos)
```

### Detalhamento por arquivo:

```
types.h  (incluido por todos)
   ^
   |
hash.h -> types.h
fila.h -> types.h
pilha.h -> types.h
bst.h -> types.h
dados.h -> types.h, hash.h, fila.h
sort.h -> types.h

estado.h -> types.h, hash.h, fila.h, pilha.h, bst.h

ui.h -> raylib.h (biblioteca externa)
tela_menu.h -> estado.h
tela_cadastro.h -> estado.h, ui.h
tela_caixa.h -> estado.h, ui.h
tela_relatorios.h -> estado.h, ui.h
tela_auditoria.h -> estado.h, ui.h

main.c -> types.h, estado.h, dados.h, ui.h,
          tela_menu.h, tela_cadastro.h, tela_caixa.h,
          tela_relatorios.h, tela_auditoria.h

hash.c -> hash.h, stdlib.h, string.h
fila.c -> fila.h, stdlib.h
pilha.c -> pilha.h, stdlib.h
bst.c -> bst.h, stdlib.h
sort.c -> sort.h
dados.c -> dados.h, stdio.h, stdlib.h, string.h
ui.c -> ui.h, string.h, stdio.h

tela_menu.c -> tela_menu.h, ui.h, stdio.h
tela_cadastro.c -> tela_cadastro.h, stdio.h, stdlib.h, string.h, time.h
tela_caixa.c -> tela_caixa.h, stdio.h, stdlib.h, string.h, time.h
tela_relatorios.c -> tela_relatorios.h, sort.h, stdio.h, stdlib.h
tela_auditoria.c -> tela_auditoria.h, stdio.h, stdlib.h
```

---

## 4. types.h — Tipos Compartilhados

**Arquivo:** `src/types.h`

Define as estruturas de dados fundamentais usadas por todo o sistema, alem de constantes e enumeracoes.

> **Se voce nunca viu `struct` ou `typedef` antes, leia a Secao 1.5.1 antes de continuar.

### Constantes

```c
#define MAX_NOME      100
#define MAX_TEXTO     256
#define HASH_TAMANHO  1009   /* numero primo > 1000 */
```

`HASH_TAMANHO` foi escolhido como 1009 porque e um numero primo proximo de 1000, o que reduz o numero de colisoes na funcao de espalhamento (modulo).

### Structs

#### Produto

Representa um item no estoque do supermercado.

```c
typedef struct {
    int   codigo;
    char  nome[MAX_NOME];
    float preco;
    int   quantidade;
    int   dia, mes, ano;
} Produto;
```

| Campo | Descricao |
|-------|-----------|
| `codigo` | Identificador unico do produto (ex: 1000, 1001, ...) |
| `nome` | Nome descritivo do produto (ex: "Arroz Tipo 1 5kg") |
| `preco` | Preco unitario em reais |
| `quantidade` | Quantidade disponivel em estoque |
| `dia, mes, ano` | Data de cadastro do produto |

#### Cliente

Representa um cliente que aguarda na fila ou esta sendo atendido.

```c
typedef struct {
    char nome[MAX_NOME];
    int  dia, mes, ano;
} Cliente;
```

#### ItemVenda (lista encadeada)

Representa um unico item dentro de uma venda. E uma struct **auto-referenciada** (contem um ponteiro para si mesma), formando uma lista encadeada simples.

```c
typedef struct ItemVenda {
    int   codigo;
    char  nome[MAX_NOME];
    float preco;
    struct ItemVenda *prox;
} ItemVenda;
```

#### Venda

Representa uma venda finalizada, armazenada no historico (BST).

```c
typedef struct {
    int        id_venda;
    char       cliente[MAX_NOME];
    float      valor_total;
    int        dia, mes, ano;
    ItemVenda *itens;       /* ponteiro para lista encadeada de itens */
    int        num_itens;
} Venda;
```

| Campo | Descricao |
|-------|-----------|
| `id_venda` | Identificador unico auto-incrementado |
| `cliente` | Nome do cliente que realizou a compra |
| `valor_total` | Soma dos precos de todos os itens |
| `dia, mes, ano` | Data da venda |
| `itens` | Ponteiro para lista encadeada de `ItemVenda` |
| `num_itens` | Quantidade de itens na venda |

### Enum Tela

Enum que identifica qual tela esta sendo exibida no momento. Usado maquina de estados do `main.c`.

```c
typedef enum {
    TELA_MENU = 0,
    TELA_CADASTRO,
    TELA_CAIXA,
    TELA_RELATORIOS,
    TELA_AUDITORIA
} Tela;
```

---

## 5. hash.h/c — Tabela Hash (Encadeamento Externo)

**Arquivos:** `src/estruturas/hash.h`, `src/estruturas/hash.c`

> **Se voce nunca viu ponteiro de ponteiro (`Tipo**`) antes, leia a Secao 1.5.3 antes de continuar.**

### Conceito

A **Tabela Hash** (ou tabela de espalhamento) e uma estrutura de dados que associa chaves a valores usando uma **funcao hash**. A funcao hash converte a chave (no caso, o codigo do produto) em um indice dentro de um vetor. Idealmente, cada chave mapeia para um indice unico, mas colisoes podem ocorrer.

O metodo de **encadeamento externo** (separate chaining) trata colisoes mantendo uma **lista encadeada** em cada posicao da tabela. Quando duas chaves diferentes caem no mesmo indice, ambas sao inseridas na lista daquela posicao.

```
Diagrama:

TabelaHash
+--------+     +-----------+    +-----------+
| [0]    |---->| HashNode  |--->| HashNode  |---> NULL
|        |     | cod=1001  |    | cod=2010  |
+--------+     +-----------+    +-----------+
| [1]    |----> NULL
+--------+
| [2]    |---->| HashNode  |---> NULL
|        |     | cod=1005  |
+--------+     +-----------+
  ...
| [1008] |----> NULL
+--------+
```

### Structs

```c
typedef struct HashNode {
    Produto          produto;
    struct HashNode *prox;
} HashNode;

typedef struct {
    HashNode **tabela;   /* vetor de ponteiros para listas encadeadas */
    int        tamanho;  /* tamanho do vetor (HASH_TAMANHO = 1009)    */
    int        total;    /* numero total de produtos armazenados       */
} TabelaHash;
```

### Funcoes

#### `TabelaHash *hash_criar(int tamanho)`

Aloca e inicializa a tabela hash. Cria um vetor de `tamanho` ponteiros inicialmente NULL.

```c
TabelaHash *hash_criar(int tamanho) {
    TabelaHash *h = (TabelaHash *)malloc(sizeof(TabelaHash));
    h->tamanho = tamanho;
    h->total   = 0;
    h->tabela  = (HashNode **)calloc(tamanho, sizeof(HashNode *));
    return h;
}
```

#### `void hash_destruir(TabelaHash *h)`

Percorre todas as posicoes da tabela, libera cada no das listas encadeadas, depois libera o vetor e a struct.

```c
void hash_destruir(TabelaHash *h) {
    for (int i = 0; i < h->tamanho; i++) {
        HashNode *node = h->tabela[i];
        while (node) {
            HashNode *prox = node->prox;
            free(node);
            node = prox;
        }
    }
    free(h->tabela);
    free(h);
}
```

#### `int hash_funcao(TabelaHash *h, int codigo)`

Funcao de espalhamento que mapeia o codigo do produto para um indice. Usa o operador **modulo** (`%`).

```c
int hash_funcao(TabelaHash *h, int codigo) {
    int idx = codigo % h->tamanho;
    return (idx < 0) ? idx + h->tamanho : idx;
}
```

| Retorno | Significado |
|---------|-------------|
| `0` a `tamanho-1` | Indice na tabela |

#### `int hash_inserir(TabelaHash *h, Produto p)`

Insere um produto na tabela. Se o codigo ja existir, **atualiza** o produto existente. Caso contrario, cria um novo no na cabeca da lista encadeada do indice calculado.

```c
int hash_inserir(TabelaHash *h, Produto p) {
    int       idx  = hash_funcao(h, p.codigo);
    HashNode *node = h->tabela[idx];

    while (node) {
        if (node->produto.codigo == p.codigo) {
            node->produto = p;
            return 0;   /* atualizado */
        }
        node = node->prox;
    }

    HashNode *novo = (HashNode *)malloc(sizeof(HashNode));
    novo->produto  = p;
    novo->prox     = h->tabela[idx];
    h->tabela[idx] = novo;
    h->total++;
    return 1;       /* novo */
}
```

| Retorno | Significado |
|---------|-------------|
| `1` | Produto inserido como novo |
| `0` | Produto existente foi atualizado |

#### `Produto *hash_buscar(TabelaHash *h, int codigo)`

Busca um produto pelo codigo. Calcula o indice, percorre a lista encadeada e retorna o ponteiro para o produto, ou NULL se nao encontrado.

```c
Produto *hash_buscar(TabelaHash *h, int codigo) {
    int       idx  = hash_funcao(h, codigo);
    HashNode *node = h->tabela[idx];
    while (node) {
        if (node->produto.codigo == codigo) return &node->produto;
        node = node->prox;
    }
    return NULL;
}
```

| Retorno | Significado |
|---------|-------------|
| `Produto*` | Ponteiro para o produto encontrado |
| `NULL` | Produto nao encontrado |

#### `int hash_atualizar_quantidade(TabelaHash *h, int codigo, int delta)`

Atualiza a quantidade em estoque de um produto. O parametro `delta` pode ser negativo (venda) ou positivo (estorno/cadastro).

```c
int hash_atualizar_quantidade(TabelaHash *h, int codigo, int delta) {
    Produto *p = hash_buscar(h, codigo);
    if (!p) return -1;
    if (p->quantidade + delta < 0) return -2;
    p->quantidade += delta;
    return p->quantidade;
}
```

| Retorno | Significado |
|---------|-------------|
| `>= 0` | Nova quantidade em estoque apos a operacao |
| `-1` | Produto nao encontrado |
| `-2` | Estoque insuficiente (resultado seria negativo) |

### Complexidade Big-O

| Operacao | Caso Medio | Pior Caso |
|----------|-----------|-----------|
| `hash_buscar` | O(1) | O(n) |
| `hash_inserir` | O(1) | O(n) |
| `hash_atualizar_quantidade` | O(1) | O(n) |
| `hash_destruir` | O(n) | O(n) |

O pior caso ocorre quando todos os produtos colidem para o mesmo indice, degenerando a hash em uma lista linear. Com `HASH_TAMANHO = 1009` e 1000 produtos, e esperada uma boa dispersao.

---

## 6. fila.h/c — Fila Dinamica (FIFO)

**Arquivos:** `src/estruturas/fila.h`, `src/estruturas/fila.c`

### Conceito

A **Fila** e uma estrutura de dados que segue o principio **FIFO** (First In, First Out): o primeiro elemento inserido e o primeiro a ser removido. Analogamente a uma fila de banco ou supermercado.

A implementacao e feita com **ponteiros encadeados**, com referencias tanto para a **frente** (inicio) quanto para o **tras** (fim), permitindo insercao O(1) no fim e remocao O(1) no inicio.

```
Diagrama:

Fila
+--------+--------+     +--------+--------+     +--------+--------+
| Cliente|  prox  |---->| Cliente|  prox  |---->| Cliente|  prox  |----> NULL
| "Ana"  |        |     |"Carlos"|        |     |"Maria" |        |
+--------+--------+     +--------+--------+     +--------+--------+
   ^                                              ^
   |                                              |
 FRENTE                                         TRAS
```

### Structs

```c
typedef struct FilaNode {
    Cliente          cliente;
    struct FilaNode *prox;
} FilaNode;

typedef struct {
    FilaNode *frente;    /* ponteiro para o primeiro no (proximo a ser atendido) */
    FilaNode *tras;      /* ponteiro para o ultimo no (ultimo a chegar)          */
    int       tamanho;   /* numero de clientes na fila                            */
} Fila;
```

### Funcoes

#### `Fila *fila_criar(void)`

Cria uma fila vazia com `frente = NULL` e `tras = NULL`.

```c
Fila *fila_criar(void) {
    Fila *f    = (Fila *)malloc(sizeof(Fila));
    f->frente  = NULL;
    f->tras    = NULL;
    f->tamanho = 0;
    return f;
}
```

#### `void fila_destruir(Fila *f)`

Remove todos os elementos da fila chamando `fila_desenfileirar` em loop, depois libera a struct.

```c
void fila_destruir(Fila *f) {
    Cliente tmp;
    while (!fila_vazia(f)) fila_desenfileirar(f, &tmp);
    free(f);
}
```

#### `void fila_enfileirar(Fila *f, Cliente c)`

Insere um cliente no **final** da fila.

```c
void fila_enfileirar(Fila *f, Cliente c) {
    FilaNode *novo = (FilaNode *)malloc(sizeof(FilaNode));
    novo->cliente  = c;
    novo->prox     = NULL;
    if (f->tras) f->tras->prox = novo;
    else         f->frente     = novo;
    f->tras = novo;
    f->tamanho++;
}
```

#### `int fila_desenfileirar(Fila *f, Cliente *out)`

Remove o cliente da **frente** da fila e copia seus dados para `*out`.

```c
int fila_desenfileirar(Fila *f, Cliente *out) {
    if (!f->frente) return 0;
    FilaNode *node = f->frente;
    *out           = node->cliente;
    f->frente      = node->prox;
    if (!f->frente) f->tras = NULL;
    free(node);
    f->tamanho--;
    return 1;
}
```

| Retorno | Significado |
|---------|-------------|
| `1` | Cliente removido com sucesso |
| `0` | Fila vazia |

#### `int fila_vazia(Fila *f)`

```c
int fila_vazia(Fila *f) {
    return f->frente == NULL;
}
```

| Retorno | Significado |
|---------|-------------|
| `1` (true) | Fila vazia |
| `0` (false) | Fila com elementos |

### Complexidade Big-O

| Operacao | Complexidade |
|----------|-------------|
| `fila_enfileirar` | O(1) |
| `fila_desenfileirar` | O(1) |
| `fila_vazia` | O(1) |
| `fila_destruir` | O(n) |

---

## 7. pilha.h/c — Pilha Dinamica (LIFO)

**Arquivos:** `src/estruturas/pilha.h`, `src/estruturas/pilha.c`

### Conceito

A **Pilha** e uma estrutura de dados que segue o principio **LIFO** (Last In, First Out): o ultimo elemento inserido e o primeiro a ser removido. Analogamente a uma pilha de pratos ou ao carrinho de compras (o ultimo produto bipado e o primeiro a ser desfeito).

A implementacao e feita com **nos encadeados**, mantendo sempre uma referencia ao **topo** da pilha.

```
Diagrama:

Pilha (carrinho)
+--------+--------+
| Produto| prox   |---+
| "Pao"  |        |   |
+--------+--------+   |     +--------+--------+
                      +---->| Produto| prox   |---+
                      TOPO  | "Leite"|        |   |
                            +--------+--------+   |
                                                  +----> ...
                                                        |
                                                        v
                                                      NULL (base)
```

### Structs

```c
typedef struct PilhaNode {
    Produto           produto;
    struct PilhaNode *prox;
} PilhaNode;

typedef struct {
    PilhaNode *topo;    /* ponteiro para o elemento do topo */
    int        tamanho; /* numero de itens no carrinho      */
    float      total;   /* soma dos precos de todos os itens */
} Pilha;
```

O campo `total` e mantido incrementalmente para evitar percorrer toda a pilha para calcular o valor total.

### Funcoes

#### `Pilha *pilha_criar(void)`

```c
Pilha *pilha_criar(void) {
    Pilha *p   = (Pilha *)malloc(sizeof(Pilha));
    p->topo    = NULL;
    p->tamanho = 0;
    p->total   = 0.0f;
    return p;
}
```

#### `void pilha_destruir(Pilha *p)`

Chama `pilha_limpar` (que remove todos os elementos) e libera a struct.

```c
void pilha_destruir(Pilha *p) {
    pilha_limpar(p);
    free(p);
}
```

#### `void pilha_empilhar(Pilha *p, Produto prod)`

Insere um produto no **topo** da pilha. Atualiza o total somando o preco.

```c
void pilha_empilhar(Pilha *p, Produto prod) {
    PilhaNode *novo = (PilhaNode *)malloc(sizeof(PilhaNode));
    novo->produto   = prod;
    novo->prox      = p->topo;
    p->topo         = novo;
    p->tamanho++;
    p->total += prod.preco;
}
```

#### `int pilha_desempilhar(Pilha *p, Produto *out)`

Remove o produto do **topo** da pilha e copia seus dados para `*out`. Atualiza o total subtraindo o preco.

```c
int pilha_desempilhar(Pilha *p, Produto *out) {
    if (!p->topo) return 0;
    PilhaNode *node = p->topo;
    *out            = node->produto;
    p->topo         = node->prox;
    free(node);
    p->tamanho--;
    p->total -= out->preco;
    if (p->total < 0.0f) p->total = 0.0f;
    return 1;
}
```

| Retorno | Significado |
|---------|-------------|
| `1` | Produto removido com sucesso |
| `0` | Pilha vazia |

#### `int pilha_vazia(Pilha *p)`

```c
int pilha_vazia(Pilha *p) {
    return p->topo == NULL;
}
```

#### `void pilha_limpar(Pilha *p)`

Remove todos os elementos da pilha sem copia-los (utilizado ao finalizar ou cancelar uma venda).

```c
void pilha_limpar(Pilha *p) {
    Produto tmp;
    while (!pilha_vazia(p)) pilha_desempilhar(p, &tmp);
}
```

### Complexidade Big-O

| Operacao | Complexidade |
|----------|-------------|
| `pilha_empilhar` | O(1) |
| `pilha_desempilhar` | O(1) |
| `pilha_vazia` | O(1) |
| `pilha_limpar` | O(n) |
| `pilha_destruir` | O(n) |

---

## 8. bst.h/c — Arvore Binaria de Busca (BST)

**Arquivos:** `src/estruturas/bst.h`, `src/estruturas/bst.c`

### Conceito

A **Arvore Binaria de Busca** (Binary Search Tree — BST) e uma estrutura de dados hierarquica onde cada no possui no maximo dois filhos: `esq` (esquerda) e `dir` (direita). A propriedade fundamental da BST e:

- Todos os elementos na **subarvore esquerda** de um no possuem chave **menor** que a chave do no
- Todos os elementos na **subarvore direita** de um no possuem chave **maior** que a chave do no

No projeto, a BST armazena o **historico de vendas** ordenado pelo `id_venda`. O campo `proximo_id` da struct `BST` serve como contador auto-incrementado para gerar novos IDs.

```
Diagrama:

         BST
      +--------+
      |  raiz  |--+
      | prox=5 |  |
      | total=4|  |
      +--------+  |
                  v
            +----------+
            | Venda #3 |
            | "Carlos" |
            | R$ 45.90 |
            +----------+
            /          \
           v            v
    +----------+   +----------+
    | Venda #1 |   | Venda #4 |
    | "Ana"    |   | "Maria"  |
    | R$ 23.50 |   | R$ 87.20 |
    +----------+   +----------+
         \
          v
    +----------+
    | Venda #2 |
    | "Pedro"  |
    | R$ 12.00 |
    +----------+
```

### Structs

```c
typedef struct BSTNode {
    Venda          venda;
    struct BSTNode *esq;
    struct BSTNode *dir;
} BSTNode;

typedef struct {
    BSTNode *raiz;        /* ponteiro para a raiz da arvore         */
    int      proximo_id;  /* proximo ID disponivel para nova venda */
    int      total;       /* numero total de vendas na arvore      */
} BST;
```

### Funcoes

#### `BST *bst_criar(void)`

```c
BST *bst_criar(void) {
    BST *b        = (BST *)malloc(sizeof(BST));
    b->raiz       = NULL;
    b->proximo_id = 1;
    b->total      = 0;
    return b;
}
```

#### `void bst_destruir(BST *b)`

Funcao recursiva que percorre a arvore em **pos-ordem** (esquerda, direita, raiz) e libera cada no, incluindo a lista de `ItemVenda` de cada venda.

```c
static void destruir_no(BSTNode *node) {
    if (!node) return;
    destruir_no(node->esq);
    destruir_no(node->dir);
    ItemVenda *item = node->venda.itens;
    while (item) {
        ItemVenda *prox = item->prox;
        free(item);
        item = prox;
    }
    free(node);
}
```

#### `void bst_inserir(BST *b, Venda v)`

Insere uma venda na arvore usando o `id_venda` como chave. Se a arvore estiver vazia, o novo no torna-se a raiz. Caso contrario, percorre a arvore comparando IDs e insere na posicao correta.

```c
void bst_inserir(BST *b, Venda v) {
    BSTNode *novo = (BSTNode *)malloc(sizeof(BSTNode));
    novo->venda   = v;
    novo->esq     = NULL;
    novo->dir     = NULL;

    if (!b->raiz) {
        b->raiz = novo;
    } else {
        BSTNode *atual = b->raiz;
        while (1) {
            if (v.id_venda < atual->venda.id_venda) {
                if (!atual->esq) { atual->esq = novo; break; }
                atual = atual->esq;
            } else {
                if (!atual->dir) { atual->dir = novo; break; }
                atual = atual->dir;
            }
        }
    }
    b->total++;
}
```

#### `BSTNode *bst_buscar(BST *b, int id)`

Busca uma venda pelo ID. Aproveita a propriedade da BST para decidir a cada no se deve ir para a esquerda ou direita, sem precisar percorrer toda a arvore.

```c
BSTNode *bst_buscar(BST *b, int id) {
    BSTNode *atual = b->raiz;
    while (atual) {
        if (id == atual->venda.id_venda) return atual;
        atual = (id < atual->venda.id_venda) ? atual->esq : atual->dir;
    }
    return NULL;
}
```

#### `void bst_inorder(BSTNode *raiz, Venda *vetor, int *idx))

Percorre a arvore em **ordem simetrica** (in-order: esquerda, raiz, direita), que produz os elementos em ordem crescente de ID. Preenche um vetor previamente alocado.

```c
void bst_inorder(BSTNode *raiz, Venda *vetor, int *idx) {
    if (!raiz) return;
    bst_inorder(raiz->esq, vetor, idx);
    vetor[(*idx)++] = raiz->venda;
    bst_inorder(raiz->dir, vetor, idx);
}
```

#### `Venda *bst_extrair_vetor(BST *b, int *count)`

Extrai todas as vendas da arvore para um vetor alocado com `malloc`. O chamador deve liberar a memoria com `free()`. O vetor sai ordenado por ID (crescente) gracas ao `bst_inorder`.

```c
Venda *bst_extrair_vetor(BST *b, int *count) {
    *count = b->total;
    if (*count == 0) return NULL;
    Venda *v = (Venda *)malloc(sizeof(Venda) * (*count));
    int idx  = 0;
    bst_inorder(b->raiz, v, &idx);
    return v;
}
```

### Complexidade Big-O

| Operacao | Caso Medio | Pior Caso |
|----------|-----------|-----------|
| `bst_inserir` | O(log n) | O(n) |
| `bst_buscar` | O(log n) | O(n) |
| `bst_inorder` | O(n) | O(n) |
| `bst_extrair_vetor` | O(n) | O(n) |

O pior caso O(n) ocorre quando a arvore torna-se degenerada (essencialmente uma lista encadeada), o que acontece se os IDs forem inseridos em ordem estritamente crescente ou decrescente. Como os IDs sao auto-incrementados (`proximo_id`), de fato a arvore cresce sempre para a direita, resultando em pior caso para insercao e busca.

---

## 9. sort.h/c — Quick Sort

**Arquivos:** `src/estruturas/sort.h`, `src/estruturas/sort.c`

### Conceito

O **Quick Sort** e um algoritmo de ordenacao eficiente baseado no paradigma **dividir para conquistar**. Funciona selecionando um elemento como **pivo** e particionando o vetor em duas partes: elementos maiores (ou iguais) que o pivo ficam a esquerda, e elementos menores ficam a direita (para ordenacao decrescente). O processo e aplicado recursivamente a cada particao.

### Funcoes

#### `static void trocar(Venda *a, Venda *b)`

Troca dois elementos de posicao no vetor.

```c
static void trocar(Venda *a, Venda *b) {
    Venda tmp = *a;
    *a = *b;
    *b = tmp;
}
```

#### `static int particionar(Venda *v, int low, int high)`

Funcao de particionamento de **Lomuto**. Escolhe o ultimo elemento como pivo (`v[high].valor_total`) e rearranja o vetor de forma que todos os elementos com `valor_total >= pivot` fiquem a esquerda do pivo.

```c
static int particionar(Venda *v, int low, int high) {
    float pivot = v[high].valor_total;
    int   i     = low - 1;
    for (int j = low; j < high; j++) {
        if (v[j].valor_total >= pivot) {
            i++;
            trocar(&v[i], &v[j]);
        }
    }
    trocar(&v[i + 1], &v[high]);
    return i + 1;
}
```

#### `void quicksort_vendas_desc(Venda *v, int low, int high)`

Funcao principal que ordena o vetor de vendas em ordem **decrescente** de `valor_total`.

```c
void quicksort_vendas_desc(Venda *v, int low, int high) {
    if (low < high) {
        int pi = particionar(v, low, high);
        quicksort_vendas_desc(v, low, pi - 1);
        quicksort_vendas_desc(v, pi + 1, high);
    }
}
```

### Complexidade Big-O

| Caso | Complexidade |
|------|-------------|
| Melhor caso | O(n log n) |
| Caso medio | O(n log n) |
| Pior caso | O(n²) |

---

## 10. dados.h/c — Geracao e Leitura de Arquivos

**Arquivos:** `src/estruturas/dados.h`, `src/estruturas/dados.c`

### Conceito

Este modulo e responsavel por:
1. **Gerar** arquivos de dados sinteticos (1000 produtos e 1000 clientes) caso nao existam
2. **Carregar** esses arquivos para as estruturas de dados (Hash e Fila) na inicializacao

### Geracao de Produtos

A funcao `gerar_produtos_txt` combina dois vetores de strings — `nomes_base` (96 itens como "Arroz", "Feijao", "Acucar") e `complementos` (23 itens como "Tipo 1", "1kg", "Premium") — para gerar 1000 produtos unicos no formato:

```
<codigo>;<nome>;<preco>;<quantidade>;<dia>;<mes>;<ano>
```

Exemplo de linha gerada:
```
1042;Leite Integral 1L;5.75;120;15;03;2026
```

O precos sao gerados aleatoriamente entre R$ 2,50 e R$ 100,00. As quantidades entre 10 e 200.

### Geracao de Clientes

A funcao `gerar_clientes_txt` combina nomes (50 opcoes) e sobrenomes (25 opcoes) para gerar 1000 clientes no formato:

```
<nome> <sobrenome>;<dia>;<mes>;<ano>
```

Exemplo:
```
Ana Silva;12;04;2026
```

### Gerador Pseudoaleatorio

O modulo implementa seu proprio gerador pseudoaleatorio (gerador linear congruencial) para nao depender da funcao `rand()` da biblioteca padrao, garantindo que os dados gerados sejam deterministas (semente fixa).

```c
static unsigned int _seed = 42;
static int _rand(void) {
    _seed = _seed * 1664525u + 1013904223u;
    return (int)((_seed >> 1) & 0x7FFFFFFF);
}
```

### Carregamento dos Dados

#### `int carregar_produtos(const char *caminho, TabelaHash *hash)`

Le o arquivo `produtos.txt` linha a linha usando `fgets` e `sscanf`, inserindo cada produto na tabela hash.

```c
int carregar_produtos(const char *caminho, TabelaHash *hash) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;
    int  count = 0;
    char linha[MAX_TEXTO];
    while (fgets(linha, sizeof(linha), f)) {
        Produto p;
        if (sscanf(linha, "%d;%99[^;];%f;%d;%d;%d;%d",
                   &p.codigo, p.nome, &p.preco, &p.quantidade,
                   &p.dia, &p.mes, &p.ano) == 7) {
            hash_inserir(hash, p);
            count++;
        }
    }
    fclose(f);
    return count;
}
```

#### `int carregar_clientes(const char *caminho, Fila *fila)`

Le o arquivo `clientes.txt` linha a linha e enfileira cada cliente.

```c
int carregar_clientes(const char *caminho, Fila *fila) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;
    int  count = 0;
    char linha[MAX_TEXTO];
    while (fgets(linha, sizeof(linha), f)) {
        Cliente c;
        if (sscanf(linha, "%99[^;];%d;%d;%d",
                   c.nome, &c.dia, &c.mes, &c.ano) == 4) {
            fila_enfileirar(fila, c);
            count++;
        }
    }
    fclose(f);
    return count;
}
```

---

## 11. estado.h — Estado Global da Aplicacao

**Arquivo:** `src/estado.h`

### Conceito

A struct `EstadoApp` funciona como **contexto global** da aplicacao, passado por ponteiro para todas as funcoes de desenho das telas. Centraliza:

- Ponteiros para as quatro estruturas de dados
- Estado de navegacao (qual tela esta ativa)
- Cliente em atendimento no caixa
- Mensagem de status para feedback ao usuario
- Dimensoes atuais da janela

### Struct

```c
typedef struct {
    TabelaHash *estoque;
    Fila       *fila_clientes;
    Pilha      *carrinho;
    BST        *historico;

    Tela tela_atual;

    bool    tem_cliente;
    Cliente cliente_atual;

    char  msg_status[256];
    float msg_timer;
    bool  msg_erro;

    int largura;
    int altura;
} EstadoApp;
```

### Funcao Inline: `estado_msg`

Define uma mensagem de feedback que aparece no rodape da tela por 3 segundos.

```c
static inline void estado_msg(EstadoApp *app, const char *msg, bool erro) {
    int i = 0;
    while (msg[i] && i < 255) { app->msg_status[i] = msg[i]; i++; }
    app->msg_status[i] = '\0';
    app->msg_timer     = 3.0f;
    app->msg_erro      = erro;
}
```

---

## 12. ui.h/c — Componentes Visuais (Raylib)

**Arquivos:** `src/interface/ui.h`, `src/interface/ui.c`

### Conceito

O modulo `ui` implementa componentes de interface grafica reutilizaveis sobre a biblioteca Raylib, incluindo:

- Paleta de cores padronizada (verde escuro, azul, laranja, vermelho, roxo)
- Campo de texto editavel (input)
- Botao com hover e sombra
- Card (painel com borda e sombra)
- Barra de cabecalho (header)
- Mensagem de status (banner inferior)
- Scroll com mouse wheel
- Texto centralizado

### Paleta de Cores

```c
#define COR_FUNDO       CLITERAL(Color){ 235, 240, 235, 255 }  /* verde claro */
#define COR_PRIMARIA    CLITERAL(Color){  27,  94,  32, 255 }  /* verde escuro */
#define COR_ACENTO      CLITERAL(Color){  56, 142,  60, 255 }  /* verde medio */
#define COR_HOVER       CLITERAL(Color){  46, 125,  50, 255 }  /* hover */
#define COR_PERIGO      CLITERAL(Color){ 183,  28,  28, 255 }  /* vermelho */
#define COR_AVISO       CLITERAL(Color){ 230,  81,   0, 255 }  /* laranja */
#define COR_INFO        CLITERAL(Color){  13,  71, 161, 255 }  /* azul */
#define COR_ROXO        CLITERAL(Color){  74,  20, 140, 255 }  /* roxo */
#define COR_CARD        CLITERAL(Color){ 255, 255, 255, 255 }  /* branco */
#define COR_TEXTO       CLITERAL(Color){  33,  33,  33, 255 }  /* preto */
#define COR_SUBTEXTO    CLITERAL(Color){  97,  97,  97, 255 }  /* cinza */
```

### Componentes

#### CampoTexto

```c
typedef struct {
    char  text[256];
    int   len;
    bool  ativo;
    Rectangle bounds;
    char  placeholder[64];
    bool  so_numeros;
} CampoTexto;
```

Funcionalidades:
- `campo_init` — inicializa com placeholder e tipo (numerico ou texto)
- `campo_atualizar` — processa clique (ativa/desativa), backspace e digitacao
- `campo_desenhar` — renderiza com borda, placeholder, texto e cursor piscante
- `campo_limpar` — reseta o campo para vazio

Quando `so_numeros = true`, apenas caracteres '0'-'9' sao aceitos. O cursor pisca com frequencia de 2 Hz usando `GetTime()`.

#### Botao

```c
bool ui_botao(Rectangle r, const char *txt, Color cor,
              Color cor_hover, Color cor_txt, int fs);
```

Renderiza um botao com fundo colorido, realce no hover e sombra sutil. Retorna `true` se foi clicado no frame atual.

#### Card

```c
void ui_card(Rectangle r, Color cor);
```

Renderiza um painel retangular com borda arredondada (via `DrawRectangle` com sombra deslocada 3px).

#### Header

```c
void ui_header(const char *titulo, Color cor);
```

Barra superior com 60px de altura, titulo centralizado em branco e linha de realce inferior.

#### Status

```c
void ui_status(const char *msg, bool erro, float *timer);
```

Banner de 44px no rodape da janela. Usa fundo verde sucesso (COR_SUCESSO_BG) ou vermelho erro (COR_ERRO_BG). Desaparece apos `*timer` segundos.

#### Scroll

```c
void ui_scroll_ajustar(float *offset, float conteudo_h, float area_h);
```

Ajusta o deslocamento vertical baseado no movimento da roda do mouse (`GetMouseWheelMove`). Mantem o offset dentro dos limites validos.

---

## 13. main.c — Ponto de Entrada e Loop Principal

**Arquivo:** `src/main.c`

> **Se voce nunca viu um loop principal do Raylib antes, leia a Secao 1.5.5 antes de continuar.**

### Fluxo de Execucao

```
Inicio
  |
  v
Inicializar janela Raylib (1280x720, redimensionavel, MSAA)
  |
  v
Inicializar EstadoApp (memset zero)
  |
  v
Alocar estruturas de dados:
  -> hash_criar(HASH_TAMANHO)
  -> fila_criar()
  -> pilha_criar()
  -> bst_criar()
  |
  v
Carregar/Gerar dados:
  -> Se produtos.txt nao existe: gerar_produtos_txt()
  -> Se clientes.txt nao existe: gerar_clientes_txt()
  -> carregar_produtos() para a Hash
  -> carregar_clientes() para a Fila
  |
  v
Inicializar estados das telas (init)
  |
  v
Loop principal (enquanto janela aberta):
  -> Atualizar dimensoes (redimensionamento)
  -> BeginDrawing()
  -> Switch(app.tela_atual):
       TELA_MENU      -> tela_menu_desenhar()
       TELA_CADASTRO  -> tela_cadastro_desenhar()
       TELA_CAIXA     -> tela_caixa_desenhar()
       TELA_RELATORIOS -> tela_relatorios_desenhar()
       TELA_AUDITORIA  -> tela_auditoria_desenhar()
  -> EndDrawing()
  |
  v
Sair do loop (WindowShouldClose)
  |
  v
Liberar memoria:
  -> tela_relatorios_limpar()
  -> hash_destruir()
  -> fila_destruir()
  -> pilha_destruir()
  -> bst_destruir()
  -> CloseWindow()
  |
  v
Fim
```

### Inicializacao da Janela

```c
SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
InitWindow(1280, 720, "Mercadinho - Sistema de Gerenciamento | ED");
SetTargetFPS(60);
```

A janela e inicializada com 1280x720, redimensionavel (FLAG_WINDOW_RESIZABLE) e com anti-aliasing multisample 4x (FLAG_MSAA_4X_HINT).

### Carregamento Condicional

```c
FILE *fp = fopen(arq_produtos, "r");
if (!fp) {
    gerar_produtos_txt(arq_produtos);
} else {
    fclose(fp);
}
```

Se os arquivos de dados nao existirem no diretorio `data/`, eles sao gerados automaticamente com 1000 registros cada. O programa entao carrega os dados para as estruturas apropriadas e exibe um resumo:

```
[INFO] 1000 produtos carregados na Hash.
[INFO] 1000 clientes carregados na Fila.
```

---

## 14. tela_menu.c — Dashboard / Menu Principal

**Arquivos:** `src/interface/tela_menu.h`, `src/interface/tela_menu.c`

### Conceito

A tela de menu funciona como um **dashboard** que exibe indicadores numericos do estado atual do sistema e oferece botoes de navegacao para as demais funcionalidades.

### Layout

```
+----------------------------------------------------------+
|  MERCADINHO | Sistema de Gerenciamento          [HEADER] |
+----------------------------------------------------------+
| +----------+  +----------+  +----------+                  |
| | Clientes |  | Produtos |  | Vendas   |                  |
| | na Fila  |  | no Estq. |  | Realiz.  |                  |
| |    42    |  |   1000   |  |    15    |                  |
| +----------+  +----------+  +----------+                  |
|                                                           |
| +------------------+  +------------------+                |
| | FRENTE DE CAIXA  |  | CADASTRO DE      |                |
| | Atender clientes |  | PRODUTOS         |                |
| | e finalizar      |  | Inserir novos    |                |
| | vendas           |  | itens no estoque |                |
| +------------------+  +------------------+                |
|                                                           |
| +------------------+  +------------------+                |
| | RELATORIOS       |  | AUDITORIA /      |                |
| | GERENCIAIS       |  | CONSULTA         |                |
| | Ranking de       |  | Buscar venda     |                |
| | faturamento      |  | por ID           |                |
| +------------------+  +------------------+                |
+----------------------------------------------------------+
```

### Botoes de Navegacao

| Botao | Cor | Tela Destino |
|-------|-----|-------------|
| FRENTE DE CAIXA | Verde escuro | `TELA_CAIXA` |
| CADASTRO DE PRODUTOS | Azul | `TELA_CADASTRO` |
| RELATORIOS GERENCIAIS | Laranja | `TELA_RELATORIOS` |
| AUDITORIA / CONSULTA | Roxo | `TELA_AUDITORIA` |

### Indicadores

- **Clientes na Fila** — `app->fila_clientes->tamanho` (quantos clientes aguardam)
- **Produtos no Estoque** — `app->estoque->total` (quantos produtos cadastrados)
- **Vendas Realizadas** — `app->historico->total` (quantas vendas foram finalizadas)

---

## 15. tela_cadastro.c — Cadastro de Produtos

**Arquivos:** `src/interface/tela_cadastro.h`, `src/interface/tela_cadastro.c`

### Conceito

Tela para cadastrar novos produtos no estoque (tabela hash) ou consultar produtos existentes.

### Layout

```
+----------------------------------------------------------+
|  CADASTRO DE PRODUTOS                           [HEADER] |
+----------------------------------------------------------+
| [< VOLTAR]                                                 |
|                                                           |
| +------------------------------------------------------+ |
| | Codigo:        [___________]                         | |
| | Nome:          [___________________________]         | |
| | Preco (R$):    [___________]                         | |
| | Quantidade:    [___________]                         | |
| | Data:          [DD] / [MM] / [AAAA]                  | |
| |                                                      | |
| | [ CADASTRAR ]                                        | |
| +------------------------------------------------------+ |
|                                                           |
| +------------------------------------------------------+ |
| | Consulta Rapida por Codigo:                          | |
| | [________] [ BUSCAR ]                                | |
| | [1001] Arroz Tipo 1 5kg | R$ 15.90 | Qtd: 50       | |
| +------------------------------------------------------+ |
|                                                           |
| Status: Produto cadastrado com sucesso!          [BANNER] |
+----------------------------------------------------------+
```

### Funcionalidades

1. **Campos do formulario** com validacao individual
2. **Data pre-preenchida** com a data atual do sistema (via `time()` e `localtime()`)
3. **Botao CADASTRAR** que chama `hash_inserir()` — se o codigo ja existir, o produto e atualizado
4. **Painel de Consulta Rapida** que chama `hash_buscar()` e exibe o resultado
5. **Mensagens de status** para feedback (sucesso/erro)

### Validacoes

- Campos obrigatorios: codigo, nome, preco, quantidade
- Codigo e quantidade: apenas numeros (campo `so_numeros = true`)
- Valores numericos devem ser positivos

---

## 16. tela_caixa.c — Frente de Caixa

**Arquivos:** `src/interface/tela_caixa.h`, `src/interface/tela_caixa.c`

### Conceito

Tela principal de operacao do sistema, dividida em tres paineis laterais que implementam o fluxo de atendimento.

### Layout

```
+----------------------------------------------------------+
|  FRENTE DE CAIXA | ATENDIMENTO                  [HEADER] |
+----------------------------------------------------------+
| [< VOLTAR]                                                 |
|                                                           |
| +----------+ +----------+ +------------------+           |
| | CLIENTE  | | BIPE DE  | | RESUMO DA VENDA |           |
| | ATEND.   | | PRODUTOS | |                 |           |
| |          | |          | | 3 item(ns)      |           |
| | Maria    | | Codigo:  | | TOTAL:          |           |
| | 15/03/26 | | [___]    | |   R$ 45,90      |           |
| |          | | [BIPAR]  | |                 |           |
| | [PROXIMO | |          | | [DESFAZER]      |           |
| | CLIENTE] | | 1. Pao   | |                 |           |
| |          | | 2. Leite | | [FINALIZAR]     |           |
| | Fila: 42 | | 3. Cafe  | |                 |           |
| |          | |          | | [CANCELAR]      |           |
| +----------+ +----------+ +------------------+           |
|                                                           |
| Status: Venda #5 finalizada! Total: R$ 45.90   [BANNER] |
+----------------------------------------------------------+
```

### Paineis

#### Painel Esquerdo: Cliente em Atendimento (largura 420px)

- Exibe o nome e a data de registro do cliente atual
- Botao **PROXIMO CLIENTE**: desenfileira da fila (`fila_desenfileirar`) e limpa o carrinho
- Indicador de quantos clientes ainda estao na fila

#### Painel Central: Bipe de Produtos (largura 420px)

- Campo de codigo com suporte a Enter (tecla de atalho)
- Botao **BIPAR**: chama `hash_atualizar_quantidade(codigo, -1)` para dar baixa no estoque e `pilha_empilhar()` para adicionar ao carrinho
- Lista do carrinho com scroll implicito (ate o espaco disponivel)

#### Painel Direito: Resumo da Venda (largura variavel)

- Exibe numero de itens e valor total (mantido incrementalmente na Pilha)
- Botao **DESFAZER ULTIMO ITEM**: `pilha_desempilhar()` e `hash_atualizar_quantidade(codigo, +1)` para estorno
- Botao **FINALIZAR VENDA**: constroi uma struct `Venda`, copia os itens da pilha para uma lista encadeada (`ItemVenda`), insere na BST (`bst_inserir`) e limpa o carrinho
- Botao **CANCELAR VENDA**: estorna todos os itens e libera o cliente

---

## 17. tela_relatorios.c — Relatorios Gerenciais

**Arquivos:** `src/interface/tela_relatorios.h`, `src/interface/tela_relatorios.c`

### Conceito

Tela que exibe relatorios sobre as vendas realizadas. Oferece duas modalidades de visualizacao.

### Estados

```c
typedef enum {
    REL_NENHUM = 0,
    REL_INORDER,
    REL_FATURAMENTO
} ModoRelatorio;

typedef struct {
    ModoRelatorio modo;
    Venda        *vetor;      /* vetor alocado com malloc */
    int           count;
    float         scroll;
} TelaRelatoriosState;
```

### Layout

```
+----------------------------------------------------------+
|  RELATORIOS GERENCIAIS                          [HEADER] |
+----------------------------------------------------------+
| [< VOLTAR]                                                 |
|                                                           |
| [ RELATORIO POR ID    ] [ RANKING DE FATURAMENTO  ]      |
|   (EM-ORDEM)                (DECRESCENTE)                 |
|                                                           |
| +------------------------------------------------------+ |
| | Pos   ID     Cliente          Data     Itens  Total  | |
| | 1     #0005  Carlos Silva   15/03/26    5    R$ 87.20| |
| | 2     #0003  Ana Oliveira   14/03/26    3    R$ 45.90| |
| | 3     #0001  Maria Souza    12/03/26    1    R$ 23.50| |
| | ...                                                   | |
| +------------------------------------------------------+ |
|                                         Total: 12 venda(s) |
+----------------------------------------------------------+
```

### Funcionalidades

- **Relatorio por ID (Em-Ordem)**: Extrai as vendas da BST com `bst_extrair_vetor()` e as exibe ordenadas por ID (crescente, pois o in-order ja produz essa ordem)
- **Ranking de Faturamento (Decrescente)**: Extrai as vendas e aplica `quicksort_vendas_desc()` para ordenar do maior para o menor valor total
- Scroll com a roda do mouse, usando `BeginScissorMode()` para clipping

### Gerenciamento de Memoria

O vetor de vendas e alocado com `malloc` a cada nova consulta e liberado com `free()` quando:
- O usuario troca o tipo de relatorio
- O usuario volta para o menu
- O programa e encerrado (`tela_relatorios_limpar` e chamado em `main.c`)

---

## 18. tela_auditoria.c — Auditoria / Consulta de Vendas

**Arquivos:** `src/interface/tela_auditoria.h`, `src/interface/tela_auditoria.c`

### Conceito

Tela que permite consultar uma venda especifica pelo seu ID e visualizar o cupom fiscal completo com todos os itens.

### Layout

```
+----------------------------------------------------------+
|  AUDITORIA | CONSULTA DE VENDAS                 [HEADER] |
+----------------------------------------------------------+
| [< VOLTAR]                                                 |
|                                                           |
| ID da Venda: [____] [ BUSCAR ]  (ou pressione Enter)     |
|                                                           |
| +------------------------------------------------------+ |
| |              CUPOM FISCAL  #0005                      | |
| | ----------------------------------------------------- | |
| | Cliente:  Carlos Silva                                | |
| | Data:     15/03/2026                                  | |
| | Itens:    5 produto(s)                                | |
| | ----------------------------------------------------- | |
| | Produto              Codigo    Preco                  | |
| | ----------------------------------------------------- | |
| | Arroz Tipo 1 5kg     1001     R$ 15.90               | |
| | Leite Integral 1L    1042     R$ 5.75                | |
| | Cafe Moido           1058     R$ 12.50               | |
| | ...                                                   | |
| | ----------------------------------------------------- | |
| | TOTAL:                                   R$ 87.20    | |
| +------------------------------------------------------+ |
+----------------------------------------------------------+
```

### Funcionalidades

- Campo de ID com suporte a Enter
- Botao **BUSCAR** que chama `bst_buscar(app->historico, id)`
- Exibicao detalhada da venda: cliente, data, numero de itens
- Listagem de cada item com codigo e preco
- Valor total em destaque
- Mensagem de erro se o ID nao for encontrado

---

## 19. Fluxo Completo de uma Venda

Abaixo esta o caminho percorrido por uma venda desde a inicializacao do sistema ate a finalizacao:

### Passo 1: Inicializacao (main.c)

1. Programa inicia, carrega 1000 produtos na Tabela Hash e 1000 clientes na Fila
2. Tela inicial: Dashboard (TELA_MENU)

### Passo 2: Navegacao

3. Operador clica em "FRENTE DE CAIXA"
4. `app->tela_atual = TELA_CAIXA`

### Passo 3: Chamar Cliente

5. Operador clica em "PROXIMO CLIENTE"
6. `fila_desenfileirar()` remove o cliente da **Fila** (FIFO)
7. Cliente copiado para `app->cliente_atual`, `app->tem_cliente = true`
8. Carrinho anterior e limpo com `pilha_limpar()`

### Passo 4: Bipar Produtos

9. Operador digita o codigo do produto e clica "BIPAR" (ou Enter)
10. `hash_atualizar_quantidade(codigo, -1)` da baixa no estoque da **Hash**
11. `pilha_empilhar()` adiciona o produto ao **carrinho** (Pilha LIFO)
12. Carrinho total e atualizado incrementalmente (`p->total += prod.preco`)
13. O produto aparece na lista do carrinho no painel central

### Passo 5: Repetir passo 4 para cada produto

### Passo 6: Finalizar Venda

14. Operador clica "FINALIZAR VENDA"
15. Uma struct `Venda` e construida:
    - `id_venda = app->historico->proximo_id++`
    - `cliente = app->cliente_atual.nome`
    - `valor_total = app->carrinho->total`
    - `itens` = lista encadeada copiada da pilha
16. `bst_inserir(app->historico, v)` adiciona a venda na **BST**
17. `pilha_limpar(app->carrinho)` esvazia o carrinho
18. `app->tem_cliente = false` libera o cliente

### Alternativa: Cancelar Venda

14b. Operador clica "CANCELAR VENDA"
15b. Loop: `pilha_desempilhar()` + `hash_atualizar_quantidade(codigo, +1)` estorna todos os itens
16b. `app->tem_cliente = false`

### Passo 7: Consulta Posterior (Auditoria)

19. No menu, operador clica "AUDITORIA / CONSULTA"
20. Digita o ID da venda e clica "BUSCAR"
21. `bst_buscar(app->historico, id)` encontra a venda na **BST**
22. Cupom fiscal completo e exibido com todos os itens

### Passo 8: Relatorios

23. No menu, operador clica "RELATORIOS GERENCIAIS"
24. Para o ranking: `bst_extrair_vetor()` extrai todas as vendas, `quicksort_vendas_desc()` ordena por valor total decrescente

---

## 20. Gerenciamento de Memoria

> **Se voce nunca viu `malloc`/`calloc`/`free` antes, leia a Secao 1.5.4 antes de continuar.**

### Alocacao

| Estrutura | Criacao | Responsabilidade |
|-----------|---------|------------------|
| `TabelaHash` | `hash_criar()` | main.c (inicio) |
| `Fila` | `fila_criar()` | main.c (inicio) |
| `Pilha` | `pilha_criar()` | main.c (inicio) |
| `BST` | `bst_criar()` | main.c (inicio) |
| `Venda*` (vetor) | `bst_extrair_vetor()` | tela_relatorios.c |
| `ItemVenda` | Finalizacao de venda | tela_caixa.c |

### Liberacao

| Estrutura | Destruicao | Responsabilidade |
|-----------|-----------|------------------|
| `TabelaHash` | `hash_destruir()` | main.c (fim) |
| `Fila` | `fila_destruir()` | main.c (fim) |
| `Pilha` | `pilha_destruir()` | main.c (fim) |
| `BST` | `bst_destruir()` (libera nos + ItemVenda internos) | main.c (fim) |
| `Venda*` (vetor) | `free(st->vetor)` | tela_relatorios_limpar() |

### Detalhamento

1. **Hash**: `hash_destruir()` percorre todas as 1009 posicoes, libera cada no das listas encadeadas, libera o vetor de ponteiros e a struct.
2. **Fila**: `fila_destruir()` desenfileira todos os elementos ate a fila ficar vazia, depois libera a struct.
3. **Pilha**: `pilha_limpar()` desempilha todos os elementos (usado a cada finalizacao de venda), e `pilha_destruir()` chama `pilha_limpar()` e depois libera a struct.
4. **BST**: `bst_destruir()` chama a funcao recursiva `destruir_no()` que percorre a arvore em pos-ordem. Para cada no, tambem percorre a lista de `ItemVenda` da venda e libera cada item.
5. **Vetor de relatorio**: Alocado em `bst_extrair_vetor()` e liberado em `tela_relatorios_limpar()`, que e chamada ao trocar o tipo de relatorio, ao voltar ao menu e ao encerrar o programa.

### Pontos de Atencao

- **Vazamento zero**: Todas as alocacoes possuem `free()` correspondente
- **ItemVenda na BST**: A lista de itens de cada venda e copiada da pilha (que e limpa logo apos) e armazenada no no da BST. A responsabilidade de libera-la e da `bst_destruir()`.
- **Double free**: Nao ocorre porque a pilha e limpa apos a copia para a venda, e os itens na BST sao liberados apenas na destruicao.

---

## 21. Como Compilar e Executar

### Pre-requisitos (Windows)

1. **w64devkit** — Kit de desenvolvimento MinGW-w64 para Windows
   - Download: https://github.com/skeeto/w64devkit/releases
   - Extrair para `C:\w64devkit`

2. **Raylib 5.5** — Biblioteca grafica
   - Download: https://github.com/raysan5/raylib/releases (pacote `raylib-5.5_win64_mingw-w64.zip`)
   - Extrair para `C:\raylib`
   - Deve conter: `C:\raylib\include\raylib.h` e `C:\raylib\lib\libraylib.a`

### Compilacao Manual

```batch
set W64=C:\w64devkit\bin
set RAYLIB_INC=C:\raylib\include
set RAYLIB_LIB=C:\raylib\lib

"%W64%\gcc.exe" -std=c99 -Wall -O2 ^
    -I"%RAYLIB_INC%" ^
    -Isrc ^
    -o mercadinho.exe ^
    src\main.c ^
    src\estruturas\hash.c ^
    src\estruturas\fila.c ^
    src\estruturas\pilha.c ^
    src\estruturas\bst.c ^
    src\estruturas\sort.c ^
    src\estruturas\dados.c ^
    src\interface\ui.c ^
    src\interface\tela_menu.c ^
    src\interface\tela_cadastro.c ^
    src\interface\tela_caixa.c ^
    src\interface\tela_relatorios.c ^
    src\interface\tela_auditoria.c ^
    -L"%RAYLIB_LIB%" -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
```

### Compilacao Automatica

Execute `build.bat` (se o ambiente ja estiver configurado) ou `instalar_e_compilar.bat` (que baixa as dependencias automaticamente).

```batch
.\build.bat
```

### Execucao

O executavel `mercadinho.exe` sera gerado na raiz do projeto. Execute-o diretamente:

```batch
.\mercadinho.exe
```

Os arquivos `data/produtos.txt` e `data/clientes.txt` serao gerados automaticamente na primeira execucao caso nao existam.

---

## 22. Conceitos de Estrutura de Dados — Resumo

### 22.1 Tabela Hash

| Aspecto | Descricao |
|---------|-----------|
| **Definicao** | Estrutura que mapeia chaves a valores usando funcao de espalhamento |
| **Aplicacao** | Estoque de produtos (chave = codigo do produto) |
| **Tratamento de colisoes** | Encadeamento externo (separate chaining) |
| **Funcao hash** | `codigo % HASH_TAMANHO` (modulo) |
| **Complexidade** | O(1) medio, O(n) pior caso |
| **Operacoes** | inserir, buscar, atualizar_quantidade, destruir |

### 22.2 Fila (FIFO)

| Aspecto | Descricao |
|---------|-----------|
| **Definicao** | First In, First Out — o primeiro a entrar e o primeiro a sair |
| **Aplicacao** | Fila de clientes aguardando atendimento |
| **Implementacao** | Ponteiros encadeados com frente e tras |
| **Complexidade** | O(1) para enfileirar e desenfileirar |
| **Operacoes** | criar, destruir, enfileirar, desenfileirar, vazia |

### 22.3 Pilha (LIFO)

| Aspecto | Descricao |
|---------|-----------|
| **Definicao** | Last In, First Out — o ultimo a entrar e o primeiro a sair |
| **Aplicacao** | Carrinho de compras (desfazer ultimo item) |
| **Implementacao** | Nos encadeados com referencia ao topo |
| **Total incremental** | Campo `total` mantido para O(1) na consulta do valor |
| **Complexidade** | O(1) para empilhar e desempilhar |
| **Operacoes** | criar, destruir, empilhar, desempilhar, limpar, vazia |

### 22.4 Arvore Binaria de Busca (BST)

| Aspecto | Descricao |
|---------|-----------|
| **Definicao** | Arvore binaria onde a subarvore esquerda contem apenas nos menores, e a direita apenas nos maiores |
| **Aplicacao** | Historico de vendas (chave = ID da venda) |
| **Propriedade** | In-order produz elementos em ordem crescente |
| **Complexidade** | O(log n) medio, O(n) pior caso (arvore degenerada) |
| **Operacoes** | criar, destruir, inserir, buscar, inorder, extrair_vetor |

### 22.5 Quick Sort

| Aspecto | Descricao |
|---------|-----------|
| **Definicao** | Algoritmo de ordenacao dividir-para-conquistar com pivo |
| **Aplicacao** | Ordenacao do ranking de faturamento (decrescente) |
| **Particionamento** | Lomuto (pivo = ultimo elemento) |
| **Complexidade** | O(n log n) medio, O(n²) pior caso |
| **Comparacao** | `>=` para ordem decrescente |

### 22.6 Lista Encadeada (ItemVenda)

| Aspecto | Descricao |
|---------|-----------|
| **Definicao** | Estrutura linear onde cada elemento contem um ponteiro para o proximo |
| **Aplicacao** | Itens de uma venda (dentro da BST) |
| **Sentido** | Simples (cada no apena para o proximo) |
| **Criacao** | Durante a finalizacao da venda, copiando os itens da pilha |
| **Liberacao** | Durante a destruicao da BST, percorrendo e liberando cada no |

---

## Apendice: Listagem de Constantes e Macros

| Constante | Valor | Uso |
|-----------|-------|-----|
| `MAX_NOME` | 100 | Tamanho maximo de nomes |
| `MAX_TEXTO` | 256 | Tamanho de buffer para linhas de arquivo |
| `HASH_TAMANHO` | 1009 | Tamanho da tabela hash (numero primo) |
| `JANELA_W` / `JANELA_H` | 1280 / 720 | Dimensoes iniciais da janela |
| `FPS` | 60 | Quadros por segundo alvo |
| `FLAG_WINDOW_RESIZABLE` | - | Permite redimensionar a janela |
| `FLAG_MSAA_4X_HINT` | - | Anti-aliasing 4x |

---

## Apendice: Indice de Arquivos e Funcoes

### `src/types.h`
- `Produto`, `Cliente`, `ItemVenda`, `Venda` (structs)
- `Tela` (enum)

### `src/estado.h`
- `EstadoApp` (struct)
- `estado_msg()` (inline)

### `src/estruturas/hash.h / hash.c`
- `hash_criar()`, `hash_destruir()`, `hash_funcao()`
- `hash_inserir()`, `hash_buscar()`, `hash_atualizar_quantidade()`

### `src/estruturas/fila.h / fila.c`
- `fila_criar()`, `fila_destruir()`
- `fila_enfileirar()`, `fila_desenfileirar()`, `fila_vazia()`

### `src/estruturas/pilha.h / pilha.c`
- `pilha_criar()`, `pilha_destruir()`
- `pilha_empilhar()`, `pilha_desempilhar()`, `pilha_vazia()`, `pilha_limpar()`

### `src/estruturas/bst.h / bst.c`
- `bst_criar()`, `bst_destruir()`, `bst_inserir()`
- `bst_buscar()`, `bst_inorder()`, `bst_extrair_vetor()`

### `src/estruturas/sort.h / sort.c`
- `quicksort_vendas_desc()`

### `src/estruturas/dados.h / dados.c`
- `gerar_produtos_txt()`, `gerar_clientes_txt()`
- `carregar_produtos()`, `carregar_clientes()`
- `_rand()` (static)

### `src/interface/ui.h / ui.c`
- `campo_init()`, `campo_atualizar()`, `campo_desenhar()`, `campo_limpar()`
- `ui_botao()`, `ui_card()`, `ui_header()`
- `ui_botao_voltar()`, `ui_status()`, `ui_texto_centrado()`
- `ui_scroll_ajustar()`

### `src/interface/tela_menu.h / tela_menu.c`
- `tela_menu_desenhar()`

### `src/interface/tela_cadastro.h / tela_cadastro.c`
- `tela_cadastro_init()`, `tela_cadastro_desenhar()`

### `src/interface/tela_caixa.h / tela_caixa.c`
- `tela_caixa_init()`, `tela_caixa_desenhar()`

### `src/interface/tela_relatorios.h / tela_relatorios.c`
- `tela_relatorios_init()`, `tela_relatorios_limpar()`, `tela_relatorios_desenhar()`

### `src/interface/tela_auditoria.h / tela_auditoria.c`
- `tela_auditoria_init()`, `tela_auditoria_desenhar()`

### `src/main.c`
- `main()`

---

*Documentacao gerada automaticamente a partir dos codigos-fonte do projeto Mercadinho.*  
*UNIVALI — 3o Periodo — Estrutura de Dados — 2026*
