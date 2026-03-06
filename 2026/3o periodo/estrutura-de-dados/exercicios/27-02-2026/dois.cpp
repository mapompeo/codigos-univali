#include <iostream>
#include <stdio.h>
using namespace std;

main()
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
}