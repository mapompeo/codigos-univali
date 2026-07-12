#include <iostream>
using namespace std;

struct ListaEstatica
{
    int inteiros[10] = {}, quantidade;
};

struct ListaDinamica
{
    int *dados;
    int capacidade, quantidade;
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
        return true;
    }
    return false;
}

int buscar(ListaEstatica *l, int valor)
{
    for (int i = 0; i < l->quantidade; i++)
    {
        if (l->inteiros[i] == valor)
            return i;
    }
    return -1;
}

bool remover(ListaEstatica *l, int valor)
{
    int indice = buscar(l, valor);
    if (indice == -1)
        return false;

    for (int i = indice; i < l->quantidade - 1; i++)
        l->inteiros[i] = l->inteiros[i + 1];

    l->quantidade--;
    return true;
}

void inicializarDinamica(ListaDinamica *l, int capacidadeInicial)
{
    l->capacidade = capacidadeInicial;
    l->quantidade = 0;
    l->dados = new int[l->capacidade];
}

bool inserirDinamica(ListaDinamica *l, int valor)
{
    if (l->quantidade == l->capacidade)
    {
        l->capacidade *= 2;
        int *novo = new int[l->capacidade];
        for (int i = 0; i < l->quantidade; i++)
            novo[i] = l->dados[i];
        delete[] l->dados;
        l->dados = novo;
    }
    l->dados[l->quantidade] = valor;
    l->quantidade++;
    return true;
}

void atualizar(ListaDinamica *l, int indice, int novoValor)
{
    if (indice < 0 || indice >= l->quantidade)
    {
        cout << "Índice inválido!" << endl;
        return;
    }
    l->dados[indice] = novoValor;
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

    cout << "--- Questão 3 ---" << endl;
    ListaEstatica lista;
    inicializar(&lista);

    inserir(&lista, 10);
    inserir(&lista, 20);
    inserir(&lista, 30);
    inserir(&lista, 40);

    cout << "Lista após inserções: ";
    for (int i = 0; i < lista.quantidade; i++)
        cout << lista.inteiros[i] << " ";
    cout << endl;

    int indice = buscar(&lista, 20);
    cout << "Busca pelo valor 20: índice " << indice << endl;

    remover(&lista, 20);
    cout << "Lista após remover 20: ";
    for (int i = 0; i < lista.quantidade; i++)
        cout << lista.inteiros[i] << " ";
    cout << endl;

    cout << "--- Questão 4 ---" << endl;
    ListaDinamica listaDin;
    inicializarDinamica(&listaDin, 2);

    inserirDinamica(&listaDin, 100);
    inserirDinamica(&listaDin, 200);
    inserirDinamica(&listaDin, 300);
    inserirDinamica(&listaDin, 400);

    cout << "Lista dinâmica: ";
    for (int i = 0; i < listaDin.quantidade; i++)
        cout << listaDin.dados[i] << " ";
    cout << endl;
    cout << "Capacidade atual: " << listaDin.capacidade << endl;

    atualizar(&listaDin, 1, 999);
    cout << "Após atualizar índice 1 para 999: ";
    for (int i = 0; i < listaDin.quantidade; i++)
        cout << listaDin.dados[i] << " ";
    cout << endl;

    atualizar(&listaDin, 10, 0);

    delete[] listaDin.dados;

    return 0;
}