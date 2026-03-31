#ifndef TAD_HANGAR_H
#define TAD_HANGAR_H

#include <string>

using namespace std;

struct NoNaveHangar
{
    string nome;
    NoNaveHangar *prox;
};

struct Fila
{
    NoNaveHangar *frente;
    NoNaveHangar *fim;
};

void enqueue(Fila *fila, string nome);
string dequeue(Fila *fila);

#endif
