#ifndef LISTA_SIMPLES_H
#define LISTA_SIMPLES_H

#include <stdio.h>
#include <stdlib.h>

typedef struct No
{
    int valor;
    struct No *proximo;
} No;

typedef struct ListaSimples
{
    No *inicio;
} ListaSimples;

ListaSimples *criaListaSimples();
void adicionarInicioDaListaSimples(ListaSimples *ListaSimples, int valor);
void imprimirListaSimples(ListaSimples *ListaSimples);

void removerUltimoDaLista(ListaSimples *ListaSimples);
void adicionarFinalDaLista(ListaSimples *ListaSimples, int valor);

void inserirOrdenado(ListaSimples *lista, int valor);
int contarNos(ListaSimples *lista);
No *buscarNo(ListaSimples *lista, int valor);
void inverterLista(ListaSimples *lista);
void liberarLista(ListaSimples *lista);

#endif