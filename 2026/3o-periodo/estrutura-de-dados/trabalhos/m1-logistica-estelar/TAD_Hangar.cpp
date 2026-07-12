#include "TAD_Hangar.h"
#include <iostream>

using namespace std;

void enqueue(Fila *fila, string nome)
{
    NoNaveHangar *novaFila = new NoNaveHangar();
    novaFila->nome = nome;
    novaFila->prox = NULL;

    if (fila->frente == NULL)
    {
        fila->frente = novaFila;
        fila->fim = novaFila;
        return;
    }

    fila->fim->prox = novaFila;
    fila->fim = novaFila;
}

string dequeue(Fila *fila)
{
    if (fila->frente == NULL)
    {
        return "";
    }

    NoNaveHangar *temp = fila->frente;
    string nome = temp->nome;
    fila->frente = fila->frente->prox;
    delete temp;
    return nome;
}
