#include <iostream>
#include <cstdlib>
#include "app.h"
using namespace std;

ListaControle *iniciar_lista()
{
    ListaControle *lista = (ListaControle *)calloc(1, sizeof(ListaControle));
    lista->cabeca = nullptr;
    lista->cauda = nullptr;
    return lista;
}

void inserir_inicio(ListaControle *lista, int data)
{
    No *novo = (No *)calloc(1, sizeof(No));
    novo->data = data;
    novo->prev = nullptr;
    novo->next = lista->cabeca;

    if (lista->cabeca != nullptr)
        lista->cabeca->prev = novo;
    else
        lista->cauda = novo;

    lista->cabeca = novo;
}

void inserir_fim(ListaControle *lista, int data)
{
    No *novo = (No *)calloc(1, sizeof(No));
    novo->data = data;
    novo->next = nullptr;
    novo->prev = lista->cauda;

    if (lista->cauda != nullptr)
        lista->cauda->next = novo;
    else
        lista->cabeca = novo;

    lista->cauda = novo;
}

void exibir_direto(ListaControle *lista)
{
    No *no = lista->cabeca;
    while (no != nullptr)
    {
        cout << no->data;
        if (no->next != nullptr)
            cout << " -> ";
        no = no->next;
    }
    cout << endl;
}

void exibir_inverso(ListaControle *lista)
{
    No *no = lista->cauda;
    while (no != nullptr)
    {
        cout << no->data;
        if (no->prev != nullptr)
            cout << " -> ";
        no = no->prev;
    }
    cout << endl;
}

void limpar_lista(ListaControle *lista)
{
    No *atual = lista->cabeca;
    while (atual != nullptr)
    {
        No *proximo = atual->next;
        delete atual;
        atual = proximo;
    }
    lista->cabeca = nullptr;
    lista->cauda = nullptr;
}

int main()
{
    ListaControle *lista = iniciar_lista();

    inserir_fim(lista, 10);
    inserir_fim(lista, 20);
    cout << "depois de inserir_fim(10) e inserir_fim(20): ";
    exibir_direto(lista);

    inserir_inicio(lista, 5);
    inserir_inicio(lista, 2);

    cout << "\ndireto  (esperado 2 5 10 20): ";
    exibir_direto(lista);
    cout << "inverso (esperado 20 10 5 2): ";
    exibir_inverso(lista);

    limpar_lista(lista);
    cout << "\ncabeca apos limpar: " << lista->cabeca << " (deve ser 0)" << endl;

    cout << "\nlista vazia - direto:  ";
    exibir_direto(lista);
    cout << "lista vazia - inverso: ";
    exibir_inverso(lista);

    cout << "\nsem crash = passou!" << endl;

    delete lista;
    return 0;
}