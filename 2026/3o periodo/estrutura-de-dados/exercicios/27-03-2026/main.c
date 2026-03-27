#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tamanho 11

typedef struct Aluno
{
    int matricula;
    char nome[100];
} Aluno;

typedef struct No
{
    Aluno aluno;
    struct No *proximo;
} No;

typedef struct TabelaHash
{
    No *dados[tamanho];
} TabelaHash;

int funcaoHash(int chave)
{
    return chave % tamanho;
}

void inserirExterior(TabelaHash *tabela, Aluno alunoNovo)
{
    int id = funcaoHash(alunoNovo.matricula);
    No *novoNo = (No *)calloc(1, sizeof(No));
    novoNo->aluno = alunoNovo;
    novoNo->proximo = tabela->dados[id];
    tabela->dados[id] = novoNo;

    printf("Aluno %s inserido no indice %d (exterior)\n", alunoNovo.nome, id);
}

void exibirTabelaHash(TabelaHash *tabela)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("Posicao [%d]: ", i);

        No *atual = tabela->dados[i];
        while (atual != NULL)
        {
            printf("{%d, %s} -> ", atual->aluno.matricula, atual->aluno.nome);
            atual = atual->proximo;
        }
        printf("NULL\n");
    }
}

int main()
{
    TabelaHash *tabela = (TabelaHash *)calloc(1, sizeof(TabelaHash));

    Aluno joao = {123, "joao"};
    Aluno alice = {12, "alice"};
    Aluno cleber = {23, "cleber"};
    Aluno josias = {10, "josias"};

    inserirExterior(tabela, joao);
    inserirExterior(tabela, alice);
    inserirExterior(tabela, cleber);
    inserirExterior(tabela, josias);

    return 0;
}
