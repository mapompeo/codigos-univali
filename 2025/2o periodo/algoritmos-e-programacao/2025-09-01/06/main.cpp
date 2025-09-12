#include <iostream>
#include <time.h>
#include <fstream>
using namespace std;
ofstream dados;

struct Nascimento
{
    int mes;
    int ano;
};

struct Gado
{
    int codigo;
    float litrosLeite;
    float alim;
    Nascimento data;
    bool abate;
};

void gerarDados()
{
    dados.open("dados.csv");
    Gado gado[2000];
    for (int i = 0; i < 2000; i++)
    {
        Gado temp;
        dados << temp.codigo = rand() % 10000 << ", ";
        daods << temp.litrosLeite = rand() % 200;
        temp.alim = rand() % 100;
        temp.data.mes = rand() % 12 + 1;
        temp.data.ano = rand() % 10 + 2015;
        temp.abate = false;
        dados[i] << temp << endl;
    }
    dados.close();
}

void lerDados()
{
}

int mesAtual()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return ltm->tm_mon + 1;
}

int anoAtual()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return ltm->tm_year + 1900;
}

float leiteSemana(struct Gado gado)
{
    return gado.litrosLeite / 7;
}

int abate(struct Gado gado)
{

    ifstream dados;
    dados.open("dados.csv");

    if (gado.data.ano < anoAtual() - 5 && gado.data.mes < mesAtual() || gado.litrosLeite < 40 || leiteSemana(gado) <= 70)
    {
        gado.abate = true;
    }
    return gado.abate;
}

int main()
{
    srand(time(NULL));
    gerarDados();
    return 0;
}