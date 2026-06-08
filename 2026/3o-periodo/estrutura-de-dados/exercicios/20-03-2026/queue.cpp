#include <iostream>
#include <string>
using namespace std;

struct No
{
    string ticket;
    No *proximo;

    No(const string &arq) : ticket(arq), proximo(nullptr) {}
};


void adicionarTicket(Fila &ticket, const string &ticket)
{
}

void imprimirProximo(queue &fila)
{
}

void verProximo(const queue &fila)
{
}

void exibirFila(const queue &fila)
{
}

void liberarFila(queue &fila)
{
}

int main()
{
    queue ticket;

    adicionarArquivo(filaDeImpressao, "teste.txt");
    adicionarArquivo(filaDeImpressao, "slide.pptx");
    adicionarArquivo(filaDeImpressao, "documento.docx");

    cout << "\n";
    exibirFila(filaDeImpressao);

    cout << "\n";
    verProximo(filaDeImpressao);

    cout << "\n";
    imprimirProximo(filaDeImpressao);
    imprimirProximo(filaDeImpressao);

    cout << "\n";
    exibirFila(filaDeImpressao);

    liberarFila(filaDeImpressao);

    return 0;
}