#ifndef LISTA_SIMPLES_H
#define LISTA_SIMPLES_H

#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct No
{
    int valor;
    No *proximo;
};

struct ListaSimples
{
    No *inicio;
};

ListaSimples *criaListaSimples();
void adicionarInicioDaListaSimples(ListaSimples *lista, int valor);
void imprimirListaSimples(ListaSimples *lista);

void removerUltimoDaLista(ListaSimples *lista);
void adicionarFinalDaLista(ListaSimples *lista, int valor);

void inserirOrdenado(ListaSimples *lista, int valor);
int contarNos(ListaSimples *lista);
No *buscarNo(ListaSimples *lista, int valor);
void inverterLista(ListaSimples *lista);
void liberarLista(ListaSimples *lista);

#endif