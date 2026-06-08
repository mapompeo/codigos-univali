#include <iostream>
#include <limits>
#include <string>

using namespace std;

class FilaImpressaoEstatica
{
private:
    static const int TAMANHO_MAXIMO = 100;
    string dados[TAMANHO_MAXIMO];
    int inicio;
    int fim;
    int quantidade;

public:
    FilaImpressaoEstatica() : inicio(0), fim(0), quantidade(0) {}

    bool vazia() const
    {
        return quantidade == 0;
    }

    bool cheia() const
    {
        return quantidade == TAMANHO_MAXIMO;
    }

    bool enfileirar(const string &nomeArquivo)
    {
        if (cheia())
        {
            return false;
        }

        dados[fim] = nomeArquivo;
        fim = (fim + 1) % TAMANHO_MAXIMO;
        quantidade++;
        return true;
    }

    bool imprimirProximo(string &arquivo)
    {
        if (vazia())
        {
            return false;
        }

        arquivo = dados[inicio];
        inicio = (inicio + 1) % TAMANHO_MAXIMO;
        quantidade--;
        return true;
    }
};

struct Ticket
{
    int id;
    string nome;
};

struct NoTicket
{
    Ticket dado;
    NoTicket *proximo;
};

class FilaChamadosDinamica
{
private:
    NoTicket *inicio;
    NoTicket *fim;

public:
    FilaChamadosDinamica() : inicio(nullptr), fim(nullptr) {}

    ~FilaChamadosDinamica()
    {
        Ticket descartado;
        while (desenfileirar(descartado))
        {
        }
    }

    bool vazia() const
    {
        return inicio == nullptr;
    }

    bool enfileirar(const Ticket &ticket)
    {
        NoTicket *novo = new (nothrow) NoTicket;
        if (novo == nullptr)
        {
            return false;
        }

        novo->dado = ticket;
        novo->proximo = nullptr;

        if (vazia())
        {
            inicio = novo;
            fim = novo;
        }
        else
        {
            fim->proximo = novo;
            fim = novo;
        }

        return true;
    }

    bool desenfileirar(Ticket &ticket)
    {
        if (vazia())
        {
            return false;
        }

        NoTicket *removido = inicio;
        ticket = removido->dado;
        inicio = inicio->proximo;

        if (inicio == nullptr)
        {
            fim = nullptr;
        }

        delete removido;
        return true;
    }

    void visualizarFila() const
    {
        if (vazia())
        {
            cout << "Fila de chamados vazia.\n";
            return;
        }

        cout << "Tickets pendentes:\n";
        NoTicket *atual = inicio;
        while (atual != nullptr)
        {
            cout << "ID: " << atual->dado.id << " | Nome: " << atual->dado.nome << "\n";
            atual = atual->proximo;
        }
    }
};

void limparEntrada()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void simularFilaImpressao()
{
    FilaImpressaoEstatica fila;
    int opcao = -1;

    while (opcao != 0)
    {
        cout << "\n=== FILA DE IMPRESSAO (ESTATICA) ===\n";
        cout << "1 - Adicionar arquivo na fila\n";
        cout << "2 - Imprimir proximo\n";
        cout << "0 - Voltar\n";
        cout << "Escolha: ";
        cin >> opcao;
        limparEntrada();

        if (opcao == 1)
        {
            string nomeArquivo;
            cout << "Nome do arquivo: ";
            getline(cin, nomeArquivo);

            if (fila.enfileirar(nomeArquivo))
            {
                cout << "Arquivo colocado na fila com sucesso.\n";
            }
            else
            {
                cout << "Fila cheia. Nao foi possivel adicionar o arquivo.\n";
            }
        }
        else if (opcao == 2)
        {
            string arquivo;
            if (fila.imprimirProximo(arquivo))
            {
                cout << "Imprimindo arquivo: " << arquivo << "\n";
            }
            else
            {
                cout << "Fila vazia. Nenhum arquivo para imprimir.\n";
            }
        }
        else if (opcao != 0)
        {
            cout << "Opcao invalida.\n";
        }
    }
}

void simularFilaChamados()
{
    FilaChamadosDinamica fila;
    int opcao = -1;

    while (opcao != 0)
    {
        cout << "\n=== FILA DE CHAMADOS (DINAMICA) ===\n";
        cout << "1 - Abrir novo ticket\n";
        cout << "2 - Atender proximo ticket\n";
        cout << "3 - Visualizar fila\n";
        cout << "0 - Voltar\n";
        cout << "Escolha: ";
        cin >> opcao;
        limparEntrada();

        if (opcao == 1)
        {
            Ticket ticket;

            cout << "ID do ticket: ";
            cin >> ticket.id;
            limparEntrada();

            cout << "Nome do solicitante: ";
            getline(cin, ticket.nome);

            if (fila.enfileirar(ticket))
            {
                cout << "Ticket aberto e colocado na fila.\n";
            }
            else
            {
                cout << "Falha ao alocar memoria para o ticket.\n";
            }
        }
        else if (opcao == 2)
        {
            Ticket ticket;
            if (fila.desenfileirar(ticket))
            {
                cout << "Atendendo ticket -> ID: " << ticket.id << " | Nome: " << ticket.nome << "\n";
                cout << "Ticket removido da fila e memoria liberada.\n";
            }
            else
            {
                cout << "Fila vazia. Nenhum ticket para atender.\n";
            }
        }
        else if (opcao == 3)
        {
            fila.visualizarFila();
        }
        else if (opcao != 0)
        {
            cout << "Opcao invalida.\n";
        }
    }
}

int main()
{
    int opcao = -1;

    while (opcao != 0)
    {
        cout << "\n========== MENU PRINCIPAL ==========\n";
        cout << "1 - Simular fila de impressao (estatica)\n";
        cout << "2 - Simular fila de chamados (dinamica)\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";
        cin >> opcao;
        limparEntrada();

        if (opcao == 1)
        {
            simularFilaImpressao();
        }
        else if (opcao == 2)
        {
            simularFilaChamados();
        }
        else if (opcao != 0)
        {
            cout << "Opcao invalida.\n";
        }
    }

    cout << "Programa encerrado.\n";
    return 0;
}
