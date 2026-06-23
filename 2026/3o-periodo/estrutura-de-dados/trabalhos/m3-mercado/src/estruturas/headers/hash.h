#ifndef HASH_H
#define HASH_H

#include "../../types.h"

/* Nó da lista encadeada (encadeamento externo para colisões) */
typedef struct HashNode {
    Produto          produto;
    struct HashNode *prox;
} HashNode;

/* Tabela Hash */
typedef struct {
    HashNode **tabela;
    int        tamanho;
    int        total;
} TabelaHash;

TabelaHash *hash_criar(int tamanho);
void        hash_destruir(TabelaHash *h);
int         hash_funcao(TabelaHash *h, int codigo);
int         hash_inserir(TabelaHash *h, Produto p);          /* 1=novo, 0=atualizado */
Produto    *hash_buscar(TabelaHash *h, int codigo);          /* NULL se nao existe  */
int         hash_atualizar_quantidade(TabelaHash *h, int codigo, int delta); /* novo qty ou -1/-2 */

#endif /* HASH_H */
