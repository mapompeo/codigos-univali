#include <iostream>
using namespace std;

int main()
{
    const int linha = 5, coluna = 5;
    int matriz[linha][coluna];

    int valor = 0;
    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            matriz[i][j] = valor;
            valor++;
        }
    }

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            if (i == j)
            {
                cout << matriz[i][j] << "\t";
            }
            else
            {
                cout << "* " << "\t";
            }
        }
        cout << endl;
    }

    return 0;
}
