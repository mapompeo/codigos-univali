#ifndef TAD_CARGA_H
#define TAD_CARGA_H

#include <string>

using namespace std;

struct NoCarga
{
    string nome;
    float peso;
    NoCarga *prox;
};

struct ListaCargas
{
    NoCarga *head;
};

void inserirCarga(ListaCargas *lista, string nome, float peso);
void removerCarga(ListaCargas *lista, string nome);

#endif
