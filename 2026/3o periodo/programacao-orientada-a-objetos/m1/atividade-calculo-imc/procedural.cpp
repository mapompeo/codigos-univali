#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

double calcularIMC(double peso, double altura)
{
    return peso / (altura * altura);
}

// classificação com base na tabela da oms
string classificarIMC(double imc)
{
    if (imc < 18.5)
        return "Abaixo do peso";
    else if (imc < 25.0)
        return "Peso normal";
    else if (imc < 30.0)
        return "Sobrepeso";
    else if (imc < 35.0)
        return "Obesidade Grau I";
    else if (imc < 40.0)
        return "Obesidade Grau II";
    else
        return "Obesidade Grau III";
}

// exibe o resultado
void exibirResultado(double peso, double altura, double imc, const string &classificacao)
{
    cout << "\n========== RESULTADO ==========\n";
    cout << fixed << setprecision(2);
    cout << "Peso:           " << peso << " kg\n";
    cout << "Altura:         " << altura << " m\n";
    cout << "IMC:            " << imc << "\n";
    cout << "Classificacao:  " << classificacao << "\n";
    cout << "================================\n";
}

int main()
{
    double peso, altura;

    cout << "=== Calculadora de IMC (Procedural) ===\n\n";

    // recebe os dados
    cout << "Digite o peso (kg): ";
    cin >> peso;
    cout << "Digite a altura (m): ";
    cin >> altura;

    // valida os valores
    if (altura <= 0 || peso <= 0)
    {
        cout << "Erro: peso e altura devem ser valores positivos.\n";
        return 1;
    }

    // chama as funções em sequência: calcula, classifica e exibe
    double imc = calcularIMC(peso, altura);
    string classificacao = classificarIMC(imc);
    exibirResultado(peso, altura, imc, classificacao);

    return 0;
}
