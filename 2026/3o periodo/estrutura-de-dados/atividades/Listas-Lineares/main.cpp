#include <iostream>
using namespace std;

struct ListaEstatica
{
    int inteiros[10] = {}, quantidade;
};

void inverter(int *vet, int tam)
{
    cout << "--- Questão 2 ---" << endl;
    int *inicio = vet, *fim = vet + (tam - 1), temp;

    while (inicio < fim)
    {
        temp = *inicio;
        *inicio = *fim;
        *fim = temp;
        inicio++, fim--;
    }
}

void inicializar(ListaEstatica *l)
{
    l->quantidade = 0;
}

bool inserir(ListaEstatica *l, int valor)
{
    if (l->quantidade < 10)
    {
        l->inteiros[l->quantidade] = valor;
        l->quantidade++;
        return false;
    }
}

int buscar(ListaEstatica *l, int valor)
{
}

bool remover(ListaEstatica *l, int valor)
{
}

int main()
{
    int vetor[5] = {};
    int *ponteiro = vetor;

    cout << "--- Questão 1 ---" << endl;

    for (int i = 0; i < 5; i++)
    {
        cout << "Insira um valor dentro da lista: ";
        cin >> *(ponteiro + i);
        cout << "Endereço: " << (ponteiro + i) << ", Valor: " << *(ponteiro + i) << endl;
    }

    inverter(vetor, 5);
    cout << "Vetor invertido: ";
    for (int i = 0; i < 5; i++)
    {
        cout << *(ponteiro + i) << " ";
    }
    cout << endl;

    return 0;
}
