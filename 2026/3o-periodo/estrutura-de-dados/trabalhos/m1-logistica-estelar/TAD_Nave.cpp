#include "TAD_Nave.h"
#include <iostream>

using namespace std;

void inserirNave(ListaNaves *lista, string nome)
{
    NoNave *novaNave = new NoNave{nome, nullptr, nullptr};

    if (lista->head == nullptr)
    {
        lista->head = novaNave;
        lista->tail = novaNave;
    }
    else
    {
        novaNave->ant = lista->tail;
        novaNave->prox = nullptr;
        lista->tail->prox = novaNave;
        lista->tail = novaNave;
    }
}

void removerNave(ListaNaves *lista, string nome)
{
    NoNave *atual = lista->head;
    while (atual != nullptr)
    {
        if (atual->nome == nome)
        {
            // unico elemento
            if (atual->ant == nullptr && atual->prox == nullptr)
            {
                lista->head = nullptr;
                lista->tail = nullptr;
            }
            // primeiro elemento
            else if (atual->ant == nullptr)
            {
                lista->head = atual->prox;
                if (lista->head)
                    lista->head->ant = nullptr;
            }
            // ultimo elemento
            else if (atual->prox == nullptr)
            {
                lista->tail = atual->ant;
                if (lista->tail)
                    lista->tail->prox = nullptr;
            }
            // elemento do meio
            else
            {
                atual->ant->prox = atual->prox;
                atual->prox->ant = atual->ant;
            }
            delete atual;
            break;
        }
        atual = atual->prox;
    }
}
