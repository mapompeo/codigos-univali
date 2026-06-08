#ifndef LISTA_SIMPLES_H
#define LISTA_SIMPLES_H

using namespace std;

struct No
{
    int valor;
    No *proximo;
};

struct ListaSimples
{
    No *inicio;
};

ListaSimples *criaListaSimples();
void adicionarInicioDaListaSimples(ListaSimples *ListaSimples, int valor);
void imprimirListaSimples(ListaSimples *ListaSimples);

void removerUltimoDaLista(ListaSimples *ListaSimples);
void adicionarFinalDaLista(ListaSimples *ListaSimples, int valor);

#endif