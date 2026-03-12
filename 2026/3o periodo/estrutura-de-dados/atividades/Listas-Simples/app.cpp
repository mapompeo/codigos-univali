#include <iostream>
#include "lista_simples.h"

using namespace std;

int main()
{
    ListaSimples *lista = criaListaSimples();

    // Etapa 1: inserirOrdenado com 50, 10, 30 → deve imprimir 10 -> 30 -> 50
    cout << "--- Etapa 1: inserirOrdenado ---" << endl;
    inserirOrdenado(lista, 50);
    inserirOrdenado(lista, 10);
    inserirOrdenado(lista, 30);
    cout << "Lista: ";
    imprimirListaSimples(lista);

    // Etapa 2: contarNos
    cout << "\n--- Etapa 2: contarNos ---" << endl;
    cout << "Tamanho da lista: " << contarNos(lista) << endl;

    // Etapa 3: inverterLista → deve imprimir 50 -> 30 -> 10
    cout << "\n--- Etapa 3: inverterLista ---" << endl;
    inverterLista(lista);
    cout << "Lista invertida: ";
    imprimirListaSimples(lista);

    // Etapa 4: liberarLista
    cout << "\n--- Etapa 4: liberarLista ---" << endl;
    liberarLista(lista);
    cout << "Lista liberada com sucesso." << endl;

    return 0;
}
