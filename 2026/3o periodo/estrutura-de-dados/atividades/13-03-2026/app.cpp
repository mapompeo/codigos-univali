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
}

void limpar_lista(ListaControle *lista)
{
    No *cabeca = lista->cabeca;
    No *atual = lista->cabeca;

    while (true)
    {
        delete atual;

        atual = cabeca->next;
        cabeca = cabeca->next;

        if (atual == nullptr)
        {
            return;
        }
    }
}

int main()
{
    ListaControle *lista = iniciar_lista();

    inserir_inicio(lista, 4);
    inserir_inicio(lista, 1);
    inserir_inicio(lista, 9);
    inserir_fim(lista, 3);
    inserir_fim(lista, 7);

    cout << "inserir_inicio: 4, 1, 9  |  inserir_fim: 3, 7" << endl;
    cout << endl;

    cout << "exibir_direto: ";
    exibir_direto(lista);
    cout << endl;

    cout << "exibir_inverso: ";
    exibir_inverso(lista);
    cout << endl;

    cout << "limpar_lista: memoria liberada" << endl;
    limpar_lista(lista);

    return 0;
}