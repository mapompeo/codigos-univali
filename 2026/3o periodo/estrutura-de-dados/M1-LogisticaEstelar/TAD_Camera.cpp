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

void removerCamera(ListaCameras *lista, int id)
{
    if (lista->head == NULL)
    {
        return;
    }

    NoCamera *atual = lista->head;

    if (lista->head->prox == lista->head)
    { // caso só tenha 1 elemento
        lista->tamanho--;
        delete atual;
        lista->head = NULL;
        return;
    }

    NoCamera *anterior = NULL;

    while (atual->prox != lista->head && atual->prox != NULL)
    {
        anterior = atual;
        atual = atual->prox;
    }

    anterior->prox = lista->head;
    delete atual;

    lista->tamanho--;
}
