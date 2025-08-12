#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

void vetor()
{
    int tamanhoVet = 10, vetor1[tamanhoVet], vetor2[tamanhoVet], vetor3[tamanhoVet];

    for (int i = 0; i < tamanhoVet; i++)
    {
        vetor1[i] = i + 1;
        vetor2[i] = i + 1;
    }

    cout << "Os valores do vetor 1 é : " << endl;
    for (int i = 0; i < tamanhoVet; i++)
    {
        cout << vetor1[i] << " ";
    }

    cout << endl;

    cout << "Os valores do vetor 2 é : " << endl;
    for (int i = 0; i < tamanhoVet; i++)
    {
        cout << vetor2[i] << " ";
    }

    for (int i = 0; i < tamanhoVet; i++)
    {
        if (vetor1[i] == vetor2[i])
        {
            vetor3[i] = vetor1[i] * vetor2[i];
        }
    }

    cout << endl;

    cout << "Os valores do vetor resultado é : " << endl;
    for (int i = 0; i < tamanhoVet; i++)
    {
        cout << vetor3[i] << " ";
    }
}

int main()
{
    vetor();
    return 0;
}
