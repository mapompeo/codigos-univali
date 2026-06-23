#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "estado.h"
#include "estruturas/headers/dados.h"
#include "interface/headers/ui.h"
#include "interface/headers/tela_menu.h"
#include "interface/headers/tela_cadastro.h"
#include "interface/headers/tela_caixa.h"
#include "interface/headers/tela_relatorios.h"
#include "interface/headers/tela_auditoria.h"

#define JANELA_W  1280
#define JANELA_H   720
#define FPS        60

int main(void) {
    /* ---- inicializacao da janela ---- */
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(JANELA_W, JANELA_H, "Mercadinho - Sistema de Gerenciamento | ED");
    SetTargetFPS(FPS);

    /* ---- estado global ---- */
    EstadoApp app;
    memset(&app, 0, sizeof(EstadoApp));
    app.largura     = JANELA_W;
    app.altura      = JANELA_H;
    app.tela_atual  = TELA_MENU;
    app.tem_cliente = false;

    /* ---- alocacao das estruturas de dados ---- */
    app.estoque       = hash_criar(HASH_TAMANHO);
    app.fila_clientes = fila_criar();
    app.carrinho      = pilha_criar();
    app.historico     = bst_criar();

    /* ---- carregamento / geracao dos arquivos de dados ---- */
    const char *arq_produtos  = "data/produtos.txt";
    const char *arq_clientes  = "data/clientes.txt";

    /* se os arquivos nao existirem, gera automaticamente */
    FILE *fp = fopen(arq_produtos, "r");
    if (!fp) {
        printf("[INFO] produtos.txt nao encontrado. Gerando 1000 registros...\n");
        gerar_produtos_txt(arq_produtos);
    } else {
        fclose(fp);
    }

    FILE *fc = fopen(arq_clientes, "r");
    if (!fc) {
        printf("[INFO] clientes.txt nao encontrado. Gerando 1000 registros...\n");
        gerar_clientes_txt(arq_clientes);
    } else {
        fclose(fc);
    }

    int np = carregar_produtos(arq_produtos, app.estoque);
    int nc = carregar_clientes(arq_clientes, app.fila_clientes);
    printf("[INFO] %d produtos carregados na Hash.\n", np);
    printf("[INFO] %d clientes carregados na Fila.\n", nc);

    /* ---- estados de cada tela ---- */
    TelaCadastroState   st_cad;
    TelaCaixaState      st_caixa;
    TelaRelatoriosState st_rel;
    TelaAuditoriaState  st_aud;

    tela_cadastro_init(&st_cad);
    tela_caixa_init(&st_caixa);
    tela_relatorios_init(&st_rel);
    tela_auditoria_init(&st_aud);

    /* ---- loop principal ---- */
    while (!WindowShouldClose()) {
        /* atualiza dimensoes se a janela foi redimensionada */
        app.largura = GetScreenWidth();
        app.altura  = GetScreenHeight();

        BeginDrawing();

        switch (app.tela_atual) {
            case TELA_MENU:
                tela_menu_desenhar(&app);
                break;
            case TELA_CADASTRO:
                tela_cadastro_desenhar(&app, &st_cad);
                break;
            case TELA_CAIXA:
                tela_caixa_desenhar(&app, &st_caixa);
                break;
            case TELA_RELATORIOS:
                tela_relatorios_desenhar(&app, &st_rel);
                break;
            case TELA_AUDITORIA:
                tela_auditoria_desenhar(&app, &st_aud);
                break;
        }

        EndDrawing();
    }

    /* ---- liberacao de memoria ---- */
    tela_relatorios_limpar(&st_rel);

    hash_destruir(app.estoque);
    fila_destruir(app.fila_clientes);
    pilha_destruir(app.carrinho);
    bst_destruir(app.historico);

    CloseWindow();
    return 0;
}
