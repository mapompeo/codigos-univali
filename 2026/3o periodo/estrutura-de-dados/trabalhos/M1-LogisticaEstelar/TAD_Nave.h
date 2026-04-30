#ifndef TAD_NAVE_H
#define TAD_NAVE_H

#include <string>

using namespace std;

struct NoNave
{
    string nome;
    NoNave *ant;
    NoNave *prox;
};

struct ListaNaves
{
    NoNave *head;
    NoNave *tail;
};

void inserirNave(ListaNaves *lista, string nome);
void removerNave(ListaNaves *lista, string nome);

#endif
