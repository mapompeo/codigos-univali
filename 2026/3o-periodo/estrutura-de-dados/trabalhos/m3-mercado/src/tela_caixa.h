#ifndef TELA_CAIXA_H
#define TELA_CAIXA_H

#include "estado.h"
#include "ui.h"

typedef struct {
    CampoTexto f_cod;
} TelaCaixaState;

void tela_caixa_init(TelaCaixaState *st);
void tela_caixa_desenhar(EstadoApp *app, TelaCaixaState *st);

#endif
