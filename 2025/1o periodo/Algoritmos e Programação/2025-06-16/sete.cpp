#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#define TAMANHO 10

void matriz()
{
    int matrizA[TAMANHO][TAMANHO], vetorMaiorLinha[TAMANHO], vetorMenorColuna[TAMANHO];

    for (int j = 0; j < TAMANHO; j++)
    {
        vetorMenorColuna[j] = 1000;
    }

    for (int i = 0; i < TAMANHO; i++)
    {
        vetorMaiorLinha[i] = -1;
        for (int j = 0; j < TAMANHO; j++)
        {
            matrizA[i][j] = rand() % 10;
            cout << matrizA[i][j] << "\t";

            if (matrizA[i][j] > vetorMaiorLinha[i])
            {
                vetorMaiorLinha[i] = matrizA[i][j];
            }
            if (matrizA[i][j] < vetorMenorColuna[j])
            {
                vetorMenorColuna[j] = matrizA[i][j];
            }
        }
        cout << endl;
    }

    cout << "\nVetor com os maiores elementos de cada linha:\n";
    for (int i = 0; i < TAMANHO; i++)
    {
        cout << vetorMaiorLinha[i] << " ";
    }

    cout << "\n\nVetor com os menores elementos de cada coluna:\n";
    for (int j = 0; j < TAMANHO; j++)
    {
        cout << vetorMenorColuna[j] << " ";
    }
    cout << endl;
}

int main()
{
    srand(time(0));
    matriz();
    return 0;
}
