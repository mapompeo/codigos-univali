#ifndef TYPES_H
#define TYPES_H

#define MAX_NOME      100
#define MAX_TEXTO     256
#define HASH_TAMANHO  1009   /* numero primo > 1000 */

/* ------------------------------------------------------------------ */
/*  Produto                                                             */
/* ------------------------------------------------------------------ */
typedef struct {
    int   codigo;
    char  nome[MAX_NOME];
    float preco;
    int   quantidade;
    int   dia, mes, ano;
} Produto;

/* ------------------------------------------------------------------ */
/*  Cliente                                                             */
/* ------------------------------------------------------------------ */
typedef struct {
    char nome[MAX_NOME];
    int  dia, mes, ano;
} Cliente;

/* ------------------------------------------------------------------ */
/*  Item de Venda  (lista encadeada para registrar o que foi vendido)  */
/* ------------------------------------------------------------------ */
typedef struct ItemVenda {
    int   codigo;
    char  nome[MAX_NOME];
    float preco;
    struct ItemVenda *prox;
} ItemVenda;

/* ------------------------------------------------------------------ */
/*  Venda                                                               */
/* ------------------------------------------------------------------ */
typedef struct {
    int        id_venda;
    char       cliente[MAX_NOME];
    float      valor_total;
    int        dia, mes, ano;
    ItemVenda *itens;
    int        num_itens;
} Venda;

/* ------------------------------------------------------------------ */
/*  Identificadores de tela                                             */
/* ------------------------------------------------------------------ */
typedef enum {
    TELA_MENU = 0,
    TELA_CADASTRO,
    TELA_CAIXA,
    TELA_RELATORIOS,
    TELA_AUDITORIA
} Tela;

#endif /* TYPES_H */
