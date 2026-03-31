#ifndef TAD_SENSOR_H
#define TAD_SENSOR_H

#include <string>

using namespace std;

struct Sensor
{
    int id;
    float temperatura;
};

struct ListaSensores
{
    Sensor dados[5];
    int tamanho;
};

void inserirSensor(ListaSensores *lista, int id, float temperatura);
void removerSensor(ListaSensores *lista, int id);

#endif
