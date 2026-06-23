#ifndef TELA_RELATORIOS_H
#define TELA_RELATORIOS_H

#include "../../estado.h"
#include "ui.h"

typedef enum {
    REL_NENHUM = 0,
    REL_INORDER,
    REL_FATURAMENTO
} ModoRelatorio;

typedef struct {
    ModoRelatorio modo;
    Venda        *vetor;      /* malloc — liberado ao trocar modo ou sair */
    int           count;
    float         scroll;
} TelaRelatoriosState;

void tela_relatorios_init(TelaRelatoriosState *st);
void tela_relatorios_limpar(TelaRelatoriosState *st);
void tela_relatorios_desenhar(EstadoApp *app, TelaRelatoriosState *st);

#endif
