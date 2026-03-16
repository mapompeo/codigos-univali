#ifndef APP_H
#define APP_H

struct No
{
    int data;
    No *next;
    No *prev;
};

struct ListaControle
{
    No *cabeca;
    No *cauda;
};

ListaControle *iniciar_lista();
void inserir_fim(ListaControle *lista, int data);
void inserir_inicio(ListaControle *lista, int data);
void limpar_lista(ListaControle *lista);
void exibir_direto(ListaControle *lista);
void exibir_inverso(ListaControle *lista);

#endif