#ifndef BST_H
#define BST_H

#include "../../types.h"

/* No da Arvore Binaria de Busca */
typedef struct BSTNode {
    Venda          venda;
    struct BSTNode *esq;
    struct BSTNode *dir;
} BSTNode;

/* BST (historico de vendas) */
typedef struct {
    BSTNode *raiz;
    int      proximo_id;
    int      total;
} BST;

BST     *bst_criar(void);
void     bst_destruir(BST *b);
void     bst_inserir(BST *b, Venda v);
BSTNode *bst_buscar(BST *b, int id);

/* caminhamento em-ordem para preencher vetor ja alocado */
void     bst_inorder(BSTNode *raiz, Venda *vetor, int *idx);

/* extrai todos os nos para vetor alocado com malloc — chamador libera com free() */
Venda   *bst_extrair_vetor(BST *b, int *count);

#endif /* BST_H */
