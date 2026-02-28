#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

#define TAMANHO 5

void matriz()
{
    int linha = TAMANHO, coluna = TAMANHO, matrizA[linha][coluna], maiorElemento = 0;

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
                if (matrizA[i][j] > maiorElemento)
                {
                    maiorElemento = matrizA[i][j];
                }
            }
        }
    }
    cout << "O maior elemento da matriz: " << maiorElemento << endl;
}

int main()
{
    srand(time(0));
    matriz();
    return 0;
}