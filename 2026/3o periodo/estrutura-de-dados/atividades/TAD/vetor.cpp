#include <stdio.h>
#include <stdlib.h>
#include "vetor.h"

struct vetor
{
    float *dados;
    int capacidade;
    int tamanho;
};

Vetor *vet_cria(int cap)
{
    if (cap <= 0)
    {
        fprintf(stderr, "vet_cria: capacidade inicial deve ser positiva.\n");
        return NULL;
    }
    Vetor *v = (Vetor *)malloc(sizeof(Vetor));
    if (v == NULL)
    {
        fprintf(stderr, "vet_cria: falha ao alocar a estrutura.\n");
        return NULL;
    }
    v->dados = (float *)malloc(cap * sizeof(float));
    if (v->dados == NULL)
    {
        fprintf(stderr, "vet_cria: falha ao alocar o array interno.\n");
        free(v);
        return NULL;
    }
    v->capacidade = cap;
    v->tamanho = 0;
    return v;
}

void vet_insere(Vetor *v, float valor)
{
    if (v == NULL)
    {
        fprintf(stderr, "vet_insere: ponteiro nulo.\n");
        return;
    }
    if (v->tamanho == v->capacidade)
    {
        int nova_cap = v->capacidade * 2;
        float *novo_bloco = (float *)realloc(v->dados, nova_cap * sizeof(float));
        if (novo_bloco == NULL)
        {
            fprintf(stderr, "vet_insere: falha ao realocar memoria.\n");
            return;
        }
        v->dados = novo_bloco;
        v->capacidade = nova_cap;
    }
    v->dados[v->tamanho] = valor;
    v->tamanho++;
}

float vet_acessa(Vetor *v, int i)
{
    if (v == NULL)
    {
        fprintf(stderr, "vet_acessa: ponteiro nulo.\n");
        return 0.0f;
    }
    if (i < 0 || i >= v->tamanho)
    {
        fprintf(stderr, "vet_acessa: indice %d invalido (tamanho atual: %d).\n",
                i, v->tamanho);
        return 0.0f;
    }
    return v->dados[i];
}

int vet_tamanho(Vetor *v)
{
    if (v == NULL)
        return 0;
    return v->tamanho;
}

void vet_libera(Vetor *v)
{
    if (v == NULL)
        return;
    free(v->dados);
    free(v);
}
