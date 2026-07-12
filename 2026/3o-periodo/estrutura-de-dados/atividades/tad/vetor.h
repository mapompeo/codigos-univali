#ifndef VETOR_H
#define VETOR_H

typedef struct vetor Vetor;

Vetor *vet_cria(int cap);
void vet_insere(Vetor *v, float valor);
float vet_acessa(Vetor *v, int i);
int vet_tamanho(Vetor *v);
void vet_libera(Vetor *v);

#endif
