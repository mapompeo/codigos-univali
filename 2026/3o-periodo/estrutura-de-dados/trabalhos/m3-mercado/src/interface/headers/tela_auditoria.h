#ifndef TELA_AUDITORIA_H
#define TELA_AUDITORIA_H

#include "../../estado.h"
#include "ui.h"

typedef struct {
    CampoTexto f_id;
    BSTNode   *resultado;     /* NULL se nao encontrado ou sem busca */
    bool       buscou;
} TelaAuditoriaState;

void tela_auditoria_init(TelaAuditoriaState *st);
void tela_auditoria_desenhar(EstadoApp *app, TelaAuditoriaState *st);

#endif
