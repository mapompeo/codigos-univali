#include <iostream>
using namespace std;

void vetor()
{
    int tamanhoVet = 5, maiorPrimeiro = 0, vetor[tamanhoVet];

    for (int i = 0; i < tamanhoVet; i++)
    {
        cout << "Digite os valores do vetor: ";
        cin >> vetor[i];
    }

    for (int i = 0; i < tamanhoVet; i++)
    {
        if (vetor[i] < vetor[i + 1])
        {
            maiorPrimeiro++;
        }
    }

    cout << "Quantidade de vezes que um número é maior que o próximo: " << maiorPrimeiro << endl;
}

int main()
{
    vetor();
    return 0;
}
