#include <iostream>
#include "lista_simples.h"

using namespace std;

int main()
{
    cout << "Criando lista simples" << endl;
    ListaSimples *lista = criaListaSimples();

    cout << "\n--- adicionarInicioDaListaSimples ---" << endl;
    adicionarInicioDaListaSimples(lista, 30);
    adicionarInicioDaListaSimples(lista, 0);
    adicionarInicioDaListaSimples(lista, 89);
    cout << "Lista apos adicionar 30, 0, 89 no inicio: ";
    imprimirListaSimples(lista);

    cout << "\n--- adicionarFinalDaLista ---" << endl;
    adicionarFinalDaLista(lista, 10);
    adicionarFinalDaLista(lista, 20);
    cout << "Lista apos adicionar 10, 20 no final: ";
    imprimirListaSimples(lista);

    cout << "\n--- removerUltimoDaLista ---" << endl;
    removerUltimoDaLista(lista);
    cout << "Lista apos remover o ultimo elemento: ";
    imprimirListaSimples(lista);

    return 0;
}