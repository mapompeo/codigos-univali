#include "TAD_Carga.h"
#include <iostream>

using namespace std;

void inserirCarga(ListaCargas *lista, string nome, float peso)
{
    lista->head = new NoCarga{nome, peso, lista->head};
}

void removerCarga(ListaCargas *lista, string nome)
{
    NoCarga *atual = lista->head;
    NoCarga *anterior = nullptr;

    while (atual != nullptr)
    {
        if (atual->nome == nome)
        {
            if (anterior == nullptr)
            {
                // remove do inicio
                lista->head = atual->prox;
            }
            else
            {
                // remove do meio ou fim
                anterior->prox = atual->prox;
            }
            delete atual;
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }
}