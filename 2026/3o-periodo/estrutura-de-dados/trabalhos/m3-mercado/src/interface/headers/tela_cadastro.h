#ifndef TELA_CADASTRO_H
#define TELA_CADASTRO_H

#include "../../estado.h"
#include "ui.h"

typedef struct {
    CampoTexto f_codigo;
    CampoTexto f_nome;
    CampoTexto f_preco;
    CampoTexto f_qtd;
    CampoTexto f_dia;
    CampoTexto f_mes;
    CampoTexto f_ano;
} TelaCadastroState;

void tela_cadastro_init(TelaCadastroState *st);
void tela_cadastro_desenhar(EstadoApp *app, TelaCadastroState *st);

#endif
