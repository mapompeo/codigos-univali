#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

#define TAMANHO 4

void matriz()
{
    int linha = TAMANHO, coluna = TAMANHO, matrizA[linha][coluna];

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            matrizA[i][j] = rand() % 10;
            cout << matrizA[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Elemento na diagonal principal: " << endl;

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            if (i == j)
            {
                cout << matrizA[i][j] << endl;
            }
        }
    }
}

int main()
{
    srand(time(0));
    matriz();
    return 0;
}