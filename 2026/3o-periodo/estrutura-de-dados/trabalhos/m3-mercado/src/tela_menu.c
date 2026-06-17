#include "tela_menu.h"
#include "ui.h"
#include <stdio.h>

/* Botao grande com titulo e subtitulo */
static bool botao_grande(Rectangle r, const char *titulo, const char *sub,
                          Color cor, Color cor_h) {
    Vector2 m     = GetMousePosition();
    bool    hover = CheckCollisionPointRec(m, r);
    bool    click = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawRectangleRec(r, hover ? cor_h : cor);
    /* sombra */
    if (!hover)
        DrawRectangle((int)r.x + 3, (int)r.y + (int)r.height,
                      (int)r.width, 4, CLITERAL(Color){ 0, 0, 0, 30 });

    int tw = MeasureText(titulo, 24);
    DrawText(titulo,
             (int)r.x + ((int)r.width - tw) / 2,
             (int)r.y + (int)r.height / 2 - 22,
             24, WHITE);

    int sw = MeasureText(sub, 14);
    DrawText(sub,
             (int)r.x + ((int)r.width - sw) / 2,
             (int)r.y + (int)r.height / 2 + 10,
             14, CLITERAL(Color){ 255, 255, 255, 180 });

    return click;
}

void tela_menu_desenhar(EstadoApp *app) {
    ClearBackground(COR_FUNDO);

    /* header */
    ui_header("MERCADINHO  |  Sistema de Gerenciamento", COR_PRIMARIA);

    int W = app->largura;

    /* --- cards de dashboard --- */
    int card_y = 75;
    int card_h = 90;
    int gap    = 14;
    int margin = 20;
    int card_w = (W - 2 * margin - 2 * gap) / 3;

    /* Card 1 - clientes na fila */
    Rectangle c1 = { (float)margin, (float)card_y, (float)card_w, (float)card_h };
    ui_card(c1, COR_CARD);
    DrawText("Clientes na Fila", (int)c1.x + 12, (int)c1.y + 10, 15, COR_SUBTEXTO);
    char buf[64];
    snprintf(buf, 64, "%d", app->fila_clientes->tamanho);
    int tw = MeasureText(buf, 36);
    DrawText(buf, (int)c1.x + card_w / 2 - tw / 2, (int)c1.y + 34, 36, COR_PRIMARIA);

    /* Card 2 - produtos */
    Rectangle c2 = { (float)(margin + card_w + gap), (float)card_y,
                     (float)card_w, (float)card_h };
    ui_card(c2, COR_CARD);
    DrawText("Produtos no Estoque", (int)c2.x + 12, (int)c2.y + 10, 15, COR_SUBTEXTO);
    snprintf(buf, 64, "%d", app->estoque->total);
    tw = MeasureText(buf, 36);
    DrawText(buf, (int)c2.x + card_w / 2 - tw / 2, (int)c2.y + 34, 36, COR_INFO);

    /* Card 3 - vendas */
    Rectangle c3 = { (float)(margin + 2 * (card_w + gap)), (float)card_y,
                     (float)card_w, (float)card_h };
    ui_card(c3, COR_CARD);
    DrawText("Vendas Realizadas", (int)c3.x + 12, (int)c3.y + 10, 15, COR_SUBTEXTO);
    snprintf(buf, 64, "%d", app->historico->total);
    tw = MeasureText(buf, 36);
    DrawText(buf, (int)c3.x + card_w / 2 - tw / 2, (int)c3.y + 34, 36, COR_AVISO);

    /* --- botoes de navegacao (grade 2x2) --- */
    int btn_y1 = card_y + card_h + 16;
    int btn_h  = (app->altura - btn_y1 - 14) / 2 - 8;
    int btn_w  = (W - 2 * margin - gap) / 2;

    Rectangle b1 = { (float)margin,               (float)btn_y1,
                     (float)btn_w,                 (float)btn_h };
    Rectangle b2 = { (float)(margin + btn_w + gap), (float)btn_y1,
                     (float)btn_w,                 (float)btn_h };
    Rectangle b3 = { (float)margin,               (float)(btn_y1 + btn_h + 8),
                     (float)btn_w,                 (float)btn_h };
    Rectangle b4 = { (float)(margin + btn_w + gap), (float)(btn_y1 + btn_h + 8),
                     (float)btn_w,                 (float)btn_h };

    if (botao_grande(b1, "FRENTE DE CAIXA",
                     "Atender clientes e finalizar vendas",
                     COR_PRIMARIA, COR_HOVER))
        app->tela_atual = TELA_CAIXA;

    if (botao_grande(b2, "CADASTRO DE PRODUTOS",
                     "Inserir novos itens no estoque",
                     COR_INFO, COR_INFO_H))
        app->tela_atual = TELA_CADASTRO;

    if (botao_grande(b3, "RELATORIOS GERENCIAIS",
                     "Ranking de faturamento e historico",
                     COR_AVISO, COR_AVISO_H))
        app->tela_atual = TELA_RELATORIOS;

    if (botao_grande(b4, "AUDITORIA / CONSULTA",
                     "Buscar venda por ID e ver detalhes",
                     COR_ROXO, COR_ROXO_H))
        app->tela_atual = TELA_AUDITORIA;
}
