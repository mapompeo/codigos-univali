#ifndef TAD_LOG_H
#define TAD_LOG_H

#include <string>

using namespace std;

struct NoLog
{
    string mensagem;
    NoLog *prox;
};

struct Pilha
{
    NoLog *topo;
};

void push(Pilha *pilha, string mensagem);
std::string pop(Pilha *pilha);

#endif
