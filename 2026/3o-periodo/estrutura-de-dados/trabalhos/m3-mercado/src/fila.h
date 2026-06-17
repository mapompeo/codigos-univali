#ifndef FILA_H
#define FILA_H

#include "types.h"

/* Nó da fila */
typedef struct FilaNode {
    Cliente          cliente;
    struct FilaNode *prox;
} FilaNode;

/* Fila Dinamica baseada em ponteiros */
typedef struct {
    FilaNode *frente;
    FilaNode *tras;
    int       tamanho;
} Fila;

Fila *fila_criar(void);
void  fila_destruir(Fila *f);
void  fila_enfileirar(Fila *f, Cliente c);
int   fila_desenfileirar(Fila *f, Cliente *out);  /* 1=ok, 0=vazia */
int   fila_vazia(Fila *f);

#endif /* FILA_H */
