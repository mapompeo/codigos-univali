#include <iostream>
#include <iterator>
using namespace std;

int main()
{
    int vet[20], tamanho = sizeof(vet) / sizeof(vet[0]);

    for (int i = 0; i < tamanho; i++)
    {
        vet[i] = i;
    }

    for (int i = 0; i < tamanho; i++)
    {
        if (vet[i] % 2 == 0)
        {
            cout << "Valor par: " << vet[i] << ", posição: " << i << endl;
        }
    }

    return 0;
}
