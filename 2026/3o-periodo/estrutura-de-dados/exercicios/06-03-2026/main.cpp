#include <iostream>
using namespace std;

int main()
{
    int vetor[] = {4, 7, 10, 2, 1};
    int *p = vetor;
    int n = 3;

    cout << "Valor do endereço inicial: " << p << endl;
    cout << "Valor de N: " << n << endl;
    int *resultado = p + n;

    cout << "Equacao do slide: p + n = 100 + (n * 4)" << endl;
    cout << "Endereço final (p + 3): " << resultado << endl;
    cout << "Valor no endereço (vetor[3]): " << *resultado << endl;

    return 0;
}