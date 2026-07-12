#include <iostream>
#include "lista_simples.h"
using namespace std;

No *_criandoNo(int valor)
{
    No *no = (No *)calloc(1, sizeof(No));

    no->valor = valor;
    no->proximo = NULL;
    return no;
}

ListaSimples *criaListaSimples()
{
    ListaSimples *lista = (ListaSimples *)calloc(1, sizeof(ListaSimples));
    lista->inicio = NULL;
    return lista;
}

void adicionarInicioDaListaSimples(ListaSimples *lista, int valor)
{
    No *no = _criandoNo(valor);

    no->proximo = lista->inicio;
    lista->inicio = no;
}

void imprimirListaSimples(ListaSimples *lista)
{
    No *no = lista->inicio;

    while (no != NULL)
    {
        cout << no->valor;
        if (no->proximo != NULL)
            cout << " -> ";
        no = no->proximo;
    }
    cout << endl;
}

void adicionarFinalDaLista(ListaSimples *lista, int valor)
{
    No *novoNo = _criandoNo(valor);

    if (lista->inicio == NULL)
    {
        lista->inicio = novoNo;
        return;
    }

    No *ultimo = lista->inicio;
    while (ultimo->proximo != NULL)
    {
        ultimo = ultimo->proximo;
    }

    ultimo->proximo = novoNo;
}

void removerUltimoDaLista(ListaSimples *lista)
{
    if (lista->inicio == NULL)
    {
        return;
    }

    if (lista->inicio->proximo == NULL)
    {
        free(lista->inicio);
        lista->inicio = NULL;
        return;
    }

    No *temp = lista->inicio;
    while (temp->proximo->proximo != NULL)
    {
        temp = temp->proximo;
    }

    free(temp->proximo);
    temp->proximo = NULL;
}

void inserirOrdenado(ListaSimples *lista, int valor)
{
    No *novoNo = _criandoNo(valor);

    if (lista->inicio == NULL || valor < lista->inicio->valor)
    {
        novoNo->proximo = lista->inicio;
        lista->inicio = novoNo;
        return;
    }

    No *atual = lista->inicio;
    while (atual->proximo != NULL && atual->proximo->valor < valor)
    {
        atual = atual->proximo;
    }

    novoNo->proximo = atual->proximo;
    atual->proximo = novoNo;
}

int contarNos(ListaSimples *lista)
{
    int count = 0;
    No *atual = lista->inicio;
    while (atual != NULL)
    {
        count++;
        atual = atual->proximo;
    }
    return count;
}

No *buscarNo(ListaSimples *lista, int valor)
{
    No *atual = lista->inicio;
    while (atual != NULL)
    {
        if (atual->valor == valor)
            return atual;
        atual = atual->proximo;
    }
    return NULL;
}

void inverterLista(ListaSimples *lista)
{
    No *anterior = NULL;
    No *atual = lista->inicio;
    No *proximo = NULL;

    while (atual != NULL)
    {
        proximo = atual->proximo;
        atual->proximo = anterior;
        anterior = atual;
        atual = proximo;
    }

    lista->inicio = anterior;
}

void liberarLista(ListaSimples *lista)
{
    No *atual = lista->inicio;
    while (atual != NULL)
    {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    free(lista);
}