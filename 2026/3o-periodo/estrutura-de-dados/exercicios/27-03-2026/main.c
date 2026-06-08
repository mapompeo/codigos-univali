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

int funcaoReHash(int chave)
{
    return chave++ % tamanho;
}

void inserirAberto(TabelaHash *tabela, Aluno alunoNovo)
{
    int id = funcaoHash(alunoNovo.matricula);

    int i = 0;
    while (i < tamanho)
    {
        int posicao = (id + i) % tamanho;
        if (tabela->dados[posicao] == NULL)
        {
            No *novoNo = (No *)calloc(1, sizeof(No));
            novoNo->aluno = alunoNovo;

            tabela->dados[posicao] = novoNo;
            return;
        }
        i++;
    }
    printf("Tabela cheia, não foi possível inserir o aluno %s\n,", alunoNovo.nome);
}

void imprimirAberto(TabelaHash *tabela)
{
    printf("Tabela Hash (Endereçamento Aberto):\n");
    for (int i = 0; i < tamanho; i++)
    {
        if (tabela->dados[i] != NULL)
        {
            printf("Posição [%d]: {Matricula: %d, Nome: %s}\n", i, tabela->dados[i]->aluno.matricula, tabela->dados[i]->aluno.nome);
        }
        else
        {
            printf("Posição [%d]: NULL\n", i);
        }
    }
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

Aluno *buscaExterior(TabelaHash *tabela, int matricula)
{
    int id = funcaoHash(matricula);

    No *atual = tabela->dados[id];

    while (atual != NULL)
    {
        if (atual->aluno.matricula == matricula)
            return &(atual->aluno);

        atual = atual->proximo;
    }
    return NULL;
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

    exibirTabelaHash(tabela);

    Aluno *buscaDaAlice = buscaExterior(tabela, 12);
    Aluno *buscaNulo = buscaExterior(tabela, 5678);

    printf("[Mat: %d | Nome: %s]\n", buscaDaAlice->matricula, buscaDaAlice->nome);
    if (buscaNulo != NULL)
    {
        printf("[Mat: %d | Nome: %s]\n", buscaNulo->matricula, buscaNulo->nome);
    }
    else
    {
        printf("Aluno nao encontrado!\n");
    }

    printf("-----------------------------------------\n");

    TabelaHash *tabelaAberto = (TabelaHash *)calloc(1, sizeof(TabelaHash));

    Aluno felipe = {46, "felipe"};
    Aluno lucas = {38, "lucas"};
    Aluno maria = {46, "maria"};

    inserirAberto(tabelaAberto, felipe);
    inserirAberto(tabelaAberto, lucas);
    inserirAberto(tabelaAberto, maria);

    imprimirAberto(tabelaAberto);

    return 0;
}
