#include <iostream>
using namespace std;

struct No
{
    int valor;
    No *esquerda;
    No *direita;
};

int contaFolhas(No *arvore)
{
    if (arvore == NULL)
        return 0;

    if (arvore->esquerda == NULL && arvore->direita == NULL)
        return 1;

    return contaFolhas(arvore->esquerda) + contaFolhas(arvore->direita);
}

int altura(No *arvore)
{
    if (arvore == NULL)
        return -1;

    int altEsquerda = altura(arvore->esquerda);
    int altDireita = altura(arvore->direita);

    return 1 + (altEsquerda > altDireita ? altEsquerda : altDireita);
}

int main()
{
    No *n4 = new No{4, NULL, NULL};
    No *n5 = new No{5, NULL, NULL};
    No *n3 = new No{3, NULL, NULL};
    No *n2 = new No{2, n4, n5};
    No *raiz = new No{1, n2, n3};

    cout << "Total de folhas: " << contaFolhas(raiz) << endl;
    cout << "Altura da arvore: " << altura(raiz) << endl;

    return 0;
}