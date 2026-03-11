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

void adicionarInicioDaListaSimples(ListaSimples *ListaSimples, int valor)
{
    No *no = _criandoNo(valor);

    no->proximo = ListaSimples->inicio;
    ListaSimples->inicio = no;
}

void imprimirListaSimples(ListaSimples *ListaSimples)
{
    No *no = ListaSimples->inicio;

    while (no != NULL)
    {
        cout << no->valor << " ";
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