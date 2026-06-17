#include "pilha.h"
#include <stdlib.h>

Pilha *pilha_criar(void) {
    Pilha *p   = (Pilha *)malloc(sizeof(Pilha));
    p->topo    = NULL;
    p->tamanho = 0;
    p->total   = 0.0f;
    return p;
}

void pilha_destruir(Pilha *p) {
    pilha_limpar(p);
    free(p);
}

void pilha_empilhar(Pilha *p, Produto prod) {
    PilhaNode *novo = (PilhaNode *)malloc(sizeof(PilhaNode));
    novo->produto   = prod;
    novo->prox      = p->topo;
    p->topo         = novo;
    p->tamanho++;
    p->total += prod.preco;
}

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

int pilha_vazia(Pilha *p) {
    return p->topo == NULL;
}

void pilha_limpar(Pilha *p) {
    Produto tmp;
    while (!pilha_vazia(p)) pilha_desempilhar(p, &tmp);
}
