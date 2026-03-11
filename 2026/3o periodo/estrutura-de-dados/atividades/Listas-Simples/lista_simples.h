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
void imprimirListaSimples(ListaSimples *lista);

void inserirOrdenado(ListaSimples *lista, int valor);
int contarNos(ListaSimples *lista);
No *buscarNo(ListaSimples *lista, int valor);
void inverterLista(ListaSimples *lista);
void liberarLista(ListaSimples *lista, int valor);

#endif