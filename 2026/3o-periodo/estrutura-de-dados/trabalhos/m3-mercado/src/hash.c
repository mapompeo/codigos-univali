#include "hash.h"
#include <stdlib.h>
#include <string.h>

TabelaHash *hash_criar(int tamanho) {
    TabelaHash *h = (TabelaHash *)malloc(sizeof(TabelaHash));
    h->tamanho = tamanho;
    h->total   = 0;
    h->tabela  = (HashNode **)calloc(tamanho, sizeof(HashNode *));
    return h;
}

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

int hash_funcao(TabelaHash *h, int codigo) {
    int idx = codigo % h->tamanho;
    return (idx < 0) ? idx + h->tamanho : idx;
}

int hash_inserir(TabelaHash *h, Produto p) {
    int       idx  = hash_funcao(h, p.codigo);
    HashNode *node = h->tabela[idx];

    /* verifica se ja existe — se sim, atualiza */
    while (node) {
        if (node->produto.codigo == p.codigo) {
            node->produto = p;
            return 0;
        }
        node = node->prox;
    }

    /* insere na cabeca da lista */
    HashNode *novo = (HashNode *)malloc(sizeof(HashNode));
    novo->produto  = p;
    novo->prox     = h->tabela[idx];
    h->tabela[idx] = novo;
    h->total++;
    return 1;
}

Produto *hash_buscar(TabelaHash *h, int codigo) {
    int       idx  = hash_funcao(h, codigo);
    HashNode *node = h->tabela[idx];
    while (node) {
        if (node->produto.codigo == codigo) return &node->produto;
        node = node->prox;
    }
    return NULL;
}

int hash_atualizar_quantidade(TabelaHash *h, int codigo, int delta) {
    Produto *p = hash_buscar(h, codigo);
    if (!p) return -1;           /* produto nao encontrado */
    if (p->quantidade + delta < 0) return -2;  /* estoque insuficiente */
    p->quantidade += delta;
    return p->quantidade;
}
