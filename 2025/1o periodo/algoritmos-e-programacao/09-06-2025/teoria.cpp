#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

#define TAMANHO 5

void matriz()
{
    int linha = TAMANHO, coluna = TAMANHO, matriz[linha][coluna];

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            matriz[i][j] = rand() % 100 + 1;
        }
    }

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    matriz();
    return 0;
}