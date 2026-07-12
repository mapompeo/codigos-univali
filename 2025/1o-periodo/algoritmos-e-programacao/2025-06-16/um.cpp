#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

#define TAMANHO 5

void matriz()
{
    int linha = TAMANHO, coluna = TAMANHO, matrizA[linha][coluna], resultado = 0;

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            matrizA[i][j] = rand() % 10;
            cout << matrizA[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            {
                resultado += matrizA[i][j];
            }
        }
    }
    cout << "Soma dos elementos da matriz: " << resultado << endl;
    cout << resultado << endl;
}

int main()
{
    srand(time(0));
    matriz();
    return 0;
}