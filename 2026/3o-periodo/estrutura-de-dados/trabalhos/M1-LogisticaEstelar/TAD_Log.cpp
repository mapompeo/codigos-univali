#include "TAD_Log.h"
#include <iostream>

using namespace std;

void push(Pilha *pilha, string mensagem)
{
    NoLog *novoLog = new NoLog();
    novoLog->mensagem = mensagem;
    novoLog->prox = pilha->topo;
    pilha->topo = novoLog;
}

string pop(Pilha *pilha)
{
    if (pilha->topo == NULL)
    {
        return "";
    }

    NoLog *temp = pilha->topo;
    string mensagem = temp->mensagem;
    pilha->topo = pilha->topo->prox;
    delete temp;
    return mensagem;
}
