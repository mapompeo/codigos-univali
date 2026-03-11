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
    
    cout << "\n--- inserirOrdenado ---" << endl;
    inserirOrdenado(lista, 50);
    inserirOrdenado(lista, 10);
    inserirOrdenado(lista, 30);
    cout << "Lista apos inserir 50, 10, 30 (esperado: 10 -> 30 -> 50): ";
    imprimirListaSimples(lista);

    cout << "\n--- contarNos ---" << endl;
    int total = contarNos(lista);
    cout << "Total de nos na lista: " << total << endl;

    cout << "\n--- inverterLista ---" << endl;
    cout << "Lista antes de inverter: ";
    imprimirListaSimples(lista);
    inverterLista(lista);
    cout << "Lista apos inverter (esperado: 50 -> 30 -> 10): ";
    imprimirListaSimples(lista);

    cout << "\n--- liberarLista ---" << endl;
    liberarLista(lista);
    cout << "Lista liberada com sucesso." << endl;

    return 0;
}