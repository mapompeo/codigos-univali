#include <iostream>
#include <string>

using namespace std;

class PilhaEstatica
{
private:
    static const int TAMANHO_MAXIMO = 200;
    char dados[TAMANHO_MAXIMO];
    int topo;

public:
    PilhaEstatica() : topo(-1) {}

    bool vazia() const
    {
        return topo == -1;
    }

    bool cheia() const
    {
        return topo == TAMANHO_MAXIMO - 1;
    }

    bool empilhar(char valor)
    {
        if (cheia())
        {
            return false;
        }
        topo++;
        dados[topo] = valor;
        return true;
    }

    bool desempilhar(char &valor)
    {
        if (vazia())
        {
            return false;
        }
        valor = dados[topo];
        topo--;
        return true;
    }
};

struct No
{
    char valor;
    No *proximo;
};

class PilhaDinamica
{
private:
    No *topo;

public:
    PilhaDinamica() : topo(nullptr) {}

    ~PilhaDinamica()
    {
        char descartado;
        while (desempilhar(descartado))
        {
        }
    }

    bool vazia() const
    {
        return topo == nullptr;
    }

    bool empilhar(char valor)
    {
        No *novo = new (nothrow) No;
        if (novo == nullptr)
        {
            return false;
        }
        novo->valor = valor;
        novo->proximo = topo;
        topo = novo;
        return true;
    }

    bool desempilhar(char &valor)
    {
        if (vazia())
        {
            return false;
        }
        No *removido = topo;
        valor = removido->valor;
        topo = topo->proximo;
        delete removido;
        return true;
    }
};

string inverterStringComPilhaEstatica(const string &texto)
{
    PilhaEstatica pilha;
    for (char c : texto)
    {
        if (!pilha.empilhar(c))
        {
            return "Erro: capacidade da pilha estatica excedida.";
        }
    }

    string invertida;
    char c;
    while (pilha.desempilhar(c))
    {
        invertida += c;
    }

    return invertida;
}

bool parentesesBalanceados(const string &expressao)
{
    PilhaDinamica pilha;

    for (char c : expressao)
    {
        if (c == '(')
        {
            if (!pilha.empilhar(c))
            {
                return false;
            }
        }
        else if (c == ')')
        {
            char topo;
            if (!pilha.desempilhar(topo))
            {
                return false;
            }
        }
    }

    return pilha.vazia();
}

int main()
{
    string palavra;
    string expressao;

    cout << "1) Pilha estatica - inverter string\n";
    cout << "Digite uma palavra: ";
    getline(cin, palavra);

    string invertida = inverterStringComPilhaEstatica(palavra);
    cout << "Resultado invertido: " << invertida << "\n\n";

    cout << "2) Pilha dinamica - validacao de parenteses\n";
    cout << "Digite uma expressao: ";
    getline(cin, expressao);

    if (parentesesBalanceados(expressao))
    {
        cout << "Parenteses balanceados." << endl;
    }
    else
    {
        cout << "Parenteses NAO balanceados." << endl;
    }

    return 0;
}
