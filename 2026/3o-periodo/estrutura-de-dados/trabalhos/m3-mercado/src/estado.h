#ifndef ESTADO_H
#define ESTADO_H

#include <stdbool.h>
#include "types.h"
#include "estruturas/headers/hash.h"
#include "estruturas/headers/fila.h"
#include "estruturas/headers/pilha.h"
#include "estruturas/headers/bst.h"

/* ------------------------------------------------------------------ */
/*  Estado global da aplicacao (passado por ponteiro para cada tela)   */
/* ------------------------------------------------------------------ */
typedef struct {
    /* Estruturas de dados principais */
    TabelaHash *estoque;
    Fila       *fila_clientes;
    Pilha      *carrinho;
    BST        *historico;

    /* Navegacao */
    Tela tela_atual;

    /* Cliente em atendimento */
    bool    tem_cliente;
    Cliente cliente_atual;

    /* Mensagem de status (banner inferior) */
    char  msg_status[256];
    float msg_timer;
    bool  msg_erro;

    /* Dimensoes da janela */
    int largura;
    int altura;
} EstadoApp;

/* Exibe mensagem de feedback por 3 segundos */
static inline void estado_msg(EstadoApp *app, const char *msg, bool erro) {
    int i = 0;
    while (msg[i] && i < 255) { app->msg_status[i] = msg[i]; i++; }
    app->msg_status[i] = '\0';
    app->msg_timer     = 3.0f;
    app->msg_erro      = erro;
}

#endif /* ESTADO_H */
