# Documentação Completa — Sistema de Gerenciamento de Mercadinho
**Disciplina:** Estrutura de Dados | **Professor:** Welington Gadelha | **UNIVALI 2026**

---

## Sumário

1. [Visão Geral do Projeto](#1-visão-geral-do-projeto)
2. [Estrutura de Arquivos](#2-estrutura-de-arquivos)
3. [Diagrama de Dependências](#3-diagrama-de-dependências)
4. [types.h — Tipos Fundamentais](#4-typesh--tipos-fundamentais)
5. [hash.h / hash.c — Tabela Hash com Encadeamento Externo](#5-hashh--hashc--tabela-hash-com-encadeamento-externo)
6. [fila.h / fila.c — Fila Dinâmica](#6-filah--filac--fila-dinâmica)
7. [pilha.h / pilha.c — Pilha Dinâmica](#7-pilhah--pilhac--pilha-dinâmica)
8. [bst.h / bst.c — Árvore Binária de Busca](#8-bsth--bstc--árvore-binária-de-busca)
9. [sort.h / sort.c — Quick Sort](#9-sorth--sortc--quick-sort)
10. [dados.h / dados.c — Geração e Leitura de Arquivos](#10-dadosh--dadosc--geração-e-leitura-de-arquivos)
11. [estado.h — Estado Global da Aplicação](#11-estadoh--estado-global-da-aplicação)
12. [ui.h / ui.c — Componentes Visuais Reutilizáveis](#12-uih--uic--componentes-visuais-reutilizáveis)
13. [main.c — Ponto de Entrada e Loop Principal](#13-mainc--ponto-de-entrada-e-loop-principal)
14. [tela_menu.c — Menu Principal com Dashboard](#14-tela_menuc--menu-principal-com-dashboard)
15. [tela_cadastro.c — Cadastro de Produtos](#15-tela_cadastroc--cadastro-de-produtos)
16. [tela_caixa.c — Frente de Caixa e Atendimento](#16-tela_caixac--frente-de-caixa-e-atendimento)
17. [tela_relatorios.c — Relatórios Gerenciais](#17-tela_relatoriosc--relatórios-gerenciais)
18. [tela_auditoria.c — Auditoria e Consulta](#18-tela_auditoriac--auditoria-e-consulta)
19. [Fluxo Completo de Uma Venda](#19-fluxo-completo-de-uma-venda)
20. [Gerenciamento de Memória](#20-gerenciamento-de-memória)
21. [Como Compilar e Executar](#21-como-compilar-e-executar)
22. [Conceitos de Estrutura de Dados — Resumo para Defesa](#22-conceitos-de-estrutura-de-dados--resumo-para-defesa)

---

## 1. Visão Geral do Projeto

O sistema é um **gerenciador de mercadinho** com interface gráfica feita em **C puro + Raylib**. Ele integra quatro estruturas de dados lineares e não-lineares clássicas da disciplina de Estrutura de Dados:

| Estrutura | Uso no sistema |
|---|---|
| **Tabela Hash** (encadeamento externo) | Estoque de produtos — busca e atualização O(1) médio |
| **Fila Dinâmica** (FIFO) | Fila de atendimento de clientes |
| **Pilha Dinâmica** (LIFO) | Carrinho de compras com funcionalidade de Undo |
| **Árvore Binária de Busca** (BST) | Histórico de vendas indexado por ID |

Além disso, um **Quick Sort manual** ordena vendas por faturamento no relatório gerencial.

### Princípio arquitetural

O código é dividido em **camadas**:

```
┌─────────────────────────────────────────────┐
│              CAMADA DE TELAS                │
│  tela_menu / tela_cadastro / tela_caixa     │
│  tela_relatorios / tela_auditoria           │
├─────────────────────────────────────────────┤
│           CAMADA DE UI (Raylib)             │
│         ui.c — botões, campos, cards        │
├─────────────────────────────────────────────┤
│        CAMADA DE LÓGICA / DADOS             │
│  hash / fila / pilha / bst / sort / dados   │
├─────────────────────────────────────────────┤
│          TIPOS E ESTADO GLOBAL              │
│           types.h / estado.h               │
└─────────────────────────────────────────────┘
```

As camadas inferiores **nunca incluem Raylib**. As camadas superiores nunca implementam lógica de estrutura de dados. Essa separação é um requisito do enunciado e garante clareza no código.

---

## 2. Estrutura de Arquivos

```
m3-mercado/
├── src/
│   ├── types.h          ← Todos os structs e enum de telas
│   ├── hash.h / hash.c  ← Tabela Hash
│   ├── fila.h / fila.c  ← Fila Dinâmica
│   ├── pilha.h / pilha.c← Pilha Dinâmica
│   ├── bst.h / bst.c    ← Árvore Binária de Busca
│   ├── sort.h / sort.c  ← Quick Sort decrescente
│   ├── dados.h / dados.c← Leitura/geração dos arquivos .txt
│   ├── estado.h         ← struct EstadoApp (estado global)
│   ├── ui.h / ui.c      ← Componentes visuais reutilizáveis
│   ├── tela_menu.h/c    ← Tela do menu principal
│   ├── tela_cadastro.h/c← Tela de cadastro de produtos
│   ├── tela_caixa.h/c   ← Tela de frente de caixa
│   ├── tela_relatorios.h/c ← Tela de relatórios
│   ├── tela_auditoria.h/c  ← Tela de auditoria/consulta
│   └── main.c           ← Ponto de entrada + loop principal
├── data/
│   ├── produtos.txt     ← 1000 produtos (gerado automaticamente)
│   └── clientes.txt     ← 1000 clientes (gerado automaticamente)
├── Makefile
├── build.bat            ← Script de compilação para Windows
└── instalar_e_compilar.bat
```

---

## 3. Diagrama de Dependências

```
main.c
 ├── types.h
 ├── estado.h
 │    ├── stdbool.h
 │    ├── types.h
 │    ├── hash.h ──→ types.h
 │    ├── fila.h ──→ types.h
 │    ├── pilha.h──→ types.h
 │    └── bst.h ──→ types.h
 ├── dados.h ──→ types.h, hash.h, fila.h
 ├── ui.h ────→ raylib.h, stdbool.h
 ├── tela_menu.h ──→ estado.h
 ├── tela_cadastro.h ──→ estado.h, ui.h
 ├── tela_caixa.h ──→ estado.h, ui.h
 ├── tela_relatorios.h ──→ estado.h, ui.h
 └── tela_auditoria.h ──→ estado.h, ui.h
```

**Regra importante:** `hash.c`, `fila.c`, `pilha.c`, `bst.c` e `sort.c` **não incluem `raylib.h`** — eles são pura lógica de estrutura de dados.

---

## 4. `types.h` — Tipos Fundamentais

Este arquivo é o **coração do projeto**. Define todos os structs e o enum de navegação. Todo arquivo `.c` chega a ele por meio de algum include.

```c
#ifndef TYPES_H        // Guard de inclusão: evita que o arquivo seja processado
#define TYPES_H        // duas vezes no mesmo arquivo .c (problema de redefinição)
```

### Constantes globais

```c
#define MAX_NOME      100   // Tamanho máximo de qualquer string de nome no sistema.
                            // Usado em: Produto.nome, Cliente.nome, ItemVenda.nome,
                            // Venda.cliente. 100 chars é suficiente para qualquer nome.

#define MAX_TEXTO     256   // Usado para buffers temporários de leitura de arquivo
                            // (char linha[MAX_TEXTO] em dados.c)

#define HASH_TAMANHO  1009  // Tamanho da tabela hash. DEVE ser número primo.
                            // Por que primo? Minimiza colisões na função codigo % tamanho,
                            // pois distribui melhor os índices quando 'codigo' tem
                            // múltiplos comuns com o tamanho.
                            // 1009 é o menor primo > 1000 (necessário para comportar
                            // os 1000 produtos do arquivo com fator de carga < 1).
```

### struct Produto

```c
typedef struct {
    int   codigo;       // Chave primária. Ex: 1000, 1001, ..., 1999.
                        // É o número digitado no campo "Código" da tela de caixa.
                        // Também é a chave usada na função hash: codigo % 1009.

    char  nome[MAX_NOME]; // Nome do produto. Ex: "Arroz Tipo 1". Armazenado como
                          // array de chars (string estilo C). Tamanho fixo no struct,
                          // mas o conteúdo é variável (terminado por '\0').

    float preco;        // Preço unitário em reais. Ex: 15.90f.
                        // Usado para calcular o total do carrinho.
                        // Note: float tem ~7 dígitos de precisão — suficiente para
                        // preços de mercadinho, mas pode acumular erro em somas longas.

    int   quantidade;   // Quantidade em estoque. Decrementada no "bipar" e
                        // incrementada no "desfazer" (undo). Nunca vai abaixo de 0
                        // (verificado em hash_atualizar_quantidade).

    int   dia, mes, ano; // Data de cadastro/registro do produto.
                         // Ex: dia=17, mes=6, ano=2026.
                         // Exigida pelo enunciado para fins de auditoria.
} Produto;
```

### struct Cliente

```c
typedef struct {
    char nome[MAX_NOME]; // Nome completo do cliente. Ex: "Ana Silva".
                         // Lido do arquivo clientes.txt.

    int  dia, mes, ano;  // Data de registro do cliente no sistema.
                         // Exibida na tela de caixa quando o cliente é chamado.
} Cliente;
```

### struct ItemVenda (lista encadeada)

```c
typedef struct ItemVenda {    // Note: struct nomeada (não anônima) — necessário para
                              // referenciar a si mesma com 'struct ItemVenda *prox'

    int   codigo;             // Código do produto vendido. Guardado para referência.

    char  nome[MAX_NOME];     // Nome do produto NO MOMENTO da venda.
                              // Importante: copiamos o nome em vez de guardar um
                              // ponteiro para o Produto, pois o estoque pode mudar
                              // depois. A venda é um registro histórico imutável.

    float preco;              // Preço no momento da venda (também pode mudar depois).

    struct ItemVenda *prox;   // Ponteiro para o próximo item da venda.
                              // NULL no último item. Forma uma lista encadeada simples.
} ItemVenda;
```

**Por que lista encadeada para itens?** Porque cada venda pode ter quantidade variável de itens. Com lista encadeada, alocamos exatamente o que precisamos via `malloc`, sem desperdício.

### struct Venda

```c
typedef struct {
    int        id_venda;      // Identificador único da venda. Gerado sequencialmente
                              // (1, 2, 3...) em tela_caixa.c usando historico->proximo_id++.
                              // É a chave da BST — cada venda ocupa um nó único.

    char       cliente[MAX_NOME]; // Nome do cliente que fez a compra.
                                  // Copiado de app->cliente_atual.nome no checkout.

    float      valor_total;   // Soma dos preços de todos os itens.
                              // Copiado de app->carrinho->total no checkout.

    int        dia, mes, ano; // Data da venda (obtida com time() + localtime()).

    ItemVenda *itens;         // Ponteiro para o início da lista encadeada de itens.
                              // NULL se venda com zero itens (não deveria ocorrer).
                              // A lista é criada em tela_caixa.c percorrendo a pilha.

    int        num_itens;     // Quantidade de itens na lista acima.
                              // Copiado de app->carrinho->tamanho no checkout.
} Venda;
```

### enum Tela

```c
typedef enum {
    TELA_MENU = 0,    // Valor numérico 0. Tela inicial com dashboard e navegação.
    TELA_CADASTRO,    // Valor 1. Formulário de cadastro + consulta rápida.
    TELA_CAIXA,       // Valor 2. Atendimento, bipe de produtos, undo, checkout.
    TELA_RELATORIOS,  // Valor 3. In-order e ranking por faturamento.
    TELA_AUDITORIA    // Valor 4. Busca binária de venda por ID.
} Tela;
// O campo app->tela_atual armazena um desses valores.
// O switch em main.c decide qual função de desenho chamar a cada frame.
```

---

## 5. `hash.h` / `hash.c` — Tabela Hash com Encadeamento Externo

### Conceito

Uma **Tabela Hash** é uma estrutura que mapeia uma chave (o código do produto) a um índice de array usando uma **função hash**. O objetivo é atingir busca e inserção em tempo **O(1) médio**, contra O(n) de uma lista ou O(log n) de uma BST.

**Colisão:** quando dois códigos diferentes produzem o mesmo índice. Aqui usamos **encadeamento externo**: cada posição do array é uma lista encadeada de produtos. Todos os produtos com o mesmo hash coexistem na mesma lista.

```
Índice  Lista encadeada
  0   → [Produto cod=1009] → [Produto cod=2018] → NULL
  1   → [Produto cod=1010] → NULL
  ...
 999  → [Produto cod=1999] → NULL
 1000 → [Produto cod=1000] → NULL
```

### `hash.h` — Declarações

```c
typedef struct HashNode {
    Produto          produto;  // O produto em si, armazenado POR VALOR (cópia).
                               // Quando inserimos, copiamos todos os campos do
                               // Produto para dentro do nó. Isso evita ponteiros
                               // para dados externos que podem mudar ou ser liberados.

    struct HashNode *prox;     // Ponteiro para o próximo nó na mesma posição (colisão).
                               // NULL se este é o último nó da cadeia.
} HashNode;

typedef struct {
    HashNode **tabela;  // Array de PONTEIROS para HashNode.
                        // Cada posição é o início de uma lista encadeada.
                        // tabela[0] ... tabela[1008] (1009 posições).
                        // Alocado com calloc (todos os ponteiros iniciam como NULL).

    int        tamanho; // Tamanho do array = 1009 (passado em hash_criar).

    int        total;   // Contador de produtos únicos inseridos.
                        // Exibido no dashboard do menu como "Produtos no Estoque".
} TabelaHash;
```

### `hash_criar` — Criação da tabela

```c
TabelaHash *hash_criar(int tamanho) {
    TabelaHash *h = (TabelaHash *)malloc(sizeof(TabelaHash));
    // malloc aloca sizeof(TabelaHash) bytes no heap e retorna o endereço.
    // O cast (TabelaHash *) é necessário em C++ mas opcional em C99+.
    // Em C, malloc retorna void* que converte automaticamente.

    h->tamanho = tamanho;  // Guarda o tamanho para usar nas operações.
    h->total   = 0;        // Nenhum produto inserido ainda.

    h->tabela = (HashNode **)calloc(tamanho, sizeof(HashNode *));
    // calloc(n, size) = malloc(n * size) + zera todos os bytes.
    // Alocar 1009 ponteiros para HashNode.
    // O importante é que calloc zera tudo: cada tabela[i] começa como NULL.
    // Se usássemos malloc, os ponteiros teriam valores lixo e precisaríamos
    // de um loop 'for(i=0; i<tamanho; i++) h->tabela[i] = NULL'.

    return h;  // Retorna o ponteiro. Quem chama é responsável por
               // chamar hash_destruir quando não precisar mais.
}
```

### `hash_destruir` — Liberação de memória

```c
void hash_destruir(TabelaHash *h) {
    for (int i = 0; i < h->tamanho; i++) {
        // Percorre todas as 1009 posições do array.

        HashNode *node = h->tabela[i];
        // Pega o primeiro nó da lista na posição i.
        // Pode ser NULL (posição vazia).

        while (node) {
            // Enquanto houver nós na lista encadeada desta posição:

            HashNode *prox = node->prox;
            // Guarda o ponteiro para o próximo ANTES de liberar o atual.
            // Se fizéssemos free(node) antes disso, perderíamos o 'prox'.

            free(node);
            // Libera o nó atual. Depois disso, 'node' é um ponteiro inválido
            // (dangling pointer) — por isso não o usamos mais.

            node = prox;
            // Avança para o próximo nó (que salvamos antes do free).
        }
        // Ao sair do while, a lista na posição i está completamente liberada.
    }

    free(h->tabela);  // Libera o array de ponteiros (os 1009 ponteiros em si).
    free(h);          // Libera a struct TabelaHash.
    // Ordem importa: free(h->tabela) antes de free(h) porque depois de
    // free(h) o campo h->tabela não existe mais.
}
```

### `hash_funcao` — Cálculo do índice

```c
int hash_funcao(TabelaHash *h, int codigo) {
    int idx = codigo % h->tamanho;
    // Operação módulo: resto da divisão de 'codigo' por 1009.
    // Exemplos:
    //   1000 % 1009 = 1000  (índice 1000)
    //   1009 % 1009 = 0     (índice 0)
    //   1999 % 1009 = 990   (índice 990)
    // Isso mapeia qualquer código inteiro para [0, 1008].

    return (idx < 0) ? idx + h->tamanho : idx;
    // Guarda: em C, o operador % pode retornar negativo se 'codigo' for negativo.
    // Ex: -1 % 1009 = -1 em C (não 1008 como em Python).
    // Se idx < 0, somamos h->tamanho para obter o equivalente positivo.
    // Ex: idx = -1 → retorna -1 + 1009 = 1008 (válido).
    // Isso só ocorre se alguém digitar um código negativo na interface.
}
```

### `hash_inserir` — Inserção (com upsert)

```c
int hash_inserir(TabelaHash *h, Produto p) {
    int       idx  = hash_funcao(h, p.codigo);
    // Calcula o índice onde este produto deve estar.

    HashNode *node = h->tabela[idx];
    // Pega o primeiro nó da lista encadeada nesse índice.

    /* verifica se já existe — se sim, atualiza */
    while (node) {
        if (node->produto.codigo == p.codigo) {
            // Encontrou um produto com mesmo código: atualiza (upsert).
            // Isso permite recadastrar um produto alterando preço/quantidade.
            node->produto = p;  // Cópia direta do struct Produto (todos os campos).
            return 0;           // Retorna 0 = "atualizado", não inserido novo.
        }
        node = node->prox;  // Avança para o próximo da lista (possível colisão).
    }
    // Se chegou aqui, o código não existe na tabela → inserir novo nó.

    /* insere na cabeça da lista */
    HashNode *novo = (HashNode *)malloc(sizeof(HashNode));
    // Aloca um novo nó no heap.

    novo->produto  = p;              // Copia o produto para dentro do nó.
    novo->prox     = h->tabela[idx]; // O novo nó aponta para quem era o primeiro.
    h->tabela[idx] = novo;           // O array agora aponta para o novo nó.
    // Isso é "head insertion": O(1) independente do tamanho da lista.
    // Visualmente: [novo] → [antigo_primeiro] → ... → NULL

    h->total++;    // Incrementa o contador de produtos únicos.
    return 1;      // Retorna 1 = "inserido novo produto".
}
```

**Por que inserir na cabeça?** A alternativa seria percorrer até o fim (O(n) por inserção). Inserir na cabeça é sempre O(1), e para buscas a ordem da lista não importa.

### `hash_buscar` — Busca por código

```c
Produto *hash_buscar(TabelaHash *h, int codigo) {
    int       idx  = hash_funcao(h, codigo);
    // Vai direto ao índice onde o produto DEVE estar se existir.

    HashNode *node = h->tabela[idx];
    while (node) {
        if (node->produto.codigo == codigo)
            return &node->produto;
        // Retorna PONTEIRO para o produto dentro do nó.
        // Isso permite modificar o produto diretamente (ex: decrementar estoque).
        // O ponteiro é válido enquanto o nó existir na tabela.

        node = node->prox;  // Avança na lista de colisões.
    }
    return NULL;  // Produto não encontrado.
}
```

**Por que retornar ponteiro e não cópia?** `hash_atualizar_quantidade` precisa modificar o produto in-place. Se retornasse uma cópia, a modificação não seria refletida na tabela.

### `hash_atualizar_quantidade` — Incremento/decremento de estoque

```c
int hash_atualizar_quantidade(TabelaHash *h, int codigo, int delta) {
    Produto *p = hash_buscar(h, codigo);
    // Busca o produto. Retorna NULL se não existir.

    if (!p) return -1;  // Código de erro: produto não encontrado.
    // Retornar -1 permite que tela_caixa.c exiba mensagem específica.

    if (p->quantidade + delta < 0) return -2;
    // Verifica ANTES de modificar se a operação resultaria em estoque negativo.
    // delta = -1 no bipar, +1 no undo.
    // Ex: quantidade=0, delta=-1 → 0 + (-1) = -1 < 0 → retorna -2 (sem estoque).

    p->quantidade += delta;
    // Modifica o produto DENTRO da tabela hash (p é ponteiro para o nó).
    // delta = -1: decrementa (bipar produto).
    // delta = +1: incrementa (undo ou cancelar venda).

    return p->quantidade;  // Retorna a nova quantidade (>= 0).
}
```

---

## 6. `fila.h` / `fila.c` — Fila Dinâmica

### Conceito

Uma **Fila** é uma estrutura FIFO (First In, First Out): o primeiro a entrar é o primeiro a sair. No sistema representa a **fila de atendimento**: o primeiro cliente que chegou (primeiro na fila) é o primeiro a ser atendido.

```
FRENTE                              TRÁS
  ↓                                  ↓
[João] → [Maria] → [Pedro] → [Ana] → NULL
  ↑
 próximo a ser atendido
```

### Estruturas

```c
typedef struct FilaNode {
    Cliente          cliente;   // O cliente armazenado POR VALOR (cópia).
    struct FilaNode *prox;      // Ponteiro para o próximo na fila (NULL no último).
} FilaNode;

typedef struct {
    FilaNode *frente;  // Ponteiro para o INÍCIO da fila (próximo a sair).
    FilaNode *tras;    // Ponteiro para o FIM da fila (último inserido).
                       // Por que manter 'tras'? Para enfileirar em O(1).
                       // Sem 'tras', enfileirar exigiria percorrer toda a fila.
    int       tamanho; // Quantidade de clientes na fila. Exibido na tela de caixa.
} Fila;
```

### `fila_criar`

```c
Fila *fila_criar(void) {
    Fila *f    = (Fila *)malloc(sizeof(Fila));
    f->frente  = NULL;  // Fila vazia: nenhum nó.
    f->tras    = NULL;  // Fila vazia: tanto frente quanto trás são NULL.
    f->tamanho = 0;
    return f;
}
```

### `fila_destruir`

```c
void fila_destruir(Fila *f) {
    Cliente tmp;                               // Variável temporária para receber o cliente.
    while (!fila_vazia(f))                     // Enquanto houver clientes:
        fila_desenfileirar(f, &tmp);           // Remove e libera cada nó.
    free(f);                                   // Libera a struct Fila em si.
}
```

### `fila_enfileirar` — Adicionar ao fim

```c
void fila_enfileirar(Fila *f, Cliente c) {
    FilaNode *novo = (FilaNode *)malloc(sizeof(FilaNode));
    novo->cliente  = c;      // Copia o cliente para dentro do nó.
    novo->prox     = NULL;   // O novo nó vai para o fim: não tem próximo.

    if (f->tras)             // Se já existe algum nó na fila:
        f->tras->prox = novo; // O nó que era o último agora aponta para o novo.
    else                     // Se a fila estava vazia:
        f->frente = novo;    // O novo é tanto o frente quanto o trás.

    f->tras = novo;          // Atualiza 'tras' para o novo nó inserido.
    f->tamanho++;
}
// Custo: O(1) — graças ao ponteiro 'tras', não precisamos percorrer a lista.
```

**Visualização:**
```
Antes (fila com João e Maria):
  frente → [João] → [Maria] → NULL ← tras

Enfileira Pedro:
  novo = [Pedro] → NULL
  f->tras->prox = novo   ← Maria agora aponta para Pedro
  f->tras = novo         ← tras aponta para Pedro

Depois:
  frente → [João] → [Maria] → [Pedro] → NULL ← tras
```

### `fila_desenfileirar` — Remover do início

```c
int fila_desenfileirar(Fila *f, Cliente *out) {
    if (!f->frente) return 0;  // Fila vazia: não há nada para remover.

    FilaNode *node = f->frente;   // Guarda referência ao nó a remover.
    *out           = node->cliente; // Copia o cliente para o parâmetro de saída.
    f->frente      = node->prox;  // O novo frente é o segundo elemento.

    if (!f->frente)               // Se a fila ficou vazia:
        f->tras = NULL;           // 'tras' também deve ser NULL.
                                  // Sem isso, 'tras' continuaria apontando para
                                  // o nó liberado abaixo → ponteiro dangling!

    free(node);   // Libera o nó removido.
    f->tamanho--;
    return 1;     // Sucesso.
}
```

---

## 7. `pilha.h` / `pilha.c` — Pilha Dinâmica

### Conceito

Uma **Pilha** é uma estrutura LIFO (Last In, First Out): o último a entrar é o primeiro a sair. No sistema representa o **carrinho de compras**:

- **PUSH (empilhar):** bipar um produto na caixa.
- **POP (desempilhar):** botão "Desfazer Último Item" (Undo).
- O topo da pilha é sempre o último produto bipado.

```
       TOPO
        ↓
   [Produto C]  ← último bipado, primeiro a ser "desfeito"
   [Produto B]
   [Produto A]  ← primeiro bipado
      NULL
```

### Estruturas

```c
typedef struct PilhaNode {
    Produto           produto;  // O produto bipado, armazenado por valor.
    struct PilhaNode *prox;     // Ponteiro para o nó abaixo (NULL na base).
} PilhaNode;

typedef struct {
    PilhaNode *topo;    // Ponteiro para o elemento do topo.
    int        tamanho; // Quantidade de itens no carrinho.
    float      total;   // Soma dos preços — atualizada a cada push/pop.
                        // Evita recalcular o total percorrendo a pilha inteira
                        // toda vez que precisamos exibir o valor na tela.
} Pilha;
```

### `pilha_empilhar` (push)

```c
void pilha_empilhar(Pilha *p, Produto prod) {
    PilhaNode *novo = (PilhaNode *)malloc(sizeof(PilhaNode));
    novo->produto   = prod;    // Cópia do produto.
    novo->prox      = p->topo; // O novo nó aponta para o antigo topo.
    p->topo         = novo;    // O topo agora é o novo nó.
    p->tamanho++;
    p->total += prod.preco;    // Acumula o preço no total em tempo O(1).
}
```

**Visualização:**
```
Antes: topo → [B] → [A] → NULL

Empilha C:
  novo → [C] → (aponta para) [B] → [A] → NULL
  topo = novo

Depois: topo → [C] → [B] → [A] → NULL
```

### `pilha_desempilhar` (pop)

```c
int pilha_desempilhar(Pilha *p, Produto *out) {
    if (!p->topo) return 0;         // Pilha vazia.

    PilhaNode *node = p->topo;      // Guarda o nó do topo.
    *out            = node->produto; // Copia o produto para o parâmetro de saída.
                                    // 'out' é preenchido ANTES do free abaixo.
    p->topo         = node->prox;   // O novo topo é o elemento anterior.
    free(node);                     // Libera o nó removido.
    p->tamanho--;
    p->total -= out->preco;         // Desconta o preço do total.

    if (p->total < 0.0f)            // Guarda contra imprecisão de ponto flutuante:
        p->total = 0.0f;            // ex: 10.00 - 5.00 - 5.00 pode dar -0.0000001.
    return 1;
}
```

### `pilha_limpar`

```c
void pilha_limpar(Pilha *p) {
    Produto tmp;
    while (!pilha_vazia(p))
        pilha_desempilhar(p, &tmp);
    // Desempilha tudo, liberando cada nó. Após este loop:
    // p->topo = NULL, p->tamanho = 0, p->total = 0.
    // Usado após finalizar uma venda (checkout) para esvaziar o carrinho.
}
```

### Undo (desfazer) na tela de caixa

O botão "DESFAZER ÚLTIMO ITEM" faz:
1. `pilha_desempilhar(app->carrinho, &removido)` — remove o topo e pega o produto.
2. `hash_atualizar_quantidade(app->estoque, removido.codigo, +1)` — devolve ao estoque.

Isso é possível porque a pilha guarda o produto com seu código, permitindo localizar o item na hash e incrementar a quantidade.

---

## 8. `bst.h` / `bst.c` — Árvore Binária de Busca

### Conceito

Uma **Árvore Binária de Busca** (BST) é uma estrutura em que cada nó tem no máximo dois filhos, e a seguinte invariante é mantida:

```
Para qualquer nó N:
  - Todos os nós na subárvore ESQUERDA têm chave < N.chave
  - Todos os nós na subárvore DIREITA têm chave >= N.chave
```

No sistema, a chave é `id_venda`. Cada venda finalizada insere um nó. Isso permite:
- Busca por ID em O(log n) médio (navegando pela invariante).
- Listagem em ordem crescente de ID via **caminhamento em-ordem** (in-order).

```
            Venda #4
           /        \
       Venda #2    Venda #6
       /    \       /    \
  Venda #1  Venda #3  Venda #5  Venda #7
```
In-order desta árvore: 1, 2, 3, 4, 5, 6, 7 ✓

### Estruturas

```c
typedef struct BSTNode {
    Venda          venda;  // A venda armazenada por valor (cópia do struct Venda).
                           // Importante: o campo 'venda.itens' é um ponteiro para
                           // a lista encadeada de itens. Ao copiar Venda, copiamos
                           // o ponteiro — a propriedade da lista passa para este nó.

    struct BSTNode *esq;   // Filho esquerdo (vendas com ID menor).
    struct BSTNode *dir;   // Filho direito (vendas com ID maior).
} BSTNode;

typedef struct {
    BSTNode *raiz;        // Raiz da árvore. NULL se árvore vazia.
    int      proximo_id;  // Próximo ID a ser usado. Começa em 1. Incrementado
                          // em tela_caixa.c no checkout: v.id_venda = historico->proximo_id++
    int      total;       // Contador de vendas. Exibido no dashboard do menu.
} BST;
```

### `bst_inserir` — Inserção iterativa

```c
void bst_inserir(BST *b, Venda v) {
    BSTNode *novo = (BSTNode *)malloc(sizeof(BSTNode));
    novo->venda   = v;     // Copia toda a struct Venda (incluindo o ponteiro 'itens').
    novo->esq     = NULL;
    novo->dir     = NULL;

    if (!b->raiz) {
        b->raiz = novo;    // Árvore vazia: o novo nó vira a raiz.
    } else {
        BSTNode *atual = b->raiz;  // Começa pela raiz.
        while (1) {
            if (v.id_venda < atual->venda.id_venda) {
                // A chave é menor: deve ir para a ESQUERDA.
                if (!atual->esq) {
                    atual->esq = novo;  // Encontrou posição vazia: insere aqui.
                    break;
                }
                atual = atual->esq;     // Continua descendo à esquerda.
            } else {
                // A chave é maior ou igual: deve ir para a DIREITA.
                if (!atual->dir) {
                    atual->dir = novo;
                    break;
                }
                atual = atual->dir;     // Continua descendo à direita.
            }
        }
    }
    b->total++;  // Incrementa o contador de vendas.
}
// Custo: O(h) onde h é a altura da árvore. Se os IDs forem inseridos em ordem
// crescente (o que é o caso aqui), a árvore degenera em uma lista encadeada
// à direita, resultando em O(n). Para o tamanho do trabalho (dezenas de vendas),
// isso é irrelevante na prática.
```

### `bst_buscar` — Busca binária

```c
BSTNode *bst_buscar(BST *b, int id) {
    BSTNode *atual = b->raiz;
    while (atual) {
        if (id == atual->venda.id_venda)
            return atual;           // Encontrou: retorna o nó.

        // Decide para qual lado descer baseado na invariante BST:
        atual = (id < atual->venda.id_venda) ? atual->esq : atual->dir;
        // Se id < chave atual: vai à esquerda (onde estão os menores).
        // Se id > chave atual: vai à direita (onde estão os maiores).
    }
    return NULL;  // Chegou em NULL sem encontrar: ID não existe.
}
// Cada iteração descarta metade da árvore. Custo: O(log n) médio.
```

### `bst_inorder` — Caminhamento em-ordem

```c
void bst_inorder(BSTNode *raiz, Venda *vetor, int *idx) {
    if (!raiz) return;          // Caso base: subárvore vazia, nada a fazer.

    bst_inorder(raiz->esq, vetor, idx);  // 1) Visita toda a subárvore ESQUERDA.
    vetor[(*idx)++] = raiz->venda;       // 2) Visita este nó (copia a venda no vetor).
    bst_inorder(raiz->dir, vetor, idx);  // 3) Visita toda a subárvore DIREITA.
}
// O padrão Esquerda → Nó → Direita garante que os nós são visitados em ordem
// crescente de id_venda (pela invariante da BST).
// 'idx' é passado por ponteiro para que a recursão incremente o mesmo contador.
```

### `bst_extrair_vetor` — Cópia para array dinâmico

```c
Venda *bst_extrair_vetor(BST *b, int *count) {
    *count = b->total;
    if (*count == 0) return NULL;  // Árvore vazia: nada a extrair.

    Venda *v = (Venda *)malloc(sizeof(Venda) * (*count));
    // Aloca um array de Venda com exatamente 'count' posições.
    // Chamador é RESPONSÁVEL por chamar free() neste ponteiro depois.
    // Em tela_relatorios.c: tela_relatorios_limpar() chama free(st->vetor).

    int idx = 0;
    bst_inorder(b->raiz, v, &idx);
    // Preenche o array em ordem crescente de id_venda.
    // idx começa em 0 e vai até *count-1 ao final da recursão.

    return v;
}
```

**Importante:** O array retornado contém **cópias superficiais** das vendas. O campo `itens` (ponteiro para ItemVenda) aponta para as mesmas listas que estão nos nós da BST. Por isso, `free(vetor)` libera apenas o array, nunca as listas. As listas são liberadas por `bst_destruir`.

### `destruir_no` — Liberação recursiva

```c
static void destruir_no(BSTNode *node) {
    // 'static': visibilidade apenas dentro de bst.c.
    if (!node) return;

    destruir_no(node->esq);   // Libera toda a subárvore esquerda primeiro.
    destruir_no(node->dir);   // Libera toda a subárvore direita.

    // Agora libera a lista encadeada de itens desta venda:
    ItemVenda *item = node->venda.itens;
    while (item) {
        ItemVenda *prox = item->prox;  // Salva próximo antes do free.
        free(item);
        item = prox;
    }

    free(node);  // Por último, libera o próprio nó da BST.
}
// Pós-ordem (post-order): libera filhos antes do pai. Isso é obrigatório
// porque liberar o pai primeiro tornaria os filhos inacessíveis (memory leak).
```

---

## 9. `sort.h` / `sort.c` — Quick Sort

### Conceito

O **Quick Sort** é um algoritmo de ordenação divide-e-conquista com custo médio O(n log n). Funciona escolhendo um **pivô** e particionando o array em dois grupos: os que ficam antes do pivô e os que ficam depois. Então ordena cada parte recursivamente.

Aqui ordenamos vendas em **ordem decrescente de valor_total** para o relatório de faturamento.

### Implementação de Lomuto

```c
static void trocar(Venda *a, Venda *b) {
    Venda tmp = *a;  // Cópia temporária da venda 'a'.
    *a = *b;         // 'a' recebe os dados de 'b'.
    *b = tmp;        // 'b' recebe os dados originais de 'a'.
    // Swap completo de dois structs Venda por valor.
}

static int particionar(Venda *v, int low, int high) {
    float pivot = v[high].valor_total;
    // Escolhe o ÚLTIMO elemento como pivô (esquema de Lomuto).
    // Isso é simples mas O(n²) no pior caso (array já ordenado).

    int i = low - 1;
    // 'i' é o índice do último elemento "maior que o pivô" (para desc.).
    // Começa em low-1 (antes do início da parte a ser particionada).

    for (int j = low; j < high; j++) {
        if (v[j].valor_total >= pivot) {
            // Condição: >= pivot para ordenação DECRESCENTE.
            // (Para ascendente seria <= pivot.)
            // Elementos maiores ou iguais ao pivô ficam na metade esquerda.
            i++;
            trocar(&v[i], &v[j]);
            // Move o elemento para a "zona dos maiores".
        }
    }

    trocar(&v[i + 1], &v[high]);
    // Coloca o pivô na posição correta (entre os maiores e os menores).
    return i + 1;
    // Retorna o índice final do pivô.
}

void quicksort_vendas_desc(Venda *v, int low, int high) {
    if (low < high) {
        // Caso base: subarray com 0 ou 1 elemento já está "ordenado".

        int pi = particionar(v, low, high);
        // 'pi' é o índice onde o pivô ficou após a partição.
        // Tudo em v[low..pi-1] é >= v[pi].
        // Tudo em v[pi+1..high] é <= v[pi].

        quicksort_vendas_desc(v, low, pi - 1);   // Ordena metade esquerda.
        quicksort_vendas_desc(v, pi + 1, high);  // Ordena metade direita.
    }
}
```

**Como é chamado em `tela_relatorios.c`:**
```c
// 1. Extrai as vendas da BST para um array alocado com malloc:
st->vetor = bst_extrair_vetor(app->historico, &st->count);
// st->vetor: array com 'count' vendas em ordem crescente de ID.

// 2. Aplica o quicksort para reordenar por faturamento decrescente:
if (st->vetor && st->count > 1)
    quicksort_vendas_desc(st->vetor, 0, st->count - 1);
// Após isso: st->vetor[0] é a venda de maior valor,
//            st->vetor[count-1] é a venda de menor valor.

// 3. Ao sair da tela, libera o array:
free(st->vetor);
st->vetor = NULL;
```

---

## 10. `dados.h` / `dados.c` — Geração e Leitura de Arquivos

### Gerador de pseudoaleatoriedade

```c
static unsigned int _seed = 42;
// Variável de estado do gerador. 'static' = visível apenas em dados.c.
// Semente fixa (42 e 123) garante que os arquivos gerados são SEMPRE os mesmos.
// Isso é importante para reprodutibilidade: se o professor rodar de novo,
// vê os mesmos produtos/clientes.

static int _rand(void) {
    _seed = _seed * 1664525u + 1013904223u;
    // Gerador Congruente Linear (LCG — Linear Congruential Generator).
    // Fórmula clássica: seed = seed * a + c (mod 2^32, implícito pelo overflow).
    // a = 1664525 e c = 1013904223 são constantes de Knuth/Borland.
    // Muito simples e rápido, suficiente para gerar dados de teste.

    return (int)((_seed >> 1) & 0x7FFFFFFF);
    // _seed >> 1: desloca 1 bit à direita (divide por 2).
    // & 0x7FFFFFFF: zera o bit mais significativo → resultado sempre positivo.
    // Garante retorno no range [0, 2^31 - 1] (int positivo).
}
```

### `gerar_produtos_txt`

```c
void gerar_produtos_txt(const char *caminho) {
    FILE *f = fopen(caminho, "w");  // Abre para escrita (cria ou sobrescreve).
    if (!f) return;                 // Se não conseguiu abrir, abandona silenciosamente.
    _seed = 42;                     // Reinicia a semente para resultados determinísticos.

    for (int i = 0; i < 1000; i++) {
        int  codigo   = 1000 + i;
        // Códigos de 1000 a 1999. Sequencial e único por produto.

        int  base_idx = i % NUM_NOMES_BASE;
        // Cicla pelos 96 nomes base. Produto 0 → "Arroz", 96 → "Arroz" novamente.

        int  comp_idx = (i / NUM_NOMES_BASE) % NUM_COMPLEMENTOS;
        // A cada 96 produtos, troca o complemento. Cria combinações como:
        // "Arroz Tipo 1", "Arroz Tipo 2", "Arroz Premium", etc.

        char nome[MAX_NOME];
        snprintf(nome, MAX_NOME, "%s %s", nomes_base[base_idx], complementos[comp_idx]);
        // snprintf: como sprintf mas com limite de tamanho (evita buffer overflow).

        float preco = 2.50f + (_rand() % 9750) / 100.0f;
        // Preço entre R$2,50 e R$100,00.
        // _rand() % 9750 → [0, 9749] → dividido por 100 → [0.00, 97.49] → + 2.50.

        int   quantidade = 10 + _rand() % 191;
        // Estoque entre 10 e 200 unidades.

        fprintf(f, "%d;%s;%.2f;%d;%02d;%02d;%d\n",
                codigo, nome, preco, quantidade, dia, mes, ano);
        // Formato: "1000;Arroz Tipo 1;15.90;50;17;06;2026\n"
        // Separador ponto-e-vírgula. Lido de volta pelo sscanf em carregar_produtos.
        // %.2f: preço com exatamente 2 casas decimais.
        // %02d: dia/mês com zero à esquerda (ex: "06" não "6").
    }
    fclose(f);
}
```

### `carregar_produtos`

```c
int carregar_produtos(const char *caminho, TabelaHash *hash) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;  // Retorna 0 produtos carregados se não conseguiu abrir.

    int  count = 0;
    char linha[MAX_TEXTO];
    while (fgets(linha, sizeof(linha), f)) {
        // fgets lê uma linha por vez (até '\n' ou EOF).
        // sizeof(linha) = 256: limite de segurança para evitar overflow.

        Produto p;
        if (sscanf(linha, "%d;%99[^;];%f;%d;%d;%d;%d",
                   &p.codigo, p.nome, &p.preco, &p.quantidade,
                   &p.dia, &p.mes, &p.ano) == 7) {
            // sscanf: lê valores formatados de uma string.
            // %d: lê inteiro (codigo, quantidade, dia, mes, ano).
            // %99[^;]: lê até 99 chars que NÃO sejam ponto-e-vírgula (nome).
            //   O '[^;]' é um "scanset negado": aceita qualquer char exceto ';'.
            //   Isso permite nomes com espaços como "Arroz Tipo 1".
            // %f: lê float (preco).
            // == 7: verifica se todos os 7 campos foram lidos com sucesso.
            //   Se a linha estiver malformada, pula silenciosamente.

            hash_inserir(hash, p);  // Insere o produto na tabela hash.
            count++;
        }
    }
    fclose(f);
    return count;  // Retorna quantos produtos foram carregados com sucesso.
}
```

---

## 11. `estado.h` — Estado Global da Aplicação

```c
#include <stdbool.h>  // Define 'bool', 'true', 'false' em C99.
                      // Sem este include, 'bool' seria um tipo desconhecido.

typedef struct {
    /* Ponteiros para as estruturas de dados principais */
    TabelaHash *estoque;       // Hash com os 1000+ produtos.
    Fila       *fila_clientes; // Fila com os 1000 clientes aguardando.
    Pilha      *carrinho;      // Pilha do carrinho do cliente atual.
    BST        *historico;     // BST com as vendas finalizadas.

    /* Navegação */
    Tela tela_atual;           // Qual tela deve ser desenhada no próximo frame.
                               // O switch em main.c lê este campo a cada frame.

    /* Cliente em atendimento */
    bool    tem_cliente;       // true = há um cliente sendo atendido agora.
    Cliente cliente_atual;     // Os dados do cliente (nome, data de registro).
                               // Válido apenas quando tem_cliente == true.

    /* Mensagem de status (banner inferior) */
    char  msg_status[256];     // Texto da mensagem. Ex: "Produto cadastrado!".
    float msg_timer;           // Tempo restante para exibir (em segundos).
                               // Decrementado por GetFrameTime() em ui_status().
                               // Quando chega a 0.0f, a mensagem desaparece.
    bool  msg_erro;            // true = mensagem de erro (fundo vermelho).
                               // false = mensagem de sucesso (fundo verde).

    /* Dimensões da janela */
    int largura;               // Atualizado a cada frame com GetScreenWidth().
    int altura;                // Atualizado a cada frame com GetScreenHeight().
                               // Necessário para suporte a redimensionamento.
} EstadoApp;
```

### `estado_msg` — Exibir mensagem

```c
static inline void estado_msg(EstadoApp *app, const char *msg, bool erro) {
    // 'static inline': função auxiliar definida no header.
    // 'inline' sugere ao compilador substituir chamadas por código direto.

    int i = 0;
    while (msg[i] && i < 255) {
        app->msg_status[i] = msg[i];
        i++;
    }
    // Loop manual de cópia de string (equivalente a strncpy, mas explícito).
    app->msg_status[i] = '\0';  // Termina a string.

    app->msg_timer = 3.0f;   // A mensagem ficará visível por 3 segundos.
    app->msg_erro  = erro;   // Define cor do banner (verde ou vermelho).
}
// Uso: estado_msg(app, "Produto cadastrado!", false);
//      estado_msg(app, "Estoque insuficiente!", true);
```

---

## 12. `ui.h` / `ui.c` — Componentes Visuais Reutilizáveis

### Paleta de cores

```c
#define COR_FUNDO    CLITERAL(Color){ 235, 240, 235, 255 }
// CLITERAL(Color){R, G, B, A}: macro do Raylib para inicializar struct Color.
// 255 no alpha = totalmente opaco. 0 = transparente.
// COR_FUNDO: verde-cinza claro (fundo das telas).

#define COR_PRIMARIA CLITERAL(Color){ 27, 94, 32, 255 }
// Verde escuro (Material Design Green 900). Usado em botões principais e header.

#define COR_PERIGO   CLITERAL(Color){ 183, 28, 28, 255 }
// Vermelho escuro. Botão "Cancelar Venda" e mensagens de erro.

#define COR_AVISO    CLITERAL(Color){ 230, 81, 0, 255 }
// Laranja. Botão "Relatórios" e o header da tela de relatórios.

#define COR_INFO     CLITERAL(Color){ 13, 71, 161, 255 }
// Azul escuro. Botão "Cadastro" e header da tela de cadastro.

#define COR_ROXO     CLITERAL(Color){ 74, 20, 140, 255 }
// Roxo. Botão "Auditoria" e header da tela de auditoria.
```

### `CampoTexto` — Campo de entrada de texto

```c
typedef struct {
    char  text[256];       // O texto digitado. Sempre terminado com '\0'.
    int   len;             // Quantidade de caracteres em 'text' (sem o '\0').
    bool  ativo;           // true = campo selecionado (cursor piscante ativo).
    Rectangle bounds;      // Posição e tamanho na tela: {x, y, width, height}.
    char  placeholder[64]; // Texto exibido quando o campo está vazio e inativo.
                           // Ex: "Ex: 2000", "Codigo do produto".
    bool  so_numeros;      // Se true, aceita apenas dígitos 0-9.
                           // Usado nos campos de código, quantidade, data.
} CampoTexto;
```

### `campo_atualizar` — Processar input do teclado

```c
void campo_atualizar(CampoTexto *c) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = GetMousePosition();
        c->ativo  = CheckCollisionPointRec(m, c->bounds);
        // Ativa se o clique foi dentro do campo, desativa caso contrário.
        // CheckCollisionPointRec: função Raylib que verifica se um ponto
        // está dentro de um retângulo.
    }

    if (!c->ativo) return;  // Se não está ativo, não processa input.

    if (IsKeyPressed(KEY_BACKSPACE) && c->len > 0) {
        c->text[--c->len] = '\0';
        // --c->len: decrementa ANTES de usar (pré-decremento).
        // Remove o último caractere zerando aquela posição.
        return;
    }

    int ch;
    while ((ch = GetCharPressed()) != 0) {
        // GetCharPressed(): retorna o próximo caractere Unicode pressionado.
        // Retorna 0 quando não há mais caracteres na fila.
        // O while processa múltiplos caracteres por frame (colagem de texto).

        if (c->len >= 255) break;         // Não ultrapassa o buffer de 256 chars.
        if (c->so_numeros && (ch < '0' || ch > '9')) continue;
        // Filtra não-dígitos se o campo for numérico.
        // '0' = 48, '9' = 57 em ASCII.

        c->text[c->len++] = (char)ch;     // Adiciona o caractere.
        c->text[c->len]   = '\0';         // Mantém o terminador de string.
    }
}
```

### `campo_desenhar` — Renderizar o campo

```c
void campo_desenhar(CampoTexto *c) {
    Color borda = c->ativo ? COR_BORDA_ATIVO : COR_BORDA;
    // Verde quando ativo, cinza quando inativo.

    Color fundo = c->ativo ? COR_CARD : CLITERAL(Color){ 250, 250, 250, 255 };
    // Branco puro quando ativo, cinza bem claro quando inativo.

    DrawRectangleRec(c->bounds, fundo);
    // Preenche o retângulo do campo com a cor de fundo.

    DrawRectangleLinesEx(c->bounds, c->ativo ? 2.0f : 1.0f, borda);
    // Desenha a borda: 2px de espessura quando ativo (destaque visual), 1px inativo.

    const char *exibir = (c->len == 0 && !c->ativo) ? c->placeholder : c->text;
    // Se vazio e inativo: mostra o placeholder em cinza.
    // Caso contrário: mostra o texto digitado em escuro.

    DrawText(exibir,
             (int)c->bounds.x + 8,                          // 8px de margem esquerda.
             (int)c->bounds.y + (int)(c->bounds.height/2) - 9, // Centralizado verticalmente.
             18, cor_txt);

    /* cursor piscante */
    if (c->ativo && ((int)(GetTime() * 2) % 2 == 0)) {
        // GetTime(): tempo desde o início do programa em segundos (float).
        // * 2: frequência de 2 ciclos por segundo.
        // % 2 == 0: alterna entre 0 e 1 a cada 0.5s → cursor pisca.
        int cx = (int)c->bounds.x + 8 + MeasureText(c->text, 18);
        // MeasureText: largura em pixels do texto no tamanho 18.
        // O cursor fica logo após o último caractere.
        DrawRectangle(cx, cy, 2, 18, COR_TEXTO);
        // Cursor = retângulo de 2px de largura.
    }
}
```

### `ui_botao` — Botão clicável

```c
bool ui_botao(Rectangle r, const char *txt, Color cor, Color cor_hover, Color cor_txt, int fs) {
    Vector2 m     = GetMousePosition();
    bool    hover = CheckCollisionPointRec(m, r);
    // hover = true quando o mouse está sobre o botão.

    bool    click = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    // click = true apenas no frame em que o botão esquerdo do mouse é pressionado
    // E o cursor está sobre o botão. IsMouseButtonPressed é verdadeiro por apenas
    // 1 frame (não repete enquanto segura).

    DrawRectangleRec(r, hover ? cor_hover : cor);
    // Muda a cor ao passar o mouse (feedback visual de hover).

    if (!hover)
        DrawRectangle((int)r.x + 2, (int)r.y + (int)r.height,
                      (int)r.width, 3, CLITERAL(Color){ 0, 0, 0, 30 });
    // Sombra sutil abaixo do botão quando não está em hover.
    // A sombra desaparece no hover, dando efeito de "pressionar".

    int tw = MeasureText(txt, fs);           // Largura do texto em pixels.
    int tx = (int)r.x + ((int)r.width - tw) / 2;  // X para centralizar.
    int ty = (int)r.y + ((int)r.height - fs) / 2; // Y para centralizar verticalmente.
    DrawText(txt, tx, ty, fs, cor_txt);

    return click;  // O chamador usa este bool para executar a ação:
    // if (ui_botao(...)) { /* ação */ }
}
```

### `ui_header` — Barra de cabeçalho

```c
void ui_header(const char *titulo, Color cor) {
    DrawRectangle(0, 0, GetScreenWidth(), 60, cor);
    // Barra de 60px de altura em toda a largura da janela.

    DrawRectangle(0, 57, GetScreenWidth(), 3, CLITERAL(Color){ 0, 0, 0, 60 });
    // Linha escura semitransparente nos últimos 3px do header.
    // Cria um efeito de sombra/separação entre o header e o conteúdo.

    int fs = 22;
    int tw = MeasureText(titulo, fs);
    DrawText(titulo, GetScreenWidth() / 2 - tw / 2, (60 - fs) / 2, fs, WHITE);
    // Texto branco centralizado horizontalmente e verticalmente no header.
}
```

### `ui_status` — Banner de mensagem temporária

```c
void ui_status(const char *msg, bool erro, float *timer) {
    if (!msg || *timer <= 0.0f) return;
    // Só exibe se há mensagem E o timer ainda não zerou.

    *timer -= GetFrameTime();
    // GetFrameTime(): tempo do último frame em segundos (ex: 0.0167 para 60fps).
    // Subtrai do timer a cada frame. Quando chega a 0, a mensagem some.

    Color bg  = erro ? COR_ERRO_BG   : COR_SUCESSO_BG;  // Vermelho ou verde claro.
    Color txt = erro ? COR_PERIGO    : COR_PRIMARIA;     // Texto no tom mais escuro.

    int w = GetScreenWidth();
    DrawRectangle(0, GetScreenHeight() - 44, w, 44, bg);
    // Banner de 44px no rodapé da tela.

    DrawRectangle(0, GetScreenHeight() - 44, 4, 44, txt);
    // Barra vertical de 4px na borda esquerda (acento colorido).

    int tw = MeasureText(msg, 18);
    DrawText(msg, w / 2 - tw / 2, GetScreenHeight() - 30, 18, txt);
    // Texto centralizado no banner.
}
```

---

## 13. `main.c` — Ponto de Entrada e Loop Principal

```c
#define JANELA_W  1280   // Largura inicial da janela em pixels.
#define JANELA_H   720   // Altura inicial (proporção 16:9).
#define FPS        60    // Frames por segundo alvo.
```

### Inicialização

```c
int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    // FLAG_WINDOW_RESIZABLE: permite redimensionar a janela pelo mouse.
    // FLAG_MSAA_4X_HINT: Anti-aliasing 4x (bordas mais suaves). "Hint" = sugestão
    // ao driver gráfico (pode ser ignorado em hardware antigo).
    // Deve ser chamado ANTES de InitWindow.

    InitWindow(JANELA_W, JANELA_H, "Mercadinho - Sistema de Gerenciamento | ED");
    // Cria a janela gráfica com o título especificado.

    SetTargetFPS(FPS);
    // Limita o loop a 60 iterações por segundo. Evita que o programa consuma
    // 100% da CPU em máquinas rápidas.

    EstadoApp app;
    memset(&app, 0, sizeof(EstadoApp));
    // memset com 0: zera todos os bytes da struct.
    // Equivale a inicializar todos os campos com 0/NULL/false.
    // É uma prática segura para evitar valores lixo em campos não inicializados.

    app.estoque       = hash_criar(HASH_TAMANHO);   // Hash com 1009 buckets.
    app.fila_clientes = fila_criar();               // Fila vazia.
    app.carrinho      = pilha_criar();              // Pilha vazia.
    app.historico     = bst_criar();                // BST vazia, proximo_id=1.
```

### Carregamento de dados

```c
    FILE *fp = fopen(arq_produtos, "r");
    if (!fp) {
        gerar_produtos_txt(arq_produtos);   // Gera se não existe.
    } else {
        fclose(fp);  // Fecha imediatamente: só verificamos existência.
    }
    // Mesma lógica para clientes.txt.

    int np = carregar_produtos(arq_produtos, app.estoque);
    int nc = carregar_clientes(arq_clientes, app.fila_clientes);
    // np e nc são usados apenas para log no console. O sistema funciona
    // independentemente do número carregado.
```

### Loop principal (game loop)

```c
    while (!WindowShouldClose()) {
        // WindowShouldClose() retorna true quando:
        // - O usuário clica no X da janela.
        // - O usuário pressiona Alt+F4 (ou Cmd+Q no Mac).
        // - O código chama CloseWindow() explicitamente.

        app.largura = GetScreenWidth();
        app.altura  = GetScreenHeight();
        // Atualiza as dimensões a cada frame para suportar redimensionamento.
        // Se a janela for redimensionada, os cálculos de layout nas telas
        // usam esses valores atualizados.

        BeginDrawing();
        // Inicia o frame de renderização. Tudo entre BeginDrawing() e
        // EndDrawing() é desenhado neste frame.

        switch (app.tela_atual) {
            case TELA_MENU:       tela_menu_desenhar(&app);          break;
            case TELA_CADASTRO:   tela_cadastro_desenhar(&app, ...); break;
            case TELA_CAIXA:      tela_caixa_desenhar(&app, ...);    break;
            case TELA_RELATORIOS: tela_relatorios_desenhar(&app, ...); break;
            case TELA_AUDITORIA:  tela_auditoria_desenhar(&app, ...); break;
        }
        // Cada função de tela:
        // 1. Limpa o fundo (ClearBackground).
        // 2. Processa input do usuário (cliques, teclado).
        // 3. Executa lógica (insere na hash, empilha, etc.).
        // 4. Desenha os componentes visuais (botões, cards, listas).
        // Navegação: quando o usuário clica em um botão de navegação,
        // a função muda app.tela_atual para a nova tela. Na próxima iteração
        // do while, o switch selecionará a nova tela.

        EndDrawing();
        // Apresenta o frame na tela (double buffering: enquanto um frame é
        // exibido, o próximo é desenhado no buffer de trás).
    }
```

### Liberação de memória

```c
    tela_relatorios_limpar(&st_rel);
    // Libera st_rel.vetor (se existir) ANTES de destruir a BST.
    // st_rel.vetor contém cópias das vendas cujos campos 'itens' apontam
    // para listas dentro da BST. Liberar o vetor primeiro é seguro pois
    // free(vetor) libera apenas o array, não as listas.
    // Se liberássemos a BST primeiro, os ponteiros 'itens' no vetor
    // ficariam dangling — apontando para memória já liberada.

    hash_destruir(app.estoque);        // Libera todos os nós da hash + struct.
    fila_destruir(app.fila_clientes);  // Libera todos os nós da fila + struct.
    pilha_destruir(app.carrinho);      // Libera todos os nós da pilha + struct.
    bst_destruir(app.historico);       // Libera todos os nós da BST + listas de
                                       // ItemVenda + struct.

    CloseWindow();  // Fecha a janela e libera recursos do Raylib (OpenGL, etc.).
    return 0;       // Código de saída 0 = sucesso (convenção Unix/Windows).
```

---

## 14. `tela_menu.c` — Menu Principal com Dashboard

### `botao_grande` — Botão especial com título e subtítulo

```c
static bool botao_grande(Rectangle r, const char *titulo, const char *sub,
                          Color cor, Color cor_h) {
    // 'static': função local a tela_menu.c, não exposta em outros arquivos.

    bool hover = CheckCollisionPointRec(GetMousePosition(), r);
    bool click = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawRectangleRec(r, hover ? cor_h : cor);

    /* título centralizado na metade superior */
    int tw = MeasureText(titulo, 24);
    DrawText(titulo,
             (int)r.x + ((int)r.width - tw) / 2,
             (int)r.y + (int)r.height / 2 - 22,  // 22px acima do centro.
             24, WHITE);

    /* subtítulo centralizado na metade inferior */
    int sw = MeasureText(sub, 14);
    DrawText(sub,
             (int)r.x + ((int)r.width - sw) / 2,
             (int)r.y + (int)r.height / 2 + 10,  // 10px abaixo do centro.
             14, CLITERAL(Color){ 255, 255, 255, 180 });  // Branco semi-transparente.
    // Alpha 180/255 ≈ 70% de opacidade → texto secundário visível mas sutil.

    return click;
}
```

### Layout dos cards de dashboard

```c
int card_w = (W - 2 * margin - 2 * gap) / 3;
// W = largura total da janela (1280 por padrão).
// margin = 20px de cada lado.
// gap = 14px entre os cards.
// Espaço disponível: 1280 - 40 - 28 = 1212px.
// Dividido em 3: ~404px por card.
// Cada card exibe um contador em tamanho 36px (número grande no centro).
```

### Leitura dos contadores em tempo real

```c
snprintf(buf, 64, "%d", app->fila_clientes->tamanho);
// app->fila_clientes->tamanho: campo da struct Fila, atualizado
// automaticamente pelos enfileirar/desenfileirar. Lido diretamente.

snprintf(buf, 64, "%d", app->estoque->total);
// app->estoque->total: campo da struct TabelaHash, incrementado em hash_inserir.

snprintf(buf, 64, "%d", app->historico->total);
// app->historico->total: campo da struct BST, incrementado em bst_inserir.
```

---

## 15. `tela_cadastro.c` — Cadastro de Produtos

### Campos do formulário

```c
#define LABEL_X  40    // X dos rótulos ("Codigo:", "Nome:", etc.)
#define FIELD_X  220   // X dos campos de entrada (alinhados).
#define FIELD_W  320   // Largura padrão dos campos.
#define FIELD_H  36    // Altura dos campos.
#define ROW_GAP  52    // Espaçamento vertical entre linhas do formulário.
```

### `tela_cadastro_init`

```c
void tela_cadastro_init(TelaCadastroState *st) {
    int y0 = 130;
    // y=130: posição vertical da primeira linha do formulário.
    // O header ocupa y=0..60, e há margem de 70px.

    campo_init(&st->f_codigo, (Rectangle){ FIELD_X, y0 + 0*ROW_GAP, FIELD_W, FIELD_H },
               "Ex: 2000", true);
    // Linha 0: Código (somente números).

    campo_init(&st->f_nome,   (Rectangle){ FIELD_X, y0 + 1*ROW_GAP, FIELD_W + 200, FIELD_H },
               "Ex: Arroz Tipo 1 5kg", false);
    // Linha 1: Nome (mais largo: FIELD_W + 200 = 520px, aceita letras e espaços).

    // [...demais campos...]

    /* pré-preenche data de hoje */
    time_t t  = time(NULL);
    // time(NULL): segundos desde 1970-01-01 00:00:00 UTC (Unix epoch).

    struct tm *tm = localtime(&t);
    // localtime: converte para struct tm com campos tm_mday, tm_mon, tm_year.
    // tm_mon: 0-11 (janeiro=0), por isso somamos +1.
    // tm_year: anos desde 1900, por isso somamos +1900.

    snprintf(st->f_dia.text, 256, "%02d", tm->tm_mday);
    // %02d: formata com zero à esquerda (ex: "07" e não "7").
```

### Botão CADASTRAR — Validação e inserção

```c
if (ui_botao(btn_cad, "CADASTRAR", ...)) {
    if (st->f_codigo.len == 0 || st->f_nome.len == 0 || ...) {
        estado_msg(app, "Preencha todos os campos obrigatorios!", true);
        // Validação de campos obrigatórios. 'len == 0' indica campo vazio.
    } else {
        Produto p;
        p.codigo     = atoi(st->f_codigo.text);
        // atoi: "2000" → 2000. Retorna 0 se a string não é um número válido.

        p.preco      = (float)atof(st->f_preco.text);
        // atof: "15.90" → 15.9. Cast para float (atof retorna double).

        if (p.codigo <= 0 || p.preco <= 0 || p.quantidade < 0) {
            // Validação semântica: não aceita código 0, preço negativo.
            // p.quantidade < 0 (não < 0 não <= 0, pois estoque 0 é válido).
        } else {
            int res = hash_inserir(app->estoque, p);
            if (res == 1) estado_msg(app, "Produto cadastrado com sucesso!", false);
            else          estado_msg(app, "Produto atualizado (codigo ja existia).", false);
            // res == 1: novo produto inserido.
            // res == 0: código já existia, produto foi atualizado (upsert).
        }
    }
}
```

### Consulta rápida com `static`

```c
static CampoTexto f_busca;
static bool f_busca_init = false;
// 'static' em variável local: o valor é mantido entre chamadas da função.
// Sem 'static', f_busca seria recriado (e campo_init chamado) a cada frame.
// Com 'static', inicializamos apenas uma vez (quando f_busca_init == false).
if (!f_busca_init) {
    campo_init(&f_busca, ...);
    f_busca_init = true;
}
```

---

## 16. `tela_caixa.c` — Frente de Caixa e Atendimento

Esta é a tela mais complexa do sistema. Tem **três painéis lado a lado**:

```
┌──────────────┬──────────────────┬─────────────┐
│   ESQUERDO   │     CENTRAL      │   DIREITO   │
│  (420px)     │    (420px)       │  (~410px)   │
│              │                  │             │
│ Cliente em   │ Bipe de produtos │ Resumo da   │
│ atendimento  │ + Lista carrinho │ venda       │
│              │                  │ Undo        │
│ Prox cliente │                  │ Finalizar   │
│              │                  │ Cancelar    │
└──────────────┴──────────────────┴─────────────┘
```

### Fluxo do "Bipar Produto"

```c
/* 1. Reposiciona o campo de entrada (posição relativa ao painel central) */
st->f_cod.bounds.x     = (float)(cx + 10);
st->f_cod.bounds.y     = 148.0f;
st->f_cod.bounds.width = 200.0f;
campo_atualizar(&st->f_cod);
campo_desenhar(&st->f_cod);

/* 2. Detecta acionamento (botão ou Enter) */
bool bipe_clicado = ui_botao(btn_bipe, "BIPAR", ...);
if ((bipe_clicado || (st->f_cod.ativo && IsKeyPressed(KEY_ENTER)))
    && st->f_cod.len > 0) {
    // Aceita tanto clique no botão quanto pressionar Enter no campo.

    if (!app->tem_cliente) {
        estado_msg(app, "Chame um cliente antes de bipar!", true);
    } else {
        int cod = atoi(st->f_cod.text);

        /* 3. Decrementa o estoque na hash ANTES de empilhar */
        int res = hash_atualizar_quantidade(app->estoque, cod, -1);
        // -1 = diminui 1 unidade.

        if (res == -1) {
            // Produto não existe na tabela hash.
        } else if (res == -2) {
            // Produto existe mas estoque já está em 0.
        } else {
            /* 4. Busca o produto atualizado e empilha no carrinho */
            Produto *p = hash_buscar(app->estoque, cod);
            pilha_empilhar(app->carrinho, *p);
            // *p: desreferencia o ponteiro → passa o Produto por VALOR.
            // A pilha guarda uma CÓPIA do produto no estado atual do estoque.
            campo_limpar(&st->f_cod);  // Limpa o campo para o próximo bipe.
        }
    }
}
```

### Renderização do carrinho

```c
PilhaNode *n = app->carrinho->topo;
// Começa do TOPO (último item adicionado) e desce pela lista.

while (n && cont < max_vis) {
    // max_vis = (H - 270) / 26: quantos itens cabem na área visível.
    // Para H=720: (720-270)/26 = 17 itens.

    DrawRectangle(cx + 6, item_y - 2, cw - 12, 24, bg);
    // Linha de fundo alternada (listras zebra): itens pares = branco, ímpares = verde claro.

    snprintf(ibuf, 128, "%d  %s", cont + 1, n->produto.nome);
    // "1  Arroz Tipo 1", "2  Leite Integral", etc.

    char pbuf[24];
    snprintf(pbuf, 24, "R$ %.2f", n->produto.preco);
    int ptw = MeasureText(pbuf, 14);
    DrawText(pbuf, cx + cw - 20 - ptw, item_y, 14, COR_PRIMARIA);
    // Preço alinhado à direita do painel.

    n = n->prox;  // Desce na pilha (do topo para a base).
}
```

### Checkout — Finalizar Venda

```c
v.id_venda   = app->historico->proximo_id++;
// proximo_id começa em 1 (bst_criar). O operador ++ pós-fixo:
// usa o valor ATUAL (1 para a primeira venda), depois incrementa.
// Primeira venda: id_venda = 1, proximo_id passa a ser 2.
// Segunda venda:  id_venda = 2, proximo_id passa a ser 3. Etc.

/* Copia itens da pilha para lista encadeada */
PilhaNode *pn = app->carrinho->topo;
while (pn) {
    ItemVenda *iv = (ItemVenda *)malloc(sizeof(ItemVenda));
    iv->codigo    = pn->produto.codigo;
    strncpy(iv->nome, pn->produto.nome, MAX_NOME - 1);
    iv->nome[MAX_NOME - 1] = '\0';
    iv->preco     = pn->produto.preco;
    iv->prox      = v.itens;  // Head insertion na lista de itens da venda.
    v.itens       = iv;
    pn = pn->prox;
}
// Esta lista (de ItemVenda) passa a ser PROPRIEDADE da struct Venda.
// Quando a Venda é inserida na BST, a BST herda a propriedade dos ItemVenda.
// A BST libera os ItemVenda em bst_destruir → destruir_no.

bst_inserir(app->historico, v);
// Insere a Venda (com toda a lista de itens) na BST.
// bst_inserir faz uma CÓPIA do struct Venda (campos por valor) incluindo o
// ponteiro v.itens. O novo nó da BST e o código acima apontam para os mesmos
// ItemVenda. Mas logo abaixo:

pilha_limpar(app->carrinho);
// Libera os PilhaNode (que guardam Produto), não os ItemVenda.
// Os ItemVenda foram alocados separadamente e passaram para a BST.
// Não há conflito: Pilha e BST apontam para coisas diferentes.

app->tem_cliente = false;
// Encerra o atendimento. A próxima chamada de "Próximo Cliente"
// chama fila_desenfileirar para pegar o próximo da fila.
```

---

## 17. `tela_relatorios.c` — Relatórios Gerenciais

### Dois modos de relatório

```c
typedef enum {
    REL_NENHUM = 0,    // Estado inicial: nenhum relatório selecionado.
    REL_INORDER,       // Por ID (caminhamento em-ordem da BST).
    REL_FATURAMENTO    // Por valor total decrescente (quick sort).
} ModoRelatorio;
```

### Relatório In-Order (por ID)

```c
if (ui_botao(b_id, "RELATORIO POR ID (EM-ORDEM)", ...)) {
    tela_relatorios_limpar(st);
    // Se havia um relatório anterior, free() o vetor primeiro.

    st->modo  = REL_INORDER;
    st->vetor = bst_extrair_vetor(app->historico, &st->count);
    // bst_extrair_vetor faz malloc + inorder traversal.
    // O vetor resultante já está em ordem crescente de id_venda
    // (propriedade do caminhamento em-ordem em uma BST).
}
```

### Relatório por Faturamento (com Quick Sort)

```c
if (ui_botao(b_fat, "RANKING DE FATURAMENTO", ...)) {
    tela_relatorios_limpar(st);
    st->modo  = REL_FATURAMENTO;
    st->vetor = bst_extrair_vetor(app->historico, &st->count);
    // Mesmo vetor do relatório anterior (em ordem de ID)...

    if (st->vetor && st->count > 1)
        quicksort_vendas_desc(st->vetor, 0, st->count - 1);
    // ...mas agora reordenado pelo Quick Sort para faturamento decrescente.
    // Após o sort: st->vetor[0] = maior venda, st->vetor[count-1] = menor.
}
```

### Scroll com Scissor Mode

```c
BeginScissorMode((int)area.x, list_y + 28, (int)area.width, (int)area_h);
// Scissor Mode: define uma "janela de recorte". Tudo desenhado fora desta
// área é CORTADO (não aparece na tela). Permite implementar scroll sem
// que os itens transbordem para fora do card.

for (int i = 0; i < st->count; i++) {
    float y = (float)(list_y + 28) + i * item_h - st->scroll;
    // 'st->scroll' é o offset em pixels (atualizado pela roda do mouse).
    // Quando scroll=0: primeiro item na posição list_y+28.
    // Quando scroll=50: cada item sobe 50px → os primeiros ficam fora da área.

    if (y + item_h < list_y + 28) continue;  // Item acima da área visível: pula.
    if (y > list_y + 28 + area_h)   break;   // Item abaixo da área: para.
    // Essas verificações evitam desenhar itens que o scissor mode cortaria.
    // É uma otimização de performance.
}

EndScissorMode();  // Remove o recorte para o restante da tela.
```

### Liberação da memória ao sair

```c
void tela_relatorios_limpar(TelaRelatoriosState *st) {
    if (st->vetor) {
        free(st->vetor);   // Libera o array de Venda (cópias superficiais).
                           // Os ItemVenda dentro das Vendas NÃO são liberados aqui
                           // pois pertencem à BST.
        st->vetor = NULL;  // Zera o ponteiro para evitar double-free.
    }
    st->count  = 0;
    st->scroll = 0.0f;
    st->modo   = REL_NENHUM;
}
// Chamado: 1) ao clicar VOLTAR, 2) ao trocar de modo de relatório,
//          3) em main.c antes de bst_destruir (ordem obrigatória).
```

---

## 18. `tela_auditoria.c` — Auditoria e Consulta

### Busca binária na BST

```c
bool buscar = ui_botao(btn_busca, "BUSCAR", ...);

if (buscar || (st->f_id.ativo && IsKeyPressed(KEY_ENTER))) {
    if (st->f_id.len > 0) {
        int id        = atoi(st->f_id.text);  // Converte o texto "42" → 42.
        st->resultado = bst_buscar(app->historico, id);
        // bst_buscar navega pela BST usando a invariante:
        //   id < nó.id → vai à esquerda
        //   id > nó.id → vai à direita
        //   id == nó.id → encontrou
        // Retorna o BSTNode* ou NULL se não encontrado.
        st->buscou = true;  // Indica que já houve uma tentativa de busca.
    }
}
```

### Exibição do resultado

```c
if (!st->resultado) {
    // NULL: ID não existe na árvore.
    // Exibe mensagem de erro com o ID buscado.
}

Venda *v = &st->resultado->venda;
// Acessa a Venda dentro do nó BST por referência (ponteiro).
// Não cria cópia: mostramos os dados diretamente do nó.

/* Percorre a lista de itens da venda */
ItemVenda *item = v->itens;
while (item && iy < ry + rh - 60) {
    // Para quando acabam os itens (item == NULL) ou
    // quando o espaço vertical do card acaba.
    DrawText(item->nome, ...);
    item = item->prox;  // Avança na lista encadeada de itens.
}

/* Total no rodapé */
iy = ry + rh - 48;  // Posição fixa no rodapé do card, independente dos itens.
DrawText("TOTAL:", ...);
snprintf(totbuf, 32, "R$ %.2f", v->valor_total);
```

---

## 19. Fluxo Completo de Uma Venda

Aqui está o caminho de dados completo desde "chamar cliente" até "venda registrada na BST":

```
1. CHAMAR CLIENTE
   fila_desenfileirar(app->fila_clientes, &app->cliente_atual)
   └── Remove o primeiro FilaNode da fila
   └── Copia o Cliente para app->cliente_atual
   └── app->tem_cliente = true

2. BIPAR PRODUTO (ex: código 1042)
   hash_atualizar_quantidade(app->estoque, 1042, -1)
   └── hash_funcao(h, 1042) → idx = 1042 % 1009 = 33
   └── Percorre a lista em h->tabela[33] até encontrar codigo == 1042
   └── Decrementa produto.quantidade
   └── Retorna nova quantidade (>= 0) ou -2 (estoque insuficiente)
   
   hash_buscar(app->estoque, 1042) → Produto*
   pilha_empilhar(app->carrinho, *produto)
   └── malloc(PilhaNode)
   └── PilhaNode.produto = cópia do Produto
   └── Insere no topo da pilha
   └── app->carrinho->total += produto.preco

3. DESFAZER (opcional)
   pilha_desempilhar(app->carrinho, &removido)
   └── Remove o PilhaNode do topo
   └── app->carrinho->total -= removido.preco
   
   hash_atualizar_quantidade(app->estoque, removido.codigo, +1)
   └── Devolve 1 unidade ao estoque

4. FINALIZAR VENDA
   Monta struct Venda:
   └── id_venda = app->historico->proximo_id++  (ex: 5)
   └── cliente = app->cliente_atual.nome
   └── valor_total = app->carrinho->total
   └── dia/mes/ano = localtime(time(NULL))
   
   Copia pilha para lista encadeada de ItemVenda:
   └── Para cada PilhaNode: malloc(ItemVenda) + head insertion
   
   bst_inserir(app->historico, v)
   └── malloc(BSTNode)
   └── BSTNode.venda = cópia do struct Venda (incluindo ponteiro itens)
   └── Navega pela BST até encontrar posição correta pelo id_venda
   └── Insere como filho esquerdo ou direito
   └── app->historico->total++
   
   pilha_limpar(app->carrinho)
   └── Libera todos os PilhaNode (mas NÃO os ItemVenda, que pertencem à BST)
   
   app->tem_cliente = false

5. VER RELATÓRIO
   bst_extrair_vetor(app->historico, &count)
   └── malloc(Venda * count)
   └── bst_inorder → preenche array em ordem de id_venda
   └── Retorna ponteiro para o array
   
   quicksort_vendas_desc(vetor, 0, count-1)
   └── Reordena por valor_total decrescente
   
   [exibe na tela]
   
   free(vetor) ao sair
   └── Libera o array (NÃO os ItemVenda dentro das Vendas)

6. BUSCAR POR ID
   bst_buscar(app->historico, 5)
   └── Navega pela BST: 5 > raiz? vai à direita. 5 < nó? vai à esquerda.
   └── Retorna BSTNode* ou NULL
   
   Exibe os dados da venda + lista de ItemVenda diretamente do nó BST
```

---

## 20. Gerenciamento de Memória

### Mapa de propriedades

```
Dono          O que possui               Liberado por
─────────────────────────────────────────────────────────────
hash          HashNode[]                 hash_destruir
              └── Produto (por valor)

fila          FilaNode[]                 fila_destruir
              └── Cliente (por valor)

pilha         PilhaNode[]                pilha_destruir → pilha_limpar
              └── Produto (por valor)

bst           BSTNode[]                  bst_destruir → destruir_no
              └── Venda (por valor)
                  └── ItemVenda[] (lista encadeada)

relatorios    Venda[] (array)            tela_relatorios_limpar → free()
  (vetor)     └── campo itens: APONTA para ItemVenda da BST (não possui)
```

### Ordem correta de destruição (em main.c)

```
1. tela_relatorios_limpar(&st_rel)   ← free(vetor) — os itens ainda existem na BST
2. hash_destruir(app.estoque)         ← libera produtos
3. fila_destruir(app.fila_clientes)   ← libera clientes
4. pilha_destruir(app.carrinho)       ← libera itens do carrinho atual
5. bst_destruir(app.historico)        ← libera vendas + itens vendidos
```

**Por que esta ordem importa?** O vetor de relatórios (`st_rel.vetor`) contém cópias das `Venda`s com o campo `itens` apontando para as mesmas `ItemVenda` que estão dentro dos `BSTNode`. Se liberássemos a BST antes do vetor, os ponteiros `itens` no vetor apontariam para memória liberada (dangling pointers). Ao liberar o vetor primeiro (`free(st_rel.vetor)`) apenas o array de Vendas é liberado, sem tocar nas `ItemVenda` — que a BST depois libera corretamente em `destruir_no`.

### Verificação: não há vazamentos

| Alocação | `free` correspondente | Status |
|---|---|---|
| `hash_criar`: `malloc(TabelaHash)` + `calloc(tabela)` | `hash_destruir`: loop + `free(tabela)` + `free(h)` | ✅ |
| `hash_inserir`: `malloc(HashNode)` | `hash_destruir`: loop de cada bucket | ✅ |
| `fila_criar`: `malloc(Fila)` | `fila_destruir`: `free(f)` | ✅ |
| `fila_enfileirar`: `malloc(FilaNode)` | `fila_destruir` → `fila_desenfileirar` | ✅ |
| `pilha_criar`: `malloc(Pilha)` | `pilha_destruir`: `free(p)` | ✅ |
| `pilha_empilhar`: `malloc(PilhaNode)` | `pilha_desempilhar` / `pilha_limpar` | ✅ |
| `bst_criar`: `malloc(BST)` | `bst_destruir`: `free(b)` | ✅ |
| `bst_inserir`: `malloc(BSTNode)` | `bst_destruir` → `destruir_no` | ✅ |
| Checkout: `malloc(ItemVenda)` por item | `destruir_no`: loop de itens | ✅ |
| `bst_extrair_vetor`: `malloc(Venda * count)` | `tela_relatorios_limpar`: `free(vetor)` | ✅ |

---

## 21. Como Compilar e Executar

### Pré-requisitos (Windows)

**1. Compilador C (w64devkit — MinGW-w64 portátil):**
- Baixe: https://github.com/skeeto/w64devkit/releases
- Extraia para `C:\w64devkit`
- Conteúdo esperado: `C:\w64devkit\bin\gcc.exe`

**2. Biblioteca Raylib (pacote pré-compilado para MinGW):**
- Baixe o arquivo `raylib-X.X.X_win64_mingw-w64.zip` em: https://github.com/raysan5/raylib/releases
- Extraia para `C:\raylib`
- Conteúdo esperado:
  - `C:\raylib\include\raylib.h`
  - `C:\raylib\lib\libraylib.a`

### Compilação

**Opção 1 — Script automático:**
```
Duplo clique em: build.bat
```

**Opção 2 — Linha de comando:**
```bat
C:\w64devkit\bin\gcc.exe -std=c99 -Wall -O2 ^
    -I C:\raylib\include ^
    -o mercadinho.exe ^
    src\main.c src\hash.c src\fila.c src\pilha.c src\bst.c ^
    src\sort.c src\dados.c src\ui.c src\tela_menu.c ^
    src\tela_cadastro.c src\tela_caixa.c src\tela_relatorios.c ^
    src\tela_auditoria.c ^
    -L C:\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
```

**Flags explicadas:**
- `-std=c99`: usa o padrão C99 (necessário para `//` comentários, declarações no meio do bloco, `bool`, etc.)
- `-Wall -Wextra`: habilita todos os avisos do compilador (boa prática)
- `-O2`: otimização nível 2 (código mais rápido sem comprometer debugabilidade)
- `-I C:\raylib\include`: informa o diretório dos headers da Raylib
- `-L C:\raylib\lib`: informa o diretório das bibliotecas `.a`
- `-lraylib`: linka a biblioteca Raylib
- `-lopengl32 -lgdi32 -lwinmm`: dependências do Windows necessárias para o Raylib (OpenGL, GDI e Multimedia)
- `-mwindows`: cria um executável Windows sem janela de console (evita o terminal preto ao abrir)

### Execução

Execute `mercadinho.exe` de dentro da pasta do projeto (não de outra pasta, pois ele procura `data/produtos.txt` relativo ao diretório atual).

Na **primeira execução**, os arquivos `data/produtos.txt` e `data/clientes.txt` são gerados automaticamente com 1000 registros cada. Nas execuções seguintes, os arquivos são apenas carregados.

---

## 22. Conceitos de Estrutura de Dados — Resumo para Defesa

### Tabela Hash

**O que é:** Estrutura que usa uma função para mapear chaves a índices de array, permitindo busca em O(1) médio.

**Como funciona neste projeto:**
- Função hash: `codigo % 1009`
- Colisões resolvidas por **encadeamento externo** (listas encadeadas em cada bucket)
- `calloc` inicializa todos os ponteiros como NULL
- Inserção sempre na cabeça da lista (O(1))
- Busca percorre a lista do bucket correspondente

**Por que 1009?** É o menor número primo maior que 1000. Números primos minimizam colisões na função módulo porque têm menos divisores em comum com os valores das chaves.

**Fator de carga:** com 1000 produtos e 1009 buckets, o fator é ≈0,99. Buckets com colisão terão listas de 2 elementos — a busca ainda é essencialmente O(1).

---

### Fila

**O que é:** FIFO (First In First Out). O primeiro elemento inserido é o primeiro a sair.

**Como funciona neste projeto:**
- Inserção (enfileirar) no final: O(1) graças ao ponteiro `tras`
- Remoção (desenfileirar) do início: O(1) usando o ponteiro `frente`
- Cada cliente que chega vai para o fim; o próximo a ser atendido sai do início

**Diferença de array:** com array, remover do início exigiria deslocar todos os elementos (O(n)). Com lista encadeada + ponteiros frente/tras, ambas as operações são O(1).

---

### Pilha

**O que é:** LIFO (Last In First Out). O último elemento inserido é o primeiro a sair.

**Como funciona neste projeto:**
- Push (empilhar): insere no topo
- Pop (desempilhar): remove do topo
- O topo é sempre o último produto bipado — pressionar Undo remove exatamente esse produto e devolve ao estoque

**Por que pilha para carrinho?** O Undo precisa remover o último item adicionado — essa semântica é exatamente LIFO.

---

### BST (Árvore Binária de Busca)

**O que é:** Árvore onde, para qualquer nó, todos os filhos à esquerda têm chave menor e à direita têm chave maior.

**Como funciona neste projeto:**
- Chave = `id_venda` (sequencial: 1, 2, 3...)
- Busca por ID: compara e vai esquerda/direita até encontrar — O(log n) médio
- Caminhamento in-order (ESQ → NÓ → DIR): visita nós em ordem crescente de ID — usado no relatório cronológico

**Atenção prática:** Como os IDs são inseridos em ordem crescente, a árvore cresce sempre para a direita, degenerando em O(n). Para um trabalho com dezenas de vendas, o impacto é desprezível.

---

### Quick Sort

**O que é:** Algoritmo de ordenação divide-e-conquista, custo médio O(n log n).

**Como funciona neste projeto:**
1. Escolhe o pivô (último elemento)
2. Particionamento: elementos maiores que o pivô vão para a esquerda, menores para a direita (ordenação decrescente)
3. Recursão nas duas metades
4. O array de Vendas é reordenado in-place (sem alocar novo array)

**Por que in-place?** O `malloc` do array já foi feito em `bst_extrair_vetor`. O quicksort apenas reorganiza os elementos dentro desse array, sem custo adicional de memória.

---

### In-Order (Caminhamento em árvore)

O caminhamento **em-ordem** (in-order) em uma BST sempre visita os nós em **ordem crescente da chave**. Isso é uma propriedade matemática da BST:

```
     4
    / \
   2   6
  / \ / \
 1  3 5  7

In-order: 1 → 2 → 3 → 4 → 5 → 6 → 7
```

No projeto, como a chave é `id_venda`, o in-order produz as vendas ordenadas cronologicamente (menor ID = primeira venda).
