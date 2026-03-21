#include <iostream>
#include <string>
using namespace std;

struct No
{
    string arquivo;
    No *proximo;

    No(const string &arq) : arquivo(arq), proximo(nullptr) {}
};

// Estrutura de fila com ponteiros
struct Fila
{
    No *inicio;
    No *fim;

    Fila() : inicio(nullptr), fim(nullptr) {}
};

// Verifica se a fila está vazia
bool estaVazia(const Fila &fila)
{
    return fila.inicio == nullptr;
}

// Obtém o primeiro elemento (sem remover)
string obterPrimeiro(const Fila &fila)
{
    if (estaVazia(fila))
    {
        return "";
    }
    return fila.inicio->arquivo;
}

void adicionarArquivo(Fila &fila, const string &arquivo)
{
    No *novoNo = new No(arquivo);

    if (estaVazia(fila))
    {
        fila.inicio = novoNo;
    }
    else
    {
        fila.fim->proximo = novoNo;
    }
    fila.fim = novoNo;

    cout << "Arquivo: '" << arquivo << "' adicionado à fila." << endl;
}

void imprimirProximo(Fila &fila)
{
    if (estaVazia(fila))
    {
        cout << "Não foi possível exibir a fila pois ela está vazia!" << endl;
        return;
    }

    cout << "Imprimindo: '" << fila.inicio->arquivo << "'" << endl;

    No *temp = fila.inicio;
    fila.inicio = fila.inicio->proximo;
    delete temp;

    if (fila.inicio == nullptr)
    {
        fila.fim = nullptr;
    }
}

void verProximo(const Fila &fila)
{
    if (estaVazia(fila))
    {
        cout << "A fila está vazia";
        return;
    }
    cout << "Próximo a imprimir: " << fila.inicio->arquivo << endl;
}

void exibirFila(const Fila &fila)
{
    if (estaVazia(fila))
    {
        cout << "Fila vazia.\n";
        return;
    }

    cout << "Fila de impressão:\n";
    int pos = 1;
    No *atual = fila.inicio;
    while (atual != nullptr)
    {
        cout << "  " << pos++ << ". " << atual->arquivo << "\n";
        atual = atual->proximo;
    }
}

// Libera a memória da fila
void liberarFila(Fila &fila)
{
    No *atual = fila.inicio;
    while (atual != nullptr)
    {
        No *temp = atual;
        atual = atual->proximo;
        delete temp;
    }
    fila.inicio = nullptr;
    fila.fim = nullptr;
}

int main()
{
    Fila filaDeImpressao;

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