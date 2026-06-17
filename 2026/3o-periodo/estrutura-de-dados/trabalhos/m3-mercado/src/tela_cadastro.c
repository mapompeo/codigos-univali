#include "tela_cadastro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LABEL_X  40
#define FIELD_X  220
#define FIELD_W  320
#define FIELD_H  36
#define ROW_GAP  52

void tela_cadastro_init(TelaCadastroState *st) {
    int y0 = 130;
    campo_init(&st->f_codigo, (Rectangle){ FIELD_X, y0 + 0*ROW_GAP, FIELD_W, FIELD_H }, "Ex: 2000", true);
    campo_init(&st->f_nome,   (Rectangle){ FIELD_X, y0 + 1*ROW_GAP, FIELD_W + 200, FIELD_H }, "Ex: Arroz Tipo 1 5kg", false);
    campo_init(&st->f_preco,  (Rectangle){ FIELD_X, y0 + 2*ROW_GAP, 160, FIELD_H }, "Ex: 15.90", false);
    campo_init(&st->f_qtd,    (Rectangle){ FIELD_X, y0 + 3*ROW_GAP, 120, FIELD_H }, "Ex: 50", true);
    campo_init(&st->f_dia,    (Rectangle){ FIELD_X,       y0 + 4*ROW_GAP, 70,  FIELD_H }, "DD",   true);
    campo_init(&st->f_mes,    (Rectangle){ FIELD_X + 82,  y0 + 4*ROW_GAP, 70,  FIELD_H }, "MM",   true);
    campo_init(&st->f_ano,    (Rectangle){ FIELD_X + 164, y0 + 4*ROW_GAP, 100, FIELD_H }, "AAAA", true);
    /* pre-preenche data de hoje */
    time_t t  = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(st->f_dia.text, 256, "%02d", tm->tm_mday);
    snprintf(st->f_mes.text, 256, "%02d", tm->tm_mon + 1);
    snprintf(st->f_ano.text, 256, "%04d", tm->tm_year + 1900);
    st->f_dia.len = (int)strlen(st->f_dia.text);
    st->f_mes.len = (int)strlen(st->f_mes.text);
    st->f_ano.len = (int)strlen(st->f_ano.text);
}

