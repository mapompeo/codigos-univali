#include <iostream>
#include <time.h>
#include <cmath>
using namespace std;

struct Vector3
{
    int x;
    int y;
    int z;
};

Vector3 gerarValoresVetor(Vector3 &vetor)
{
    vetor.x = rand() % 10;
    vetor.y = rand() % 10;
    vetor.z = rand() % 10;
    return vetor;
}

Vector3 GetMagnitude(Vector3 &vetor)
{
    Vector3 resultado;
    resultado.x = sqrt(vetor.x * vetor.x + vetor.y * vetor.y + vetor.z * vetor.z);
    return resultado;
}

Vector3 GetNormalized(Vector3 &vetor)
{
    Vector3 resultado;

    return resultado;
}

Vector3 distance(Vector3 vetor1, Vector3 vetor2)
{
    Vector3 resultado;

    resultado.x = vetor1.x - vetor2.x;
    resultado.y = vetor1.y - vetor2.y;
    resultado.z = vetor1.z - vetor2.z;

    return resultado;
}

int main()
{
    Vector3 vetor;
    srand(time(0));
    system("chcp 65001");



    

    gerarValoresVetor(vetor);
    cout << "Valores: " << vetor.x << ", " << vetor.y << ", " << vetor.z << endl;

    Vector3 magnitude = GetMagnitude(vetor);
    cout << "Magnitude: " << magnitude.x << endl;

    Vector3 normalizado = GetNormalized(vetor);
    cout << "Normalizado: " << normalizado.x << ", " << normalizado.y << ", " << normalizado.z << endl;





    Vector3 vetor1, vetor2;

    gerarValoresVetor(vetor1);
    cout << "Valores (Vetor 1): " << vetor1.x << ", " << vetor1.y << ", " << vetor1.z << endl;

    gerarValoresVetor(vetor2);
    cout << "Valores (Vetor 2): " << vetor2.x << ", " << vetor2.y << ", " << vetor2.z << endl;

    Vector3 distancia = distance(vetor1, vetor2);
    cout << "Distância: " << distancia.x << ", " << distancia.y << ", " << distancia.z << endl;

    return 0;
}