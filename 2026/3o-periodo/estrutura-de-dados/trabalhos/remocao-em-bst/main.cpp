#include <iostream>
#include <iomanip>

using namespace std;

// Estrutura do nó da Árvore Binária de Busca
struct Node
{
    int valor;
    Node *esquerda;
    Node *direita;

    Node(int v) : valor(v), esquerda(nullptr), direita(nullptr) {}
};

// Classe da Árvore Binária de Busca
class BST
{
private:
    Node *raiz;

    Node *inserirRec(Node *node, int valor)
    {
        if (node == nullptr)
        {
            return new Node(valor);
        }

        if (valor < node->valor)
        {
            node->esquerda = inserirRec(node->esquerda, valor);
        }
        else if (valor > node->valor)
        {
            node->direita = inserirRec(node->direita, valor);
        }
        return node;
    }

    Node *buscarRec(Node *node, int valor)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (valor == node->valor)
        {
            return node;
        }
        else if (valor < node->valor)
        {
            return buscarRec(node->esquerda, valor);
        }
        else
        {
            return buscarRec(node->direita, valor);
        }
    }

    // Encontra o nó com menor valor (mais à esquerda)
    Node *encontrarMinimo(Node *node)
    {
        while (node && node->esquerda != nullptr)
        {
            node = node->esquerda;
        }
        return node;
    }

    Node *removerRec(Node *node, int valor)
    {
        if (node == nullptr)
            return nullptr;

        if (valor < node->valor)
            node->esquerda = removerRec(node->esquerda, valor);
        else if (valor > node->valor)
            node->direita = removerRec(node->direita, valor);
        else
        {
            // Caso 1: Nó folha (grau 0)
            if (node->esquerda == nullptr && node->direita == nullptr)
            {
                delete node;
                return nullptr;
            }
            // Caso 2: Um filho
            else if (node->esquerda == nullptr || node->direita == nullptr)
            {
                Node *filho = (node->esquerda != nullptr) ? node->esquerda : node->direita;
                delete node;
                return filho;
            }
            // Caso 3: Dois filhos
            else
            {
                Node *antecessor = encontrarMinimo(node->direita);
                node->valor = antecessor->valor;
                node->direita = removerRec(node->direita, antecessor->valor);
            }
        }
        return node;
    }

    // Caminhamento em-ordem (esquerda, nó, direita)
    void exibirInOrderRec(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }

        exibirInOrderRec(node->esquerda);
        cout << node->valor << " ";
        exibirInOrderRec(node->direita);
    }

    void liberarRec(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }

        liberarRec(node->esquerda);
        liberarRec(node->direita);
        delete node;
    }

public:
    BST() : raiz(nullptr) {}

    ~BST()
    {
        liberarRec(raiz);
    }

    // Inserir um valor na árvore
    void inserir(int valor)
    {
        raiz = inserirRec(raiz, valor);
    }

    // Buscar um valor na árvore
    bool buscar(int valor)
    {
        return buscarRec(raiz, valor) != nullptr;
    }

    // Remover um valor da árvore
    void remover(int valor)
    {
        raiz = removerRec(raiz, valor);
    }

    // Exibir a árvore em-ordem
    void exibirInOrder()
    {
        exibirInOrderRec(raiz);
        cout << endl;
    }
};

int main()
{
    BST arvore;
    int valores[] = {50, 30, 70, 20, 40, 60, 80, 15, 25, 35, 45};

    cout << "Construindo árvore com: 50, 30, 70, 20, 40, 60, 80, 15, 25, 35, 45" << endl;
    for (int v : valores)
        arvore.inserir(v);

    cout << "\n--- Estado inicial ---" << endl;
    cout << "In-order: ";
    arvore.exibirInOrder();

    cout << "\n--- Remover 15 ---" << endl;
    arvore.remover(15);
    cout << "In-order: ";
    arvore.exibirInOrder();

    cout << "\n--- Remover 20 ---" << endl;
    arvore.remover(20);
    cout << "In-order: ";
    arvore.exibirInOrder();

    cout << "\n--- Remover 30 ---" << endl;
    arvore.remover(30);
    cout << "In-order: ";
    arvore.exibirInOrder();

    cout << "\n--- Remover 70 ---" << endl;
    arvore.remover(70);
    cout << "In-order: ";
    arvore.exibirInOrder();

    cout << "\n--- Remover 50 ---" << endl;
    arvore.remover(50);
    cout << "In-order: ";
    arvore.exibirInOrder();

    return 0;
}
