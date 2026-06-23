#include "headers/tela_relatorios.h"
#include "../estruturas/headers/sort.h"
#include <stdio.h>
#include <stdlib.h>

void tela_relatorios_init(TelaRelatoriosState *st) {
    st->modo   = REL_NENHUM;
    st->vetor  = NULL;
    st->count  = 0;
    st->scroll = 0.0f;
}

void tela_relatorios_limpar(TelaRelatoriosState *st) {
    if (st->vetor) { free(st->vetor); st->vetor = NULL; }
    st->count  = 0;
    st->scroll = 0.0f;
    st->modo   = REL_NENHUM;
}

void tela_relatorios_desenhar(EstadoApp *app, TelaRelatoriosState *st) {
    ClearBackground(COR_FUNDO);
    ui_header("RELATORIOS GERENCIAIS", COR_AVISO);

    if (ui_botao_voltar()) {
        tela_relatorios_limpar(st);
        app->tela_atual = TELA_MENU;
        return;
    }

    int W = app->largura;
    int H = app->altura;

    /* --- botoes de selecao de relatorio --- */
    int btn_y = 72;
    int btn_h = 42;
    int btn_w = (W - 40 - 14) / 2;

    Rectangle b_id  = { 20,          btn_y, btn_w, btn_h };
    Rectangle b_fat = { 20 + btn_w + 14, btn_y, btn_w, btn_h };

    Color cor_id  = (st->modo == REL_INORDER)      ? COR_PRIMARIA : COR_INFO;
    Color cor_fat = (st->modo == REL_FATURAMENTO)  ? COR_PRIMARIA : COR_AVISO;

    if (ui_botao(b_id, "RELATORIO POR ID (EM-ORDEM)",
                 cor_id, COR_HOVER, WHITE, 17)) {
        tela_relatorios_limpar(st);
        st->modo  = REL_INORDER;
        st->vetor = bst_extrair_vetor(app->historico, &st->count);
        /* vetor ja vem em ordem crescente de id pelo in-order */
    }

    if (ui_botao(b_fat, "RANKING DE FATURAMENTO (DECRESCENTE)",
                 cor_fat, COR_AVISO_H, WHITE, 17)) {
        tela_relatorios_limpar(st);
        st->modo  = REL_FATURAMENTO;
        st->vetor = bst_extrair_vetor(app->historico, &st->count);
        if (st->vetor && st->count > 1)
            quicksort_vendas_desc(st->vetor, 0, st->count - 1);
    }

    /* --- area da lista --- */
    int list_y = btn_y + btn_h + 10;
    int list_h = H - list_y - 50;
    Rectangle area = { 20, (float)list_y, W - 40, (float)list_h };
    ui_card(area, COR_CARD);

    if (st->modo == REL_NENHUM) {
        const char *hint = "Selecione um relatorio acima.";
        int htw = MeasureText(hint, 18);
        DrawText(hint, W / 2 - htw / 2, list_y + list_h / 2 - 9, 18, COR_SUBTEXTO);
        return;
    }

    if (!st->vetor || st->count == 0) {
        const char *empty = "Nenhuma venda registrada.";
        int etw = MeasureText(empty, 18);
        DrawText(empty, W / 2 - etw / 2, list_y + list_h / 2 - 9, 18, COR_SUBTEXTO);
        return;
    }

    /* cabecalho da tabela */
    int hx = 28;
    DrawText("Pos", hx,       list_y + 8, 15, COR_SUBTEXTO);
    DrawText("ID",  hx + 50,  list_y + 8, 15, COR_SUBTEXTO);
    DrawText("Cliente", hx + 120, list_y + 8, 15, COR_SUBTEXTO);
    DrawText("Data",    hx + 440, list_y + 8, 15, COR_SUBTEXTO);
    DrawText("Itens",   hx + 560, list_y + 8, 15, COR_SUBTEXTO);
    DrawText("Total",   hx + 640, list_y + 8, 15, COR_SUBTEXTO);
    DrawLine(28, list_y + 26, W - 28, list_y + 26, COR_BORDA);

    /* scroll */
    float item_h      = 28.0f;
    float conteudo_h  = item_h * st->count;
    float area_h      = (float)(list_h - 30);

    /* captura scroll apenas quando mouse esta sobre a area */
    Vector2 mp = GetMousePosition();
    if (CheckCollisionPointRec(mp, area))
        ui_scroll_ajustar(&st->scroll, conteudo_h, area_h);

    /* clip: usa scissor mode para nao transbordar */
    BeginScissorMode((int)area.x, list_y + 28,
                     (int)area.width, (int)area_h);

    for (int i = 0; i < st->count; i++) {
        float y = (float)(list_y + 28) + i * item_h - st->scroll;
        if (y + item_h < list_y + 28) continue;
        if (y > list_y + 28 + area_h)   break;

        Color bg = (i % 2 == 0) ? COR_CARD : COR_CARD2;
        DrawRectangle(22, (int)y, W - 44, (int)item_h - 2, bg);

        /* posicao */
        char pos[8];
        snprintf(pos, 8, "%d", i + 1);
        DrawText(pos, hx, (int)y + 7, 15, COR_SUBTEXTO);

        /* ID */
        char idb[12];
        snprintf(idb, 12, "#%04d", st->vetor[i].id_venda);
        DrawText(idb, hx + 50, (int)y + 7, 15,
                 (st->modo == REL_INORDER) ? COR_INFO : COR_AVISO);

        /* cliente (truncado) */
        char cln[40];
        snprintf(cln, 40, "%.38s", st->vetor[i].cliente);
        DrawText(cln, hx + 120, (int)y + 7, 15, COR_TEXTO);

        /* data */
        char dat[16];
        snprintf(dat, 16, "%02d/%02d/%04d",
                 st->vetor[i].dia,
                 st->vetor[i].mes,
                 st->vetor[i].ano);
        DrawText(dat, hx + 440, (int)y + 7, 15, COR_SUBTEXTO);

        /* num itens */
        char nit[8];
        snprintf(nit, 8, "%d", st->vetor[i].num_itens);
        DrawText(nit, hx + 565, (int)y + 7, 15, COR_SUBTEXTO);

        /* valor total */
        char val[24];
        snprintf(val, 24, "R$ %.2f", st->vetor[i].valor_total);
        int vtw = MeasureText(val, 15);
        DrawText(val, W - 30 - vtw, (int)y + 7, 15, COR_PRIMARIA);
    }

    EndScissorMode();

    /* total de registros */
    char tot[48];
    snprintf(tot, 48, "Total: %d venda(s)", st->count);
    DrawText(tot, W - 28 - MeasureText(tot, 14), H - 46, 14, COR_SUBTEXTO);

    ui_status(app->msg_status, app->msg_erro, &app->msg_timer);
}
