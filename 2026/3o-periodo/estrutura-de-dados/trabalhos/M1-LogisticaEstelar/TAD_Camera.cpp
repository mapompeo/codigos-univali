#include "TAD_Camera.h"
#include <iostream>

using namespace std;

// eu poderia ter feito head e tail, mas nao sabia se podia, então fiz ela normal mesmo

void inserirCamera(ListaCameras *lista, int id, string local)
{
    NoCamera *novaCamera = new NoCamera();
    novaCamera->id = id;
    novaCamera->local = local;

    lista->tamanho++;

    if (lista->head == NULL)
    {
        lista->head = novaCamera;
        novaCamera->prox = lista->head;
        return;
    }

    NoCamera *atual = lista->head;
    while (atual->prox != lista->head)
    {
        atual = atual->prox;
    }

    atual->prox = novaCamera;
    novaCamera->prox = lista->head;
}

void removerCamera(ListaCameras *lista, string local)
{
    if (lista->head == NULL)
    {
        return;
    }

    NoCamera *atual = lista->head;
    NoCamera *anterior = NULL;

    do
    {
        if (atual->local == local)
        {
            break;
        }
        anterior = atual;
        atual = atual->prox;
    } while (atual != lista->head);

    if (atual->local != local)
        return;

    if (atual->prox == lista->head && atual == lista->head)
    {
        lista->head = NULL;
    }
    else
    {
        if (atual == lista->head)
        {
            NoCamera *ultimo = lista->head;

            while (ultimo->prox != lista->head)
            {
                ultimo = ultimo->prox;
            }

            lista->head = atual->prox;
            ultimo->prox = lista->head;
        }
        else
        {
            anterior->prox = atual->prox;
        }
    }

    delete atual;
    lista->tamanho--;
}
