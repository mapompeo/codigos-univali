#include <iostream>
#include <stdio.h>
using namespace std;

main()
{
    int valor, maior = 0, menor = 0;

    for (int i = 5; i > 0; i--)
    {
        cout << "Insira um valor (" << i << " restantes): ";
        cin >> valor;
        if (valor > maior)
        {
            maior == valor;
        }
        else if (valor < menor)
        {
            menor == valor;
        }
    }

    cout << "O maior valor é " << maior << endl;
    cout << "O menor valor é " << menor << endl;
}