#include "fila.h"
#include <stdlib.h>

Fila *fila_criar(void) {
    Fila *f    = (Fila *)malloc(sizeof(Fila));
    f->frente  = NULL;
    f->tras    = NULL;
    f->tamanho = 0;
    return f;
}

void fila_destruir(Fila *f) {
    Cliente tmp;
    while (!fila_vazia(f)) fila_desenfileirar(f, &tmp);
    free(f);
}

void fila_enfileirar(Fila *f, Cliente c) {
    FilaNode *novo = (FilaNode *)malloc(sizeof(FilaNode));
    novo->cliente  = c;
    novo->prox     = NULL;
    if (f->tras) f->tras->prox = novo;
    else         f->frente     = novo;
    f->tras = novo;
    f->tamanho++;
}

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

int fila_vazia(Fila *f) {
    return f->frente == NULL;
}
