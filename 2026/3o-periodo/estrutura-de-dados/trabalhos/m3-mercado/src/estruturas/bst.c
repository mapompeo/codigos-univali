#include "headers/bst.h"
#include <stdlib.h>

BST *bst_criar(void) {
    BST *b        = (BST *)malloc(sizeof(BST));
    b->raiz       = NULL;
    b->proximo_id = 1;
    b->total      = 0;
    return b;
}

static void destruir_no(BSTNode *node) {
    if (!node) return;
    destruir_no(node->esq);
    destruir_no(node->dir);
    /* libera lista de itens da venda */
    ItemVenda *item = node->venda.itens;
    while (item) {
        ItemVenda *prox = item->prox;
        free(item);
        item = prox;
    }
    free(node);
}

void bst_destruir(BST *b) {
    destruir_no(b->raiz);
    free(b);
}

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

BSTNode *bst_buscar(BST *b, int id) {
    BSTNode *atual = b->raiz;
    while (atual) {
        if (id == atual->venda.id_venda) return atual;
        atual = (id < atual->venda.id_venda) ? atual->esq : atual->dir;
    }
    return NULL;
}

void bst_inorder(BSTNode *raiz, Venda *vetor, int *idx) {
    if (!raiz) return;
    bst_inorder(raiz->esq, vetor, idx);
    vetor[(*idx)++] = raiz->venda;
    bst_inorder(raiz->dir, vetor, idx);
}

Venda *bst_extrair_vetor(BST *b, int *count) {
    *count = b->total;
    if (*count == 0) return NULL;
    Venda *v = (Venda *)malloc(sizeof(Venda) * (*count));
    int idx  = 0;
    bst_inorder(b->raiz, v, &idx);
    return v;
}
