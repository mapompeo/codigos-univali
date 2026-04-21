#include <iostream>
#include <limits.h>
using namespace std;

struct No
{
    int valor;
    No *esquerda;
    No *direita;
};

No *criaNo(int dado)
{
    No *novoNo = new No();
    novoNo->valor = dado;
    novoNo->direita = NULL;
    novoNo->esquerda = NULL;
    return novoNo;
}

void exibir(No *noAtual)
{
    if (noAtual != NULL)
    {
        exibir(noAtual->esquerda);
        cout << "- " << noAtual->valor << endl;
        exibir(noAtual->direita);
    }
}

No *inserir(No *noAtual, int dado)
{
    if (noAtual == NULL)
    {
        return criaNo(dado);
    }

    if (dado < noAtual->valor)
    {
        noAtual->esquerda = inserir(noAtual->esquerda, dado);
    }

    else if (dado > noAtual->valor)
    {
        noAtual->direita = inserir(noAtual->direita, dado);
    }

    return noAtual;
}

int contarFolhas(No *noAtual)
{
    if (noAtual == NULL)
        return 0;

    if (noAtual->esquerda == NULL && noAtual->direita == NULL)
        return 1;

    return contarFolhas(noAtual->esquerda) + contarFolhas(noAtual->direita);
}

int ehBST(No *noAtual, int min, int max)
{
    if (noAtual == NULL)
        return 1;
    if (noAtual->valor < min || noAtual->valor > max)
        return 0;
    return ehBST(noAtual->esquerda, min, noAtual->valor - 1) && ehBST(noAtual->direita, noAtual->valor + 1, max);
}

int main()
{
    // insere os valores na arvore
    cout << "Inserindo valores na árvore: " << endl;
    No *arvore = NULL;
    arvore = inserir(arvore, 50);
    arvore = inserir(arvore, 30);
    arvore = inserir(arvore, 20);
    arvore = inserir(arvore, 40);
    arvore = inserir(arvore, 70);
    arvore = inserir(arvore, 60);
    arvore = inserir(arvore, 80);
    exibir(arvore);
    cout << endl;

    // conta o totla de folhas
    int total = contarFolhas(arvore);
    cout << "Total de folhas: " << total << endl;
    cout << endl;

    // verifica se é bst
    int resultado = ehBST(arvore, INT_MIN, INT_MAX);
    cout << "É uma BST? " << resultado ? "Sim" : "Não";
    return 0;
}