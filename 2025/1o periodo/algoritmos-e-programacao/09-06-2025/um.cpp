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
            cout << "Digite o valor da linha " << i + 1 << " e coluna " << j + 1 << ": ";
            cin >> matrizA[i][j];
        }
    }
    cout << endl;

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            cout << matrizA[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            resultado += matrizA[i][j];
        }
        cout << endl;
    }

    cout << endl;
    cout << "O resultado da soma dos elementos da matriz é: " << resultado << endl;
}

int main()
{
    matriz();
    return 0;
}