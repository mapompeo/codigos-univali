#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#define TAMANHO 20

void matriz()
{
    float matrizA[TAMANHO][TAMANHO];
    float matrizB[TAMANHO][TAMANHO];
    float somaLinhas[TAMANHO] = {0};

    for (int i = 0; i < TAMANHO; i++)
    {
        for (int j = 0; j < TAMANHO; j++)
        {
            matrizA[i][j] = rand() % 10;
            somaLinhas[i] += matrizA[i][j];
        }
    }

    for (int i = 0; i < TAMANHO; i++)
    {
        for (int j = 0; j < TAMANHO; j++)
        {
            matrizB[i][j] = matrizA[i][j] * somaLinhas[i];
        }
    }

    cout << "Matriz resultante (cada elemento da matrizA multiplicado pela soma da sua linha):\n\n";
    for (int i = 0; i < TAMANHO; i++)
    {
        for (int j = 0; j < TAMANHO; j++)
        {
            cout << setw(8) << matrizB[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    srand(time(0));
    matriz();
    return 0;
}
