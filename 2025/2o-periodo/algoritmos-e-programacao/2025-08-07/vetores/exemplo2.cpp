#include <iostream>
using namespace std;

int main()
{
    int vet[15], tamanho = sizeof(vet) / sizeof(vet[0]);

    for (int i = 0; i < tamanho; i++)
    {
        vet[i] = i;
    }

    for (int i = tamanho - 1; i >= 0; i--)
    {
        cout << vet[i] << endl;
    }

    return 0;
}
