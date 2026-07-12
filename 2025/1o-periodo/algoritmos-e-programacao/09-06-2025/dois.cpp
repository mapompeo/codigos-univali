#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

void matriz()
{
    int linha = 3, coluna = 5, matrizA[linha][coluna], matrizB[linha][coluna], matrizC[linha][coluna];

    // Matriz A e Matriz B
    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            matrizA[i][j] = rand() % 100 + 1;
            matrizB[i][j] = rand() % 100 + 1;
        }
    }

    cout << "Matriz A: " << endl;
    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            cout << matrizA[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "Matriz B: " << endl;
    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            cout << matrizB[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Gerar uma matriz C com a soma dos elementos da matriz A e B
    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            matrizC[i][j] = matrizA[i][j] + matrizB[i][j];
        }
    }

    cout << "Matriz C (matrizA + matrizB): " << endl;
    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            cout << matrizC[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Mostrar todos os elementos da linha 2 da matriz C
    cout << "Mostrar todos os elementos da linha 2 da matriz C: " << endl;
    for (int j = 0; j < coluna; j++)
    {
        cout << matrizC[1][j] << " ";
    }
    cout << endl;
    cout << endl;

    // Mostrar todos os elementos da coluna 3 da matriz C.
    cout << "Mostrar todos os elementos da coluna 3 da matriz C:" << endl;
    for (int i = 0; i < linha; i++)
    {
        cout << matrizC[i][2] << " " << endl;
    }
}

int main()
{
    srand(time(0));
    matriz();
    return 0;
}