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

int funcaoHash(chave)
{
    return chave % tamanho;
}

int inserirExterior(TabelaHash *tabela, Aluno alunoNovo)
{
    int id = funcaoHash(alunoNovo.matricula);
    No *novoNo = (No *)calloc(1, sizeof(No));
    novoNo->aluno = alunoNovo;
    novoNo->proximo = tabela->dados[id];
    tabela->dados[id] = novoNo;

    printf("ALuno %s inserido no índice %d (exterior)\n", alunoNovo.nome[id]);
}

int main()
{
    TabelaHash *tabela = (TabelaHash *)calloc(1, sizeof(TabelaHash));

    Aluno joao = {123, "joao"};
    Aluno alice = {30, "alice"};
    Aluno cleber = {10, "cleber"};
    Aluno josias = {45, "josias"};

    return 0;
}
