#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Horario
{
    int horas, minutos, segundos;
};

Horario gerarHora(Horario hora)
{
    hora.horas = rand() % 24;
    hora.minutos = rand() % 60;
    hora.segundos = rand() % 60;

    cout << hora.horas << ":" << hora.minutos << ":" << hora.segundos << endl;
    return hora;
}

void somahora(Horario horaA, Horario horaB)
{
    Horario resultado;
    resultado.horas = horaA.horas + horaB.horas;
    resultado.minutos = horaA.minutos + horaB.minutos;
    resultado.segundos = horaA.segundos + horaB.segundos;

    if (resultado.segundos >= 60)
    {
        resultado.segundos -= 60;
        resultado.minutos++;
    }
    if (resultado.minutos >= 60)
    {
        resultado.minutos -= 60;
        resultado.horas++;
    }
    if (resultado.horas >= 24)
    {
        resultado.horas -= 24;
    }

    cout << resultado.horas << ":" << resultado.minutos << ":" << resultado.segundos << endl;
}

int main()
{
    srand(time(0));
    Horario horaA, horaB;

    horaA = gerarHora(horaA);
    horaB = gerarHora(horaB);
    somahora(horaA, horaB);

    return 0;
}