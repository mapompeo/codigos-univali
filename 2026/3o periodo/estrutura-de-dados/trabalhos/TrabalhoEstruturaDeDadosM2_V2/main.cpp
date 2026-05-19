#include "arvorebmais.h"

int main() {
    ArvoreBMais arvore(3);

    arvore.inserir(1);
    arvore.inserir(2);
    arvore.inserir(3);
    arvore.inserir(4);
    arvore.inserir(5);
    arvore.inserir(6);
    arvore.inserir(7);
    arvore.inserir(8);
    arvore.inserir(9);

    arvore.imprimirArvore();

    arvore.remover(3);

    arvore.imprimirArvore();

    return 0;
}
