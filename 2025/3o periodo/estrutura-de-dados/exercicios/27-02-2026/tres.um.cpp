#include <iostream>
#include <stdio.h>
using namespace std;

int um()
{
    int value = 0;
    cout << "Digite um valor: ";
    cin >> value;

    if (value > 0)
    {
        cout << "O valor é positivo";
    }
    else if (value < 0)
    {
        cout << "O valor é negativo";
    }
    else
    {
        cout << "O valor é zero";
    }

    if (value % 2 == 0)
    {
        cout << "O valor é par";
    }
    else
    {
        cout << "O valor é ímpar";
    }

    return 0;
}

int dois()
{
    int value = 0;
    cout << "Digite um valor: ";
    cin >> value;

    for (int i = 0; i < value; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (i % 3 == 0 && j % 3 == 0)
            {
                cout << i << " X " << j << endl;
            }
        }
    }

    return 0;
}

main()
{
    system("cls");
    cout << "1 - Olá" << endl;
    cout << "2 - Hora atual (simulada)" << endl;
    cout << "0 - Sair" << endl;

    int value = 0;
    cout << "Digite um valor: ";
    cin >> value;

    switch (value)
    {
    case 1:
        system("cls");
        um();
        break;
    case 2:
        system("cls");
        dois();
    default:
        break;
    }
}