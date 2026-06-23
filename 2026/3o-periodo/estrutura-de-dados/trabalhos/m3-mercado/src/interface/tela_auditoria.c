#include "headers/tela_auditoria.h"
#include <stdio.h>
#include <stdlib.h>

void tela_auditoria_init(TelaAuditoriaState *st) {
    campo_init(&st->f_id,
               (Rectangle){ 220, 110, 180, 38 },
               "Numero do ID", true);
    st->resultado = NULL;
    st->buscou    = false;
}

void tela_auditoria_desenhar(EstadoApp *app, TelaAuditoriaState *st) {
    ClearBackground(COR_FUNDO);
    ui_header("AUDITORIA  |  CONSULTA DE VENDAS", COR_ROXO);

    if (ui_botao_voltar()) {
        app->tela_atual = TELA_MENU;
        return;
    }

    int W = app->largura;
    int H = app->altura;

    /* --- painel de busca --- */
    ui_card((Rectangle){ 20, 72, W - 40, 72 }, COR_CARD);

    DrawText("ID da Venda:", 35, 90, 18, COR_TEXTO);

    st->f_id.bounds.x = 220;
    st->f_id.bounds.y = 85;
    campo_atualizar(&st->f_id);
    campo_desenhar(&st->f_id);

    Rectangle btn_busca = { 415, 85, 130, 38 };
    bool buscar = ui_botao(btn_busca, "BUSCAR", COR_ROXO, COR_ROXO_H, WHITE, 17);

    if (buscar || (st->f_id.ativo && IsKeyPressed(KEY_ENTER))) {
        if (st->f_id.len > 0) {
            int id        = atoi(st->f_id.text);
            st->resultado = bst_buscar(app->historico, id);
            st->buscou    = true;
        }
    }

    /* hint de atalho */
    DrawText("(ou pressione Enter no campo)", 560, 96, 13, COR_SUBTEXTO);

    /* --- area de resultado --- */
    int ry = 155;
    int rh = H - ry - 50;
    ui_card((Rectangle){ 20, (float)ry, W - 40, (float)rh }, COR_CARD);

    if (!st->buscou) {
        const char *hint = "Digite o ID de uma venda e clique Buscar.";
        int hw = MeasureText(hint, 18);
        DrawText(hint, W / 2 - hw / 2, ry + rh / 2 - 9, 18, COR_SUBTEXTO);
        ui_status(app->msg_status, app->msg_erro, &app->msg_timer);
        return;
    }

    if (!st->resultado) {
        char err[64];
        snprintf(err, 64, "Venda #%s nao encontrada na arvore.", st->f_id.text);
        int ew = MeasureText(err, 18);
        DrawText(err, W / 2 - ew / 2, ry + rh / 2 - 9, 18, COR_PERIGO);
        ui_status(app->msg_status, app->msg_erro, &app->msg_timer);
        return;
    }

    Venda *v = &st->resultado->venda;

    /* cabecalho da venda */
    int ix = 35;
    int iy = ry + 14;

    char hdr[64];
    snprintf(hdr, 64, "CUPOM FISCAL  #%04d", v->id_venda);
    int hw = MeasureText(hdr, 22);
    DrawText(hdr, W / 2 - hw / 2, iy, 22, COR_ROXO);
    iy += 32;

    DrawLine(28, iy, W - 28, iy, COR_BORDA);
    iy += 10;

    /* info da venda */
    char dbuf[80];
    snprintf(dbuf, 80, "Cliente:  %s", v->cliente);
    DrawText(dbuf, ix, iy, 17, COR_TEXTO);
    iy += 26;

    char datbuf[40];
    snprintf(datbuf, 40, "Data:     %02d/%02d/%04d", v->dia, v->mes, v->ano);
    DrawText(datbuf, ix, iy, 17, COR_TEXTO);
    iy += 26;

    char nitbuf[40];
    snprintf(nitbuf, 40, "Itens:    %d produto(s)", v->num_itens);
    DrawText(nitbuf, ix, iy, 17, COR_TEXTO);
    iy += 30;

    DrawLine(28, iy, W - 28, iy, COR_BORDA);
    iy += 8;

    /* lista de itens */
    DrawText("Produto",  ix,         iy, 15, COR_SUBTEXTO);
    DrawText("Codigo",   ix + 380,   iy, 15, COR_SUBTEXTO);
    DrawText("Preco",    ix + 480,   iy, 15, COR_SUBTEXTO);
    iy += 22;
    DrawLine(28, iy, W - 28, iy, COR_BORDA);
    iy += 4;

    ItemVenda *item = v->itens;
    int        pos  = 0;
    while (item && iy < ry + rh - 60) {
        Color bg = (pos % 2 == 0) ? COR_CARD : COR_CARD2;
        DrawRectangle(22, iy, W - 44, 24, bg);

        char nbuf[MAX_NOME];
        snprintf(nbuf, MAX_NOME, "%.55s", item->nome);
        DrawText(nbuf, ix, iy + 4, 15, COR_TEXTO);

        char cbuf[16];
        snprintf(cbuf, 16, "%d", item->codigo);
        DrawText(cbuf, ix + 380, iy + 4, 15, COR_SUBTEXTO);

        char pbuf[20];
        snprintf(pbuf, 20, "R$ %.2f", item->preco);
        int ptw = MeasureText(pbuf, 15);
        DrawText(pbuf, W - 28 - ptw, iy + 4, 15, COR_PRIMARIA);

        iy   += 26;
        pos++;
        item  = item->prox;
    }

    /* linha total */
    iy = ry + rh - 48;
    DrawLine(28, iy, W - 28, iy, COR_BORDA);
    DrawText("TOTAL:", ix, iy + 10, 18, COR_TEXTO);
    char totbuf[32];
    snprintf(totbuf, 32, "R$ %.2f", v->valor_total);
    int ttw = MeasureText(totbuf, 22);
    DrawText(totbuf, W - 28 - ttw, iy + 8, 22, COR_PRIMARIA);

    ui_status(app->msg_status, app->msg_erro, &app->msg_timer);
}
