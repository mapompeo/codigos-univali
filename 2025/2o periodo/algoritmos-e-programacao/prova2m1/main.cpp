#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <windows.h>

using namespace std;

struct Nascimento
{
    int dia, mes, ano;
};

struct Pessoa
{
    string nome;
    Nascimento nascimento;
    string cpf;
};

bool bissexto(int ano)
{
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

bool validaData(const Pessoa &pessoa)
{
    int dia = pessoa.nascimento.dia;
    int mes = pessoa.nascimento.mes;
    int ano = pessoa.nascimento.ano;

    if (ano <= 0)
        return false;
    if (mes < 1 || mes > 12)
        return false;

    // verifica o total de dias por mes dentro do array
    int diasPorMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mes == 2 && bissexto(ano))
    {
        diasPorMes[2] = 29;
    }

    if (dia < 1 || dia > diasPorMes[mes])
        return false;

    return true;
}

bool validaCPF(const Pessoa &pessoa)
{
    if (pessoa.cpf.size() != 11)
        return false;

    for (size_t i = 0; i < pessoa.cpf.size(); ++i)
    {
        if (!isdigit(pessoa.cpf[i]))
            return false;
    }

    // verifica se os digitos sao iguais e retorna invalido
    bool todosIguais = true;
    for (int i = 1; i < 11; ++i)
    {
        if (pessoa.cpf[i] != pessoa.cpf[0])
        {
            todosIguais = false;
            break;
        }
    }

    // condicao simplificada
    if (todosIguais)
        return false;

    return true;
}

bool validaPessoa(const Pessoa &pessoa)
{
    return validaData(pessoa) && validaCPF(pessoa);
}

int verificaDados(vector<Pessoa> &pessoas)
{
    ifstream arquivo("../dados.txt");
    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    string linhaNome, linhaData, linhaCPF;

    while (getline(arquivo, linhaNome))
    {
        if (linhaNome.empty())
            continue; // pula as linhas em branco

        if (!getline(arquivo, linhaData))
            break;
        if (!getline(arquivo, linhaCPF))
            break;

        Pessoa p;
        p.nome = linhaNome;
        p.cpf = linhaCPF;

        if (linhaData.size() != 10 || linhaData[2] != '/' || linhaData[5] != '/')
            continue;

        char barra1, barra2;
        stringstream ss(linhaData);
        ss >> p.nascimento.dia >> barra1 >> p.nascimento.mes >> barra2 >> p.nascimento.ano;

        if (validaPessoa(p))
        {
            pessoas.push_back(p);
        }
    }

    arquivo.close();

    // mostra o nome das pessoas ok
    for (const auto &p : pessoas)
    {
        cout << p.nome << endl;
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    vector<Pessoa> pessoas;
    verificaDados(pessoas);

    return 0;
}
