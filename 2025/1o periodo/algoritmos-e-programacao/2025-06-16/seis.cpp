#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

#define TAMANHO 5

void matriz()
{
    int linha = TAMANHO, coluna = TAMANHO, matrizA[linha][coluna], vetorLinhas[TAMANHO] = {0}, vetorColunas[TAMANHO] = {0};

    for (int i = 0; i < TAMANHO; i++)
    {
        for (int j = 0; j < TAMANHO; j++)
        {
            matrizA[i][j] = rand() % 10;
            cout << matrizA[i][j] << " ";
            vetorLinhas[i] += matrizA[i][j];
            vetorColunas[j] += matrizA[i][j];
        }
        cout << endl;
    }
    cout << endl;

    cout << "Vetor soma das linhas: ";
    for (int i = 0; i < TAMANHO; i++)
    {
        cout << vetorLinhas[i] << " ";
    }
    cout << endl;

    cout << "Vetor soma das colunas: ";
    for (int j = 0; j < TAMANHO; j++)
    {
        cout << vetorColunas[j] << " ";
    }
}

int main()
{
    srand(time(0));
    matriz();
    return 0;
}