#include <iostream>
using namespace std;

struct No
{
    int valor;
    No *prev;
    No *next;
};
struct ListaControle
{
    No *cabeca;
};

ListaControle *iniciar_lista()
{
    ListaControle *lista = (ListaControle *)calloc(1, sizeof(ListaControle));
    No *no = (No *)calloc(1, sizeof(No));
    no->prev = nullptr;

    lista->cabeca = no;

    return lista;
}

void inserir_inicio(ListaControle *lista, int data)
{
    No *novo_no = (No *)calloc(1, sizeof(No));
    No *cabeca = lista->cabeca;

    novo_no->valor = data;
    novo_no->next = cabeca;
    novo_no->prev = nullptr;
    
    cabeca->prev = novo_no;
    
}

void exibir_direto(ListaControle *lista)
{
    No *no = lista->cabeca;

    while (no->prev != nullptr)
    {
        cout << no->valor << " -> ";

        no = no->next;
    }
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
    ListaControle *lista;

    inserir_inicio(lista, 4);
    inserir_inicio(lista, 1);
    inserir_inicio(lista, 9);
    inserir_inicio(lista, 3);
    inserir_inicio(lista, 4);

    exibir_direto(lista);
}