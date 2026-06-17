#include "ui.h"
#include <string.h>
#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  Campo de Texto                                                      */
/* ------------------------------------------------------------------ */

void campo_init(CampoTexto *c, Rectangle bounds, const char *ph, bool so_numeros) {
    memset(c->text, 0, sizeof(c->text));
    c->len        = 0;
    c->ativo      = false;
    c->bounds     = bounds;
    c->so_numeros = so_numeros;
    strncpy(c->placeholder, ph, 63);
    c->placeholder[63] = '\0';
}

void campo_atualizar(CampoTexto *c) {
    /* ativa / desativa ao clicar */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = GetMousePosition();
        c->ativo  = CheckCollisionPointRec(m, c->bounds);
    }

    if (!c->ativo) return;

    /* backspace */
    if (IsKeyPressed(KEY_BACKSPACE) && c->len > 0) {
        c->text[--c->len] = '\0';
        return;
    }

    /* digitar caracteres */
    int ch;
    while ((ch = GetCharPressed()) != 0) {
        if (c->len >= 255) break;
        if (c->so_numeros && (ch < '0' || ch > '9')) continue;
        c->text[c->len++] = (char)ch;
        c->text[c->len]   = '\0';
    }
}

void campo_desenhar(CampoTexto *c) {
    Color borda = c->ativo ? COR_BORDA_ATIVO : COR_BORDA;
    Color fundo = c->ativo ? COR_CARD : CLITERAL(Color){ 250, 250, 250, 255 };

    DrawRectangleRec(c->bounds, fundo);
    DrawRectangleLinesEx(c->bounds, c->ativo ? 2.0f : 1.0f, borda);

    const char *exibir = (c->len == 0 && !c->ativo) ? c->placeholder : c->text;
    Color cor_txt      = (c->len == 0 && !c->ativo) ? COR_SUBTEXTO : COR_TEXTO;

    /* texto com margem interna de 8px */
    DrawText(exibir,
             (int)c->bounds.x + 8,
             (int)c->bounds.y + (int)(c->bounds.height / 2) - 9,
             18, cor_txt);

    /* cursor piscante */
    if (c->ativo && ((int)(GetTime() * 2) % 2 == 0)) {
        int cx = (int)c->bounds.x + 8 + MeasureText(c->text, 18);
        int cy = (int)c->bounds.y + (int)(c->bounds.height / 2) - 9;
        DrawRectangle(cx, cy, 2, 18, COR_TEXTO);
    }
}

void campo_limpar(CampoTexto *c) {
    memset(c->text, 0, sizeof(c->text));
    c->len   = 0;
    c->ativo = false;
}

/* ------------------------------------------------------------------ */
/*  Botao                                                               */
/* ------------------------------------------------------------------ */

bool ui_botao(Rectangle r, const char *txt, Color cor, Color cor_hover, Color cor_txt, int fs) {
    Vector2 m     = GetMousePosition();
    bool    hover = CheckCollisionPointRec(m, r);
    bool    click = hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawRectangleRec(r, hover ? cor_hover : cor);
    DrawRectangleLinesEx(r, 1.0f, hover ? cor_hover : cor);

    /* sombra sutil */
    if (!hover)
        DrawRectangle((int)r.x + 2, (int)r.y + (int)r.height,
                      (int)r.width, 3,
                      CLITERAL(Color){ 0, 0, 0, 30 });

    int tw = MeasureText(txt, fs);
    int tx = (int)r.x + ((int)r.width  - tw) / 2;
    int ty = (int)r.y + ((int)r.height - fs) / 2;
    DrawText(txt, tx, ty, fs, cor_txt);

    return click;
}

/* ------------------------------------------------------------------ */
/*  Card                                                                */
/* ------------------------------------------------------------------ */

void ui_card(Rectangle r, Color cor) {
    /* sombra */
    DrawRectangle((int)r.x + 3, (int)r.y + 3, (int)r.width, (int)r.height,
                  CLITERAL(Color){ 0, 0, 0, 25 });
    DrawRectangleRec(r, cor);
    DrawRectangleLinesEx(r, 1.0f, COR_BORDA);
}

/* ------------------------------------------------------------------ */
/*  Header                                                              */
/* ------------------------------------------------------------------ */

void ui_header(const char *titulo, Color cor) {
    DrawRectangle(0, 0, GetScreenWidth(), 60, cor);
    /* linha de realce inferior */
    DrawRectangle(0, 57, GetScreenWidth(), 3,
                  CLITERAL(Color){ 0, 0, 0, 60 });
    int fs = 22;
    int tw = MeasureText(titulo, fs);
    DrawText(titulo,
             GetScreenWidth() / 2 - tw / 2,
             (60 - fs) / 2,
             fs, WHITE);
}

/* ------------------------------------------------------------------ */
/*  Botao Voltar                                                        */
/* ------------------------------------------------------------------ */

bool ui_botao_voltar(void) {
    Rectangle r = { 10, 10, 110, 38 };
    return ui_botao(r, "< VOLTAR", COR_PRIMARIA, COR_HOVER, WHITE, 16);
}

/* ------------------------------------------------------------------ */
/*  Mensagem de status                                                  */
/* ------------------------------------------------------------------ */

void ui_status(const char *msg, bool erro, float *timer) {
    if (!msg || *timer <= 0.0f) return;
    *timer -= GetFrameTime();

    Color bg  = erro ? COR_ERRO_BG   : COR_SUCESSO_BG;
    Color txt = erro ? COR_PERIGO    : COR_PRIMARIA;

    int w = GetScreenWidth();
    DrawRectangle(0, GetScreenHeight() - 44, w, 44, bg);
    DrawRectangle(0, GetScreenHeight() - 44, 4, 44, txt);
    int tw = MeasureText(msg, 18);
    DrawText(msg, w / 2 - tw / 2, GetScreenHeight() - 30, 18, txt);
}

/* ------------------------------------------------------------------ */
/*  Texto centrado                                                      */
/* ------------------------------------------------------------------ */

void ui_texto_centrado(Rectangle r, const char *txt, int fs, Color cor) {
    int tw = MeasureText(txt, fs);
    int tx = (int)r.x + ((int)r.width  - tw) / 2;
    int ty = (int)r.y + ((int)r.height - fs) / 2;
    DrawText(txt, tx, ty, fs, cor);
}

/* ------------------------------------------------------------------ */
/*  Scroll                                                              */
/* ------------------------------------------------------------------ */

void ui_scroll_ajustar(float *offset, float conteudo_h, float area_h) {
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        *offset -= wheel * 30.0f;
    }
    if (*offset < 0.0f)          *offset = 0.0f;
    if (conteudo_h <= area_h)    { *offset = 0.0f; return; }
    if (*offset > conteudo_h - area_h) *offset = conteudo_h - area_h;
}
