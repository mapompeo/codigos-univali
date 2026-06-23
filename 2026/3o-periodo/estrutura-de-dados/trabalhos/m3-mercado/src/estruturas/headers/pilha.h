#ifndef PILHA_H
#define PILHA_H

#include "../../types.h"

/* Nó da pilha */
typedef struct PilhaNode {
    Produto           produto;
    struct PilhaNode *prox;
} PilhaNode;

/* Pilha Dinamica (carrinho de compras) */
typedef struct {
    PilhaNode *topo;
    int        tamanho;
    float      total;
} Pilha;

Pilha *pilha_criar(void);
void   pilha_destruir(Pilha *p);
void   pilha_empilhar(Pilha *p, Produto prod);
int    pilha_desempilhar(Pilha *p, Produto *out);  /* 1=ok, 0=vazia */
int    pilha_vazia(Pilha *p);
void   pilha_limpar(Pilha *p);

#endif /* PILHA_H */
