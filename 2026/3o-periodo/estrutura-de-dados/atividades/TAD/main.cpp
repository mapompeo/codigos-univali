#include <stdio.h>
#include "vetor.h"

int main()
{
    printf("Teste 1: insercao e crescimento dinamico\n");

    Vetor *v = vet_cria(2);

    float valores[] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++)
    {
        vet_insere(v, valores[i]);
        printf("  Inserido %.1f | tamanho: %d\n", valores[i], vet_tamanho(v));
    }

    printf("\nTeste 2: acesso a todos os elementos\n");
    for (int i = 0; i < vet_tamanho(v); i++)
    {
        printf("  v[%d] = %.1f\n", i, vet_acessa(v, i));
    }

    printf("\nTeste 3: acesso a indices invalidos\n");
    vet_acessa(v, -1);
    vet_acessa(v, 100);

    printf("\nTeste 4: liberacao de memoria\n");
    vet_libera(v);
    printf(" Memoria liberada com sucesso.\n");

    printf("\nTeste 5: criacao com capacidade invalida\n");
    Vetor *v2 = vet_cria(0);
    if (v2 == NULL)
    {
        printf(" vet_cria(0) retornou NULL corretamente.\n");
    }

    return 0;
}
