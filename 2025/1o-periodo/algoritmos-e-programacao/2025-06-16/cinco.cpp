#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

#define TAMANHO 4

void matriz()
{
    int linha = TAMANHO, coluna = TAMANHO, matrizA[linha][coluna], secundaria = 0;

    for (int i = 0; i < TAMANHO; i++)
    {
        for (int j = 0; j < TAMANHO; j++)
        {
            matrizA[i][j] = rand() % 10;
            cout << matrizA[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < TAMANHO; i++)
    {
        secundaria += matrizA[i][TAMANHO - 1 - i];
    }

    cout << "Soma dos elementos da diagonal secundária: " << secundaria << endl;
}

int main()
{
    srand(time(0));
    matriz();
    return 0;
}