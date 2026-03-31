#include "TAD_Sensor.h"
#include <iostream>

using namespace std;

void inserirSensor(ListaSensores *lista, int id, float temperatura)
{
    int quantidade = lista->tamanho;

    if (quantidade < 5)
    {
        Sensor novoSensor;
        novoSensor.id = id;
        novoSensor.temperatura = temperatura;
        lista->dados[quantidade] = novoSensor;
        lista->tamanho++;
    }
}

void removerSensor(ListaSensores *lista, int id)
{
    int quantidade = lista->tamanho;

    for (int i = 0; i < quantidade; i++)
    {
        if (lista->dados[i].id == id)
        {
            for (int j = i; j < quantidade - 1; j++)
            {
                lista->dados[j] = lista->dados[j + 1];
            }
            lista->tamanho--;
            break;
        }
    }
}