void tela_cadastro_desenhar(EstadoApp *app, TelaCadastroState *st) {
    ClearBackground(COR_FUNDO);
    ui_header("CADASTRO DE PRODUTOS", COR_INFO);

    /* botao voltar */
    if (ui_botao_voltar()) {
        app->tela_atual = TELA_MENU;
        return;
    }

    int W  = app->largura;
    int y0 = 130;

    /* --- card do formulario --- */
    ui_card((Rectangle){ 20, 78, W - 40, 320 }, COR_CARD);

    /* labels */
    const char *labels[] = {
        "Codigo:", "Nome do Produto:", "Preco (R$):",
        "Quantidade:", "Data de Cadastro:"
    };
    for (int i = 0; i < 5; i++) {
        DrawText(labels[i], LABEL_X, y0 + i * ROW_GAP + 9, 17, COR_TEXTO);
    }

    /* atualiza e desenha campos */
    campo_atualizar(&st->f_codigo);
    campo_atualizar(&st->f_nome);
    campo_atualizar(&st->f_preco);
    campo_atualizar(&st->f_qtd);
    campo_atualizar(&st->f_dia);
    campo_atualizar(&st->f_mes);
    campo_atualizar(&st->f_ano);

    campo_desenhar(&st->f_codigo);
    campo_desenhar(&st->f_nome);
    campo_desenhar(&st->f_preco);
    campo_desenhar(&st->f_qtd);
    campo_desenhar(&st->f_dia);
    campo_desenhar(&st->f_mes);
    campo_desenhar(&st->f_ano);

    /* separadores visuais entre data */
    int dy = y0 + 4 * ROW_GAP;
    DrawText("/", FIELD_X + 74, dy + 9, 18, COR_SUBTEXTO);
    DrawText("/", FIELD_X + 156, dy + 9, 18, COR_SUBTEXTO);

    /* --- botao cadastrar --- */
    Rectangle btn_cad = { FIELD_X, y0 + 5 * ROW_GAP + 4, 200, 42 };
    if (ui_botao(btn_cad, "CADASTRAR", COR_INFO, COR_INFO_H, WHITE, 18)) {
        /* validacao */
        if (st->f_codigo.len == 0 || st->f_nome.len == 0 ||
            st->f_preco.len  == 0 || st->f_qtd.len  == 0) {
            estado_msg(app, "Preencha todos os campos obrigatorios!", true);
        } else {
            Produto p;
            p.codigo     = atoi(st->f_codigo.text);
            strncpy(p.nome, st->f_nome.text, MAX_NOME - 1);
            p.nome[MAX_NOME - 1] = '\0';
            p.preco      = (float)atof(st->f_preco.text);
            p.quantidade = atoi(st->f_qtd.text);
            p.dia        = st->f_dia.len > 0 ? atoi(st->f_dia.text) : 1;
            p.mes        = st->f_mes.len > 0 ? atoi(st->f_mes.text) : 1;
            p.ano        = st->f_ano.len > 0 ? atoi(st->f_ano.text) : 2026;

            if (p.codigo <= 0 || p.preco <= 0 || p.quantidade < 0) {
                estado_msg(app, "Valores invalidos! Codigo, preco e quantidade devem ser positivos.", true);
            } else {
                int res = hash_inserir(app->estoque, p);
                if (res == 1)
                    estado_msg(app, "Produto cadastrado com sucesso!", false);
                else
                    estado_msg(app, "Produto atualizado (codigo ja existia).", false);

                /* limpa campos de codigo/nome/preco/qtd */
                campo_limpar(&st->f_codigo);
                campo_limpar(&st->f_nome);
                campo_limpar(&st->f_preco);
                campo_limpar(&st->f_qtd);
            }
        }
    }

    /* --- painel de consulta rapida --- */
    int qy = y0 + 5 * ROW_GAP + 4 + 58;
    ui_card((Rectangle){ 20, qy, W - 40, 120 }, COR_CARD2);
    DrawText("Consulta Rapida por Codigo:", LABEL_X, qy + 14, 16, COR_SUBTEXTO);

    static CampoTexto f_busca;
    static bool f_busca_init = false;
    if (!f_busca_init) {
        campo_init(&f_busca, (Rectangle){ FIELD_X, qy + 10, 160, 34 }, "Codigo", true);
        f_busca_init = true;
    }
    f_busca.bounds.y = (float)(qy + 10);
    campo_atualizar(&f_busca);
    campo_desenhar(&f_busca);

    static char resultado_busca[256] = "";
    static bool resultado_erro      = false;

    Rectangle btn_b = { FIELD_X + 170, (float)(qy + 10), 110, 34 };
    if (ui_botao(btn_b, "BUSCAR", COR_PRIMARIA, COR_HOVER, WHITE, 16)) {
        if (f_busca.len > 0) {
            int cod = atoi(f_busca.text);
            Produto *p = hash_buscar(app->estoque, cod);
            if (p) {
                snprintf(resultado_busca, 256,
                         "[%d] %s  |  R$ %.2f  |  Qtd: %d",
                         p->codigo, p->nome, p->preco, p->quantidade);
                resultado_erro = false;
            } else {
                snprintf(resultado_busca, 256, "Produto codigo %d nao encontrado.", cod);
                resultado_erro = true;
            }
        }
    }

    if (resultado_busca[0] != '\0') {
        Color rc = resultado_erro ? COR_PERIGO : COR_PRIMARIA;
        DrawText(resultado_busca, LABEL_X, qy + 62, 16, rc);
    }

    /* status */
    ui_status(app->msg_status, app->msg_erro, &app->msg_timer);
}
