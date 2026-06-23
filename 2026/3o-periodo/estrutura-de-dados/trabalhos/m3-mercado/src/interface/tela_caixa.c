#include "headers/tela_caixa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void tela_caixa_init(TelaCaixaState *st) {
    campo_init(&st->f_cod,
               (Rectangle){ 220, 165, 200, 36 },
               "Codigo do produto", true);
}

/* ------------------------------------------------------------------ */
/*  Desenho da tela de Frente de Caixa                                  */
/* ------------------------------------------------------------------ */
void tela_caixa_desenhar(EstadoApp *app, TelaCaixaState *st) {
    ClearBackground(COR_FUNDO);
    ui_header("FRENTE DE CAIXA  |  ATENDIMENTO", COR_PRIMARIA);

    if (ui_botao_voltar()) {
        app->tela_atual = TELA_MENU;
        return;
    }

    int W = app->largura;
    int H = app->altura;

    /* ---- PAINEL ESQUERDO: cliente em atendimento (x=0..419) ---- */
    int pw = 420;
    ui_card((Rectangle){ 10, 70, pw - 10, H - 130 }, COR_CARD);

    DrawText("CLIENTE EM ATENDIMENTO", 20, 82, 15, COR_SUBTEXTO);
    DrawLine(20, 100, pw - 10, 100, COR_BORDA);

    if (app->tem_cliente) {
        DrawText(app->cliente_atual.nome, 20, 110, 20, COR_PRIMARIA);
        char dbuf[64];
        snprintf(dbuf, 64, "Registro: %02d/%02d/%04d",
                 app->cliente_atual.dia,
                 app->cliente_atual.mes,
                 app->cliente_atual.ano);
        DrawText(dbuf, 20, 136, 15, COR_SUBTEXTO);
    } else {
        DrawText("Nenhum cliente.", 20, 112, 18, COR_SUBTEXTO);
    }

    /* Botao proximo cliente */
    Rectangle btn_prox = { 20, 168, pw - 30, 44 };
    if (ui_botao(btn_prox, "PROXIMO CLIENTE", COR_PRIMARIA, COR_HOVER, WHITE, 18)) {
        if (app->tem_cliente) {
            /* descarta o cliente atual sem finalizar */
            app->tem_cliente = false;
        }
        if (!fila_vazia(app->fila_clientes)) {
            fila_desenfileirar(app->fila_clientes, &app->cliente_atual);
            app->tem_cliente = true;
            /* limpa o carrinho do atendimento anterior */
            pilha_limpar(app->carrinho);
            estado_msg(app, "Novo cliente chamado!", false);
        } else {
            estado_msg(app, "Fila vazia. Nenhum cliente aguardando.", true);
        }
    }

    /* Fila resumida */
    char qbuf[64];
    snprintf(qbuf, 64, "Na fila: %d cliente(s)", app->fila_clientes->tamanho);
    DrawText(qbuf, 20, 222, 16, COR_SUBTEXTO);

    /* Separador */
    DrawLine(20, 245, pw - 10, 245, COR_BORDA);

    /* ---- PAINEL CENTRAL: bipe de produtos (x=420..850) ---- */
    int cx = pw + 10;
    int cw = 420;
    ui_card((Rectangle){ cx, 70, cw, H - 130 }, COR_CARD);

    DrawText("BIPE DE PRODUTOS", cx + 10, 82, 15, COR_SUBTEXTO);
    DrawLine(cx + 10, 100, cx + cw - 10, 100, COR_BORDA);

    DrawText("Codigo:", cx + 10, 120, 17, COR_TEXTO);

    /* reposiciona o campo de codigo */
    st->f_cod.bounds.x = (float)(cx + 10);
    st->f_cod.bounds.y     = 148.0f;
    st->f_cod.bounds.width = 200.0f;
    campo_atualizar(&st->f_cod);
    campo_desenhar(&st->f_cod);

    /* botao bipar */
    Rectangle btn_bipe = { cx + 220, 148, 120, 36 };
    bool bipe_clicado  = ui_botao(btn_bipe, "BIPAR", COR_ACENTO, COR_HOVER, WHITE, 17);

    /* tambem bipa ao pressionar Enter */
    if ((bipe_clicado || (st->f_cod.ativo && IsKeyPressed(KEY_ENTER)))
        && st->f_cod.len > 0) {

        if (!app->tem_cliente) {
            estado_msg(app, "Chame um cliente antes de bipar produtos!", true);
        } else {
            int cod    = atoi(st->f_cod.text);
            int res    = hash_atualizar_quantidade(app->estoque, cod, -1);
            if (res == -1) {
                estado_msg(app, "Produto nao encontrado no estoque!", true);
            } else if (res == -2) {
                estado_msg(app, "Estoque insuficiente para este produto!", true);
            } else {
                Produto *p = hash_buscar(app->estoque, cod);
                if (p) {
                    pilha_empilhar(app->carrinho, *p);
                    estado_msg(app, "Produto adicionado ao carrinho.", false);
                    campo_limpar(&st->f_cod);
                }
            }
        }
    }

    /* lista do carrinho (scroll dentro do painel) */
    DrawLine(cx + 10, 196, cx + cw - 10, 196, COR_BORDA);
    DrawText("Carrinho:", cx + 10, 200, 15, COR_SUBTEXTO);

    /* renderiza itens empilhados (do topo para base) */
    int item_y    = 218;
    int max_vis   = (H - 270) / 26;
    int cont      = 0;
    PilhaNode *n  = app->carrinho->topo;
    while (n && cont < max_vis) {
        Color bg = (cont % 2 == 0) ? COR_CARD : COR_CARD2;
        DrawRectangle(cx + 6, item_y - 2, cw - 12, 24, bg);
        char ibuf[128];
        snprintf(ibuf, 128, "%d  %s", cont + 1, n->produto.nome);
        DrawText(ibuf, cx + 12, item_y, 14, COR_TEXTO);
        char pbuf[24];
        snprintf(pbuf, 24, "R$ %.2f", n->produto.preco);
        int ptw = MeasureText(pbuf, 14);
        DrawText(pbuf, cx + cw - 20 - ptw, item_y, 14, COR_PRIMARIA);
        item_y += 26;
        cont++;
        n = n->prox;
    }
    if (app->carrinho->tamanho > max_vis) {
        char mbuf[40];
        snprintf(mbuf, 40, "... +%d itens acima",
                 app->carrinho->tamanho - max_vis);
        DrawText(mbuf, cx + 12, item_y, 13, COR_SUBTEXTO);
    }

    /* ---- PAINEL DIREITO: totais e acoes (x=850..) ---- */
    int rx = cx + cw + 10;
    int rw = W - rx - 10;
    ui_card((Rectangle){ rx, 70, rw, H - 130 }, COR_CARD);

    DrawText("RESUMO DA VENDA", rx + 10, 82, 15, COR_SUBTEXTO);
    DrawLine(rx + 10, 100, rx + rw - 10, 100, COR_BORDA);

    /* total e itens */
    char titens[40];
    snprintf(titens, 40, "%d item(ns)", app->carrinho->tamanho);
    DrawText(titens, rx + 10, 108, 16, COR_SUBTEXTO);

    char vtotal[64];
    snprintf(vtotal, 64, "R$ %.2f", app->carrinho->total);
    int vtw = MeasureText(vtotal, 30);
    DrawText("TOTAL:", rx + 10, 132, 17, COR_TEXTO);
    DrawText(vtotal, rx + rw / 2 - vtw / 2, 156, 30, COR_PRIMARIA);

    DrawLine(rx + 10, 196, rx + rw - 10, 196, COR_BORDA);

    /* botao desfazer */
    Rectangle btn_undo = { rx + 10, 206, rw - 20, 44 };
    if (ui_botao(btn_undo, "DESFAZER ULTIMO ITEM",
                 COR_AVISO, COR_AVISO_H, WHITE, 15)) {
        if (!pilha_vazia(app->carrinho)) {
            Produto removido;
            pilha_desempilhar(app->carrinho, &removido);
            hash_atualizar_quantidade(app->estoque, removido.codigo, +1);
            estado_msg(app, "Ultimo item removido. Estoque estornado.", false);
        } else {
            estado_msg(app, "Carrinho vazio, nada a desfazer.", true);
        }
    }

    /* botao finalizar */
    Rectangle btn_fin = { rx + 10, 260, rw - 20, 54 };
    if (ui_botao(btn_fin, "FINALIZAR VENDA",
                 COR_PRIMARIA, COR_HOVER, WHITE, 20)) {
        if (!app->tem_cliente) {
            estado_msg(app, "Sem cliente! Chame o proximo da fila.", true);
        } else if (pilha_vazia(app->carrinho)) {
            estado_msg(app, "Carrinho vazio. Adicione produtos antes.", true);
        } else {
            /* constroi a venda */
            Venda v;
            v.id_venda   = app->historico->proximo_id++;
            strncpy(v.cliente, app->cliente_atual.nome, MAX_NOME - 1);
            v.cliente[MAX_NOME - 1] = '\0';
            v.valor_total = app->carrinho->total;
            v.num_itens   = app->carrinho->tamanho;

            /* data de hoje */
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            v.dia = tm->tm_mday;
            v.mes = tm->tm_mon + 1;
            v.ano = tm->tm_year + 1900;

            /* copia itens da pilha para lista encadeada */
            v.itens = NULL;
            PilhaNode *pn = app->carrinho->topo;
            while (pn) {
                ItemVenda *iv = (ItemVenda *)malloc(sizeof(ItemVenda));
                iv->codigo    = pn->produto.codigo;
                strncpy(iv->nome, pn->produto.nome, MAX_NOME - 1);
                iv->nome[MAX_NOME - 1] = '\0';
                iv->preco     = pn->produto.preco;
                iv->prox      = v.itens;
                v.itens       = iv;
                pn = pn->prox;
            }

            bst_inserir(app->historico, v);
            pilha_limpar(app->carrinho);
            app->tem_cliente = false;

            char ok[64];
            snprintf(ok, 64, "Venda #%d finalizada! Total: R$ %.2f",
                     v.id_venda, v.valor_total);
            estado_msg(app, ok, false);
        }
    }

    /* botao cancelar venda */
    Rectangle btn_can = { rx + 10, 326, rw - 20, 36 };
    if (ui_botao(btn_can, "CANCELAR VENDA",
                 COR_PERIGO, COR_PERIGO_H, WHITE, 15)) {
        if (!pilha_vazia(app->carrinho) || app->tem_cliente) {
            /* estorna todo o estoque do carrinho */
            Produto tmp;
            while (!pilha_vazia(app->carrinho)) {
                pilha_desempilhar(app->carrinho, &tmp);
                hash_atualizar_quantidade(app->estoque, tmp.codigo, +1);
            }
            app->tem_cliente = false;
            estado_msg(app, "Venda cancelada. Estoque restaurado.", false);
        }
    }

    ui_status(app->msg_status, app->msg_erro, &app->msg_timer);
}
