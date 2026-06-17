#ifndef UI_H
#define UI_H

#include <raylib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Paleta de cores do sistema                                          */
/* ------------------------------------------------------------------ */
#define COR_FUNDO       CLITERAL(Color){ 235, 240, 235, 255 }
#define COR_PRIMARIA    CLITERAL(Color){  27,  94,  32, 255 }  /* verde escuro  */
#define COR_ACENTO      CLITERAL(Color){  56, 142,  60, 255 }  /* verde medio   */
#define COR_HOVER       CLITERAL(Color){  46, 125,  50, 255 }  /* hover botao   */
#define COR_PERIGO      CLITERAL(Color){ 183,  28,  28, 255 }  /* vermelho      */
#define COR_PERIGO_H    CLITERAL(Color){ 211,  47,  47, 255 }
#define COR_AVISO       CLITERAL(Color){ 230,  81,   0, 255 }  /* laranja       */
#define COR_AVISO_H     CLITERAL(Color){ 245, 124,   0, 255 }
#define COR_INFO        CLITERAL(Color){  13,  71, 161, 255 }  /* azul          */
#define COR_INFO_H      CLITERAL(Color){  25, 118, 210, 255 }
#define COR_ROXO        CLITERAL(Color){  74,  20, 140, 255 }  /* roxo          */
#define COR_ROXO_H      CLITERAL(Color){ 123,  31, 162, 255 }
#define COR_CARD        CLITERAL(Color){ 255, 255, 255, 255 }
#define COR_CARD2       CLITERAL(Color){ 245, 250, 245, 255 }
#define COR_TEXTO       CLITERAL(Color){  33,  33,  33, 255 }
#define COR_SUBTEXTO    CLITERAL(Color){  97,  97,  97, 255 }
#define COR_BORDA       CLITERAL(Color){ 189, 189, 189, 255 }
#define COR_BORDA_ATIVO CLITERAL(Color){  56, 142,  60, 255 }
#define COR_SUCESSO_BG  CLITERAL(Color){ 200, 230, 201, 255 }
#define COR_ERRO_BG     CLITERAL(Color){ 255, 205, 210, 255 }

/* ------------------------------------------------------------------ */
/*  Campo de texto simulado                                             */
/* ------------------------------------------------------------------ */
typedef struct {
    char  text[256];
    int   len;
    bool  ativo;
    Rectangle bounds;
    char  placeholder[64];
    bool  so_numeros;
} CampoTexto;

void  campo_init(CampoTexto *c, Rectangle bounds, const char *ph, bool so_numeros);
void  campo_atualizar(CampoTexto *c);
void  campo_desenhar(CampoTexto *c);
void  campo_limpar(CampoTexto *c);

/* ------------------------------------------------------------------ */
/*  Botao                                                               */
/* ------------------------------------------------------------------ */
bool  ui_botao(Rectangle r, const char *txt, Color cor, Color cor_hover, Color cor_txt, int fs);

/* ------------------------------------------------------------------ */
/*  Card (fundo branco com borda)                                       */
/* ------------------------------------------------------------------ */
void  ui_card(Rectangle r, Color cor);

/* ------------------------------------------------------------------ */
/*  Barra de cabecalho                                                  */
/* ------------------------------------------------------------------ */
void  ui_header(const char *titulo, Color cor);

/* ------------------------------------------------------------------ */
/*  Botao Voltar (topo-esquerda)                                        */
/* ------------------------------------------------------------------ */
bool  ui_botao_voltar(void);

/* ------------------------------------------------------------------ */
/*  Mensagem de status (banner)                                         */
/* ------------------------------------------------------------------ */
void  ui_status(const char *msg, bool erro, float *timer);

/* ------------------------------------------------------------------ */
/*  Texto centralizado numa Rectangle                                   */
/* ------------------------------------------------------------------ */
void  ui_texto_centrado(Rectangle r, const char *txt, int fs, Color cor);

/* ------------------------------------------------------------------ */
/*  Scroll                                                              */
/* ------------------------------------------------------------------ */
void  ui_scroll_ajustar(float *offset, float conteudo_h, float area_h);

#endif /* UI_H */
