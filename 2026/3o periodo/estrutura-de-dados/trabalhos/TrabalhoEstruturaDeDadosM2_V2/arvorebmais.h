#pragma once
#include <iostream>
using namespace std;

struct No {
    int* chaves;
    int tamanhoChaves;
    No** filhos;
    int tamanhoFilhos;
    bool folha;
    No* proximo;

    No(bool eFolha, int grau);
    ~No();
};

struct ArvoreBMais {
    No* raiz;
    int grau;

    ArvoreBMais(int grau);
    ~ArvoreBMais();

    bool buscar(int chave);
    void inserir(int chave);
    void remover(int chave);
    void imprimirArvore();

    void destruirArvore(No* no);
    void removerEm(No* no, int chave);
    void inserirNaoCheio(No* atual, int chave);
    void dividir(No* pai, int indice, No* no);
    void roubarDaEsquerda(No* pai, int i);
    void roubarDaDireita(No* pai, int i);
    void fundir(No* pai, int indice, No* anterior, No* sucessor);
    void rotacionarDireita(No* pai, int indice);
    void rotacionarEsquerda(No* pai, int indice);
    int obterChaveMinima(No* no);
    int obterChaveMaxima(No* no);
    No* encontrarPai(No* filho);
};
