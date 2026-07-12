#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void vetor()
{
    int tamanhoVet = 30, vetor[tamanhoVet], div2 = 0, div3 = 0;

    for (int i = 0; i < tamanhoVet; i++)
    {
        vetor[i] = rand() % 100;
    }

    cout << "Os valores do vetor são: ";
    for (int i = 0; i < tamanhoVet; i++)
    {
        cout << vetor[i] << ", ";
    }
    cout << endl;

    for (int i = 0; i < tamanhoVet; i++)
    {
        if (vetor[i] % 2 == 0)
        {
            div2 ++;
        }
        if (vetor[i] % 3 == 0)
        {
            div3 ++;
        }
    }

    cout << "A quantidade de valores divisíveis por dois são: " << div2 << endl;
    cout << "A quantidade de valores divisíveis por três são: " << div3 << endl;
}

int main()
{
    vetor();
    srand(time(0));
    return 0;
}