/*
 * ARVORE B+ (B+ Tree)
 *
 * Uma arvore B+ e uma estrutura de dados em arvore balanceada onde:
 *   - Todos os dados ficam nas FOLHAS (nos sem filhos).
 *   - Os nos internos guardam apenas chaves separadoras que guiam a busca.
 *   - Cada no pode ter no maximo 2*degree chaves e 2*degree+1 filhos.
 *   - Cada no (exceto a raiz) deve ter pelo menos degree-1 chaves.
 *   - As folhas nao tem ponteiros de filho (values vazio).
 *
 * Estrutura de um no interno com degree=2 (max 4 chaves, 5 filhos):
 *
 *         [ 10 | 20 | 30 ]
 *        /    |    |    \
 *     [...]  [...] [...] [...]
 *
 * Estrutura de uma folha (todos os dados vivem aqui):
 *         [ 1 | 2 | 3 ]  ->  [ 4 | 5 | 6 ]  ->  ...
 *                         (ponteiro next liga as folhas em ordem)
 */

#include <iostream>
using namespace std;

// =============================================================================
// CLASSE NODE
// Representa um no da arvore, podendo ser interno (guia a busca) ou folha
// (armazena as chaves de dados).
// =============================================================================
class Node
{
public:
    int *keys;      // array de chaves armazenadas neste no
    int keysSize;   // quantidade atual de chaves no array
    Node **values;  // array de ponteiros para nos filhos (so usado em nos internos)
    int valuesSize; // quantidade atual de filhos no array
    bool leaf;      // true se este no e uma folha (sem filhos)
    Node *next;     // ponteiro para o proximo no folha (encadeamento horizontal)

    // Construtor: aloca os arrays com capacidade maxima de 2*degree+2 chaves
    // e 2*degree+3 filhos (margem extra para evitar overflow durante splits).
    Node(bool isLeaf, int degree) : leaf(isLeaf), next(nullptr), keysSize(0), valuesSize(0)
    {
        keys = new int[2 * degree + 2];
        values = new Node *[2 * degree + 3];
    }

    // Destrutor: libera apenas os arrays deste no.
    // Os nos filhos apontados por values sao liberados pelo destroyTree da arvore.
    ~Node()
    {
        delete[] keys;
        delete[] values;
    }
};

// =============================================================================
// CLASSE BPLUSTREE
// Gerencia a raiz e o grau da arvore, expondo as operacoes publicas.
// =============================================================================
class BPlusTree
{
private:
    Node *root; // ponteiro para a raiz da arvore
    int degree; // grau da arvore: define o numero maximo de chaves por no (2*degree)

    // -------------------------------------------------------------------------
    // destroyTree
    // Percorre a arvore em pos-ordem e libera cada no com delete.
    // Pos-ordem garante que os filhos sao deletados antes do pai, evitando
    // ponteiros pendentes.
    // -------------------------------------------------------------------------
    void destroyTree(Node *node)
    {
        if (node == nullptr)
            return;
        // Recursao: desce para todos os filhos antes de deletar este no
        if (!node->leaf)
        {
            for (int i = 0; i < node->valuesSize; i++)
            {
                destroyTree(node->values[i]);
            }
        }
        // Deleta este no (chama ~Node, liberando keys[] e values[])
        delete node;
    }

    // -------------------------------------------------------------------------
    // removeAt  (CORRECAO 6)
    //
    // Remove `key` da sub-arvore com raiz em `node`, descendo corretamente
    // nivel a nivel em vez de reiniciar da raiz a cada passo.
    //
    // Estrategia: balanceamento PROATIVO — antes de descer para um filho,
    // garante que ele tem >= degree chaves (steal ou merge). Assim a remocao
    // na folha nunca viola o invariante de minimo, eliminando a necessidade
    // de rebalancear no caminho de volta.
    //
    // Caso especial: se a chave e um SEPARADOR neste no interno (atSeparator),
    // substitui pelo predecessor (max da esquerda) ou sucessor (min da direita)
    // e remove a chave real da sub-arvore correspondente.
    // -------------------------------------------------------------------------
    void removeAt(Node *node, int key)
    {
        int i = 0;
        while (i < node->keysSize && key > node->keys[i])
            i++;

        // Caso base: folha — remove diretamente por compactacao do array
        if (node->leaf)
        {
            int j = 0;
            for (int k = 0; k < node->keysSize; k++)
            {
                if (node->keys[k] != key)
                    node->keys[j++] = node->keys[k];
            }
            node->keysSize = j;
            return;
        }

        // Verifica se key e um separador neste no interno
        bool atSeparator = (i < node->keysSize && node->keys[i] == key);

        if (atSeparator)
        {
            // Chave e separador: substitui por predecessora ou sucessora
            Node *pred = node->values[i];
            Node *succ = node->values[i + 1];
            if (pred->keysSize >= degree)
            {
                // Predecessora: maior chave da sub-arvore esquerda
                int predKey = getMaxKey(pred);
                node->keys[i] = predKey;
                removeAt(pred, predKey);
            }
            else if (succ->keysSize >= degree)
            {
                // Sucessora: menor chave da sub-arvore direita
                int succKey = getMinKey(succ);
                node->keys[i] = succKey;
                removeAt(succ, succKey);
            }
            else
            {
                // Ambos com poucas chaves: funde e remove da fusao
                merge(node, i, pred, succ);
                removeAt(pred, key);
            }
            return;
        }

        // Chave esta em values[i]: balanceamento proativo antes de descer
        if (node->values[i]->keysSize < degree)
        {
            if (i != 0 && node->values[i - 1]->keysSize >= degree)
            {
                stealFromLeft(node, i);
            }
            else if (i != node->keysSize && node->values[i + 1]->keysSize >= degree)
            {
                stealFromRight(node, i);
            }
            else
            {
                if (i == node->keysSize)
                    i--;
                merge(node, i, node->values[i], node->values[i + 1]);
            }
            // Recalcula i pois separadores podem ter mudado apos steal/merge
            i = 0;
            while (i < node->keysSize && key > node->keys[i])
                i++;
        }
        removeAt(node->values[i], key); // desce para o filho correto
    }

public:
    // Construtor: cria a arvore com uma raiz folha vazia.
    // Uma arvore B+ comeca sempre com uma unica folha vazia como raiz.
    BPlusTree(int degree) : degree(degree)
    {
        root = new Node(true, degree);
    }

    // Destrutor: chama destroyTree para liberar toda a memoria alocada.
    ~BPlusTree()
    {
        destroyTree(root);
    }

    // =========================================================================
    // BUSCA (search)
    //
    // Desce pelos nos internos usando as chaves separadoras como guia, depois
    // varre a folha procurando a chave exata.
    //
    // Nos internos: keys[i] e o menor valor que pode estar no filho values[i+1].
    // Para cada chave do no, se key < keys[i], descemos por values[i].
    // Se key >= todas as chaves, descemos pelo ultimo filho.
    //
    // Complexidade: O(log n)
    // =========================================================================
    bool search(int key)
    {
        Node *current = root;

        // Fase 1: desce pelos nos internos ate chegar numa folha
        while (!current->leaf)
        {
            int i = 0;
            // Encontra o primeiro indice i onde key < keys[i]
            // Ou seja: key pertence ao filho values[i]
            while (i < current->keysSize)
            {
                if (key < current->keys[i])
                {
                    break;
                }
                i += 1;
            }
            current = current->values[i]; // desce para o filho correto
        }

        // Fase 2: varre a folha linearmente procurando a chave exata
        int i = 0;
        while (i < current->keysSize)
        {
            if (key == current->keys[i])
            {
                return true; // chave encontrada
            }
            i += 1;
        }
        return false; // chave nao existe na arvore
    }

    // =========================================================================
    // INSERCAO (insert)
    //
    // Caso especial: se a raiz ja esta cheia (2*degree chaves), ela precisa ser
    // dividida antes de inserir. Cria-se uma nova raiz interna vazia, a antiga
    // raiz vira seu primeiro filho, e entao split e chamado para dividi-la.
    //
    // Caso normal: delega para insertNonFull que desce ate a folha correta.
    // =========================================================================
    void insert(int key)
    {
        Node *current = root;

        // Raiz cheia: precisa crescer para cima
        if (current->keysSize == 2 * degree)
        {
            Node *newRoot = new Node(false, degree); // nova raiz interna vazia
            root = newRoot;
            newRoot->values[0] = current; // antiga raiz vira primeiro filho
            newRoot->valuesSize = 1;
            split(newRoot, 0, current);  // divide a antiga raiz ao meio
            insertNonFull(newRoot, key); // insere a partir da nova raiz
        }
        else
        {
            insertNonFull(current, key); // raiz nao esta cheia, insere diretamente
        }
    }

    // =========================================================================
    // INSERCAO EM NO NAO CHEIO (insertNonFull)
    //
    // Desce recursivamente ate a folha correta para inserir a chave.
    // Se no caminho encontrar um filho cheio, divide-o primeiro (split proativo)
    // para garantir que sempre ha espaco ao descer.
    // =========================================================================
    void insertNonFull(Node *current, int key)
    {
        // Encontra a posicao i onde key deve ir (primeiro keys[i] > key)
        int i = 0;
        while (i < current->keysSize)
        {
            if (key < current->keys[i])
            {
                break;
            }
            i += 1;
        }

        if (current->leaf)
        {
            // Folha: insere key na posicao i fazendo shift dos elementos maiores
            // Antes: [1, 3, 5]  inserindo 2 na posicao i=1
            // Shift:  [1, 3, 3, 5] -> preenche com 2 -> [1, 2, 3, 5]
            for (int j = current->keysSize; j > i; j--)
            {
                current->keys[j] = current->keys[j - 1]; // desloca para direita
            }
            current->keys[i] = key;
            current->keysSize++;
        }
        else
        {
            // No interno: verifica se o filho values[i] esta cheio
            if (current->values[i]->keysSize == 2 * degree)
            {
                // Filho cheio: divide-o antes de descer
                split(current, i, current->values[i]);
                // Apos o split, current->keys[i] e a chave mediana que subiu.
                // Decide qual dos dois novos filhos seguir:
                if (key > current->keys[i])
                {
                    i += 1; // chave maior que a mediana: vai para o filho direito
                }
            }
            insertNonFull(current->values[i], key); // desce recursivamente
        }
    }

    // =========================================================================
    // DIVISAO DE NO (split)
    //
    // Divide o no cheio `node` (filho de `parent` na posicao `index`) ao meio.
    // A chave mediana (keys[degree-1]) sobe para o pai.
    // O no original fica com as chaves menores (indices 0..degree-2).
    // Um novo no e criado com as chaves maiores (indices degree..2*degree-1).
    //
    // Antes do split (degree=2, node cheio com 4 chaves):
    //   parent->keys: [10, 30]     parent->values: [A, node, C]
    //   node->keys:   [12, 15, 20, 25]
    //
    // Depois do split (mediana = keys[1] = 15 sobe para o pai):
    //   parent->keys: [10, 15, 30]    parent->values: [A, node, newNode, C]
    //   node->keys:   [12]            (degree-1 = 1 chave)
    //   newNode->keys:[20, 25]        (degree = 2 chaves)
    // =========================================================================
    void split(Node *parent, int index, Node *node)
    {
        Node *newNode = new Node(node->leaf, degree); // novo no recebe metade direita

        // Abre espaco em parent->values na posicao index+1 (shift para direita)
        for (int j = parent->valuesSize; j > index + 1; j--)
        {
            parent->values[j] = parent->values[j - 1];
        }
        parent->values[index + 1] = newNode; // insere novo filho no pai
        parent->valuesSize++;

        // Abre espaco em parent->keys na posicao index (shift para direita)
        for (int j = parent->keysSize; j > index; j--)
        {
            parent->keys[j] = parent->keys[j - 1];
        }
        parent->keys[index] = node->keys[degree - 1]; // mediana sobe para o pai
        parent->keysSize++;

        // Copia a metade direita das chaves (indices degree..fim) para newNode
        int oldKeysSize = node->keysSize; // salva o tamanho antes de modificar
        newNode->keysSize = 0;
        for (int j = degree; j < oldKeysSize; j++)
        {
            newNode->keys[newNode->keysSize++] = node->keys[j];
        }
        node->keysSize = degree - 1; // node fica com as (degree-1) chaves da esquerda

        // Para nos internos: distribui tambem os ponteiros de filho
        if (!node->leaf)
        {
            int oldValuesSize = node->valuesSize; // salva antes de modificar
            newNode->valuesSize = 0;
            // Copia os filhos da metade direita (indices degree..fim) para newNode
            for (int j = degree; j < oldValuesSize; j++)
            {
                newNode->values[newNode->valuesSize++] = node->values[j];
            }
            node->valuesSize = degree; // node fica com os degree filhos da esquerda
        }
        // CORRECAO 7: atualiza o encadeamento de folhas (ponteiro next)
        // Folhas formam uma lista ligada: node->next->... deve incluir newNode entre eles
        if (node->leaf)
        {
            newNode->next = node->next; // newNode aponta para quem node apontava
            node->next = newNode;       // node passa a apontar para newNode
        }
    }

    // =========================================================================
    // ROUBAR CHAVE DO IRMAO ESQUERDO (stealFromLeft)
    //
    // Usado durante a remocao quando um no ficou com poucos elementos mas o
    // irmao esquerdo tem elementos suficientes para ceder um.
    //
    // Operacao:
    //   1. A chave separadora do pai (pai->keys[i-1]) desce para o inicio de node.
    //   2. A ultima chave do irmao esquerdo sobe para o pai como novo separador.
    //   3. Para nos internos: o ultimo filho do irmao esquerdo migra para node.
    //
    // Exemplo (degree=2):
    //   pai->keys: [15]    esquerdo->keys: [5, 10]    node->keys: [20]
    //   Resultado: pai->keys: [10]    esquerdo->keys: [5]    node->keys: [15, 20]
    // =========================================================================
    void stealFromLeft(Node *parent, int i)
    {
        Node *node = parent->values[i];
        Node *leftSibling = parent->values[i - 1];

        // Shift de todos os elementos de node uma posicao para a direita
        // para abrir espaco na posicao 0
        for (int j = node->keysSize; j > 0; j--)
        {
            node->keys[j] = node->keys[j - 1];
        }
        // A chave separadora do pai desce para o inicio de node
        node->keys[0] = parent->keys[i - 1];
        node->keysSize++;

        // A ultima chave do irmao esquerdo sobe para o pai como novo separador
        parent->keys[i - 1] = leftSibling->keys[leftSibling->keysSize - 1];
        leftSibling->keysSize--; // CORRECAO 2: remove fisicamente a chave cedida

        // Para nos internos: move o ultimo filho do irmao para o inicio de node
        if (!node->leaf)
        {
            for (int j = node->valuesSize; j > 0; j--)
            {
                node->values[j] = node->values[j - 1]; // abre espaco em values[0]
            }
            node->values[0] = leftSibling->values[leftSibling->valuesSize - 1];
            node->valuesSize++;
            leftSibling->valuesSize--; // remove o ultimo filho do irmao
        }
    }

    // =========================================================================
    // ROUBAR CHAVE DO IRMAO DIREITO (stealFromRight)
    //
    // Simetrico ao stealFromLeft: cede um elemento do irmao direito para node.
    //
    // Operacao:
    //   1. A chave separadora do pai (pai->keys[i]) vai para o final de node.
    //   2. A primeira chave do irmao direito sobe para o pai como novo separador.
    //   3. Para nos internos: o primeiro filho do irmao direito migra para node.
    //
    // Exemplo (degree=2):
    //   pai->keys: [15]    node->keys: [10]    direito->keys: [20, 25]
    //   Resultado: pai->keys: [20]    node->keys: [10, 15]    direito->keys: [20, 25]
    //   Nota: a primeira chave do irmao (20) permanece no array dele (sem remocao).
    // =========================================================================
    void stealFromRight(Node *parent, int i)
    {
        Node *node = parent->values[i];
        Node *rightSibling = parent->values[i + 1];

        // A chave separadora do pai vai para o final de node
        node->keys[node->keysSize++] = parent->keys[i];

        // A primeira chave do irmao direito sobe para o pai como novo separador
        parent->keys[i] = rightSibling->keys[0];
        // CORRECAO 3: remove fisicamente a primeira chave do irmao (shift para esquerda)
        for (int j = 0; j < rightSibling->keysSize - 1; j++)
        {
            rightSibling->keys[j] = rightSibling->keys[j + 1];
        }
        rightSibling->keysSize--;

        // Para nos internos: move o primeiro filho do irmao para o final de node
        if (!node->leaf)
        {
            node->values[node->valuesSize++] = rightSibling->values[0];

            // Remove o primeiro filho do irmao (shift para esquerda em values)
            for (int j = 0; j < rightSibling->valuesSize - 1; j++)
            {
                rightSibling->values[j] = rightSibling->values[j + 1];
            }
            rightSibling->valuesSize--;
        }
    }

    // =========================================================================
    // REMOCAO (remove)  — CORRECAO 6
    //
    // Delega para removeAt (metodo privado) que desce corretamente a arvore
    // nivel a nivel com balanceamento proativo, eliminando a necessidade de
    // reiniciar da raiz e o risco de recursao infinita.
    //
    // Apos a remocao, verifica se a raiz ficou sem chaves (pode ocorrer quando
    // merge esvazia o unico no interno): nesse caso a arvore encolhe um nivel,
    // o unico filho vira a nova raiz e a raiz antiga e liberada da memoria.
    // =========================================================================
    void remove(int key)
    {
        removeAt(root, key);

        // CORRECAO 5: libera a raiz antiga quando a arvore encolhe um nivel
        // (sem este delete havia vazamento de memoria)
        if (!root->leaf && root->keysSize == 0)
        {
            Node *oldRoot = root;
            root = root->values[0];
            delete oldRoot;
        }
    }

    // =========================================================================
    // MENOR CHAVE (getMinKey)
    // Desce sempre pelo primeiro filho ate chegar numa folha e retorna keys[0].
    // =========================================================================
    int getMinKey(Node *node)
    {
        while (!node->leaf)
        {
            node = node->values[0]; // sempre vai para o filho mais a esquerda
        }
        return node->keys[0]; // primeira chave da folha mais a esquerda
    }

    // =========================================================================
    // MAIOR CHAVE (getMaxKey)
    // Desce sempre pelo ultimo filho ate chegar numa folha e retorna a ultima chave.
    // =========================================================================
    int getMaxKey(Node *node)
    {
        while (!node->leaf)
        {
            node = node->values[node->valuesSize - 1]; // sempre vai para o ultimo filho
        }
        return node->keys[node->keysSize - 1]; // ultima chave da folha mais a direita
    }

    // =========================================================================
    // ENCONTRAR PAI (findParent)
    //
    // Percorre a arvore da raiz em direcao a `child`, retornando o no que
    // contem `child` em seu array de filhos (values).
    //
    // A busca usa a primeira chave de `child` como guia para navegar pelos
    // nos internos, parando quando encontra `child` diretamente em values[i].
    // =========================================================================
    Node *findParent(Node *child)
    {
        Node *current = root;
        while (!current->leaf)
        {
            int i = 0;
            while (i < current->valuesSize)
            {
                if (child == current->values[i])
                {
                    return current; // encontrou: current e o pai de child
                }
                else if (child->keys[0] < current->values[i]->keys[0])
                {
                    break; // guia pela primeira chave de child
                }
                i += 1;
            }
            current = current->values[i]; // desce para o filho na direcao correta
        }
        return nullptr; // child e a raiz, nao tem pai
    }

    // =========================================================================
    // FUSAO DE NOS (merge)
    //
    // Combina dois irmaos adjacentes (`pred` e `succ`) em um unico no,
    // removendo `succ` do pai. Usado quando ambos tem poucos elementos.
    //
    // Operacao:
    //   1. Todas as chaves de succ sao copiadas para o final de pred.
    //   2. Todos os filhos de succ (se no interno) vao para o final de pred.
    //   3. O ponteiro para succ e removido de parent->values (shift esquerda).
    //   4. A chave separadora entre pred e succ e removida de parent->keys.
    //   5. succ e deletado (sua memoria e liberada).
    //
    // Antes do merge (index=0):
    //   parent->keys: [15, 30]    parent->values: [pred=[5,10], succ=[20], C]
    // Depois:
    //   parent->keys: [30]        parent->values: [pred=[5,10,20], C]
    //   succ deletado.
    // =========================================================================
    void merge(Node *parent, int index, Node *pred, Node *succ)
    {
        // CORRECAO 8: para nos internos, a chave separadora do pai deve descer
        // para pred antes das chaves de succ. Sem isso, a fusao de nos internos
        // em arvores de 3+ niveis perde a chave que separa os filhos de pred dos
        // filhos de succ, tornando algumas sub-arvores inacessiveis.
        if (!pred->leaf)
        {
            pred->keys[pred->keysSize++] = parent->keys[index];
        }

        // Copia todas as chaves de succ para o final de pred
        for (int j = 0; j < succ->keysSize; j++)
        {
            pred->keys[pred->keysSize++] = succ->keys[j];
        }

        // Copia todos os filhos de succ para o final de pred (so para nos internos)
        for (int j = 0; j < succ->valuesSize; j++)
        {
            pred->values[pred->valuesSize++] = succ->values[j];
        }

        // Mantém o encadeamento de folhas ao fundir dois nos folha
        if (pred->leaf)
        {
            pred->next = succ->next; // pred passa a apontar para o sucessor de succ
        }

        // Remove succ de parent->values (shift esquerda a partir de index+1)
        for (int j = index + 1; j < parent->valuesSize - 1; j++)
        {
            parent->values[j] = parent->values[j + 1];
        }
        parent->valuesSize--;

        // Remove a chave separadora de parent->keys (shift esquerda a partir de index)
        for (int j = index; j < parent->keysSize - 1; j++)
        {
            parent->keys[j] = parent->keys[j + 1];
        }
        parent->keysSize--;

        // Se a raiz ficou sem chaves apos o merge, o filho unico vira a nova raiz
        if (parent == root && parent->keysSize == 0)
        {
            root = pred;
        }

        delete succ; // libera a memoria de succ (seus filhos ja migraram para pred)
    }

    // =========================================================================
    // ROTACAO DIREITA (rotateRight)
    //
    // Move a ultima chave do irmao esquerdo (prev) para node, passando pela
    // chave separadora do pai como intermediaria. Equivale ao stealFromLeft
    // mas chamado apos a remocao de uma folha, via removeFromLeaf.
    //
    // Fluxo:
    //   pai->keys[index-1] desce para o inicio de node.
    //   ultima chave de prev sobe para pai->keys[index-1].
    //   Para nos internos: ultimo filho de prev migra para o inicio de node.
    // =========================================================================
    void rotateRight(Node *parent, int index)
    {
        Node *node = parent->values[index];
        Node *prev = parent->values[index - 1]; // irmao esquerdo

        // Abre espaco no inicio de node->keys (shift para direita)
        for (int j = node->keysSize; j > 0; j--)
        {
            node->keys[j] = node->keys[j - 1];
        }
        // A chave separadora do pai desce para o inicio de node
        node->keys[0] = parent->keys[index - 1];
        node->keysSize++;

        // A ultima chave de prev sobe para o pai como novo separador
        parent->keys[index - 1] = prev->keys[prev->keysSize - 1];
        prev->keysSize--; // CORRECAO 4: remove fisicamente a chave cedida do irmao esquerdo

        // Para nos internos: move o ultimo filho de prev para o inicio de node
        if (!node->leaf)
        {
            for (int j = node->valuesSize; j > 0; j--)
            {
                node->values[j] = node->values[j - 1]; // abre espaco em values[0]
            }
            node->values[0] = prev->values[prev->valuesSize - 1];
            node->valuesSize++;
            prev->valuesSize--;
        }
    }

    // =========================================================================
    // ROTACAO ESQUERDA (rotateLeft)
    //
    // Move a primeira chave do irmao direito (next) para node, passando pela
    // chave separadora do pai. Simetrico ao rotateRight.
    //
    // Fluxo:
    //   pai->keys[index] desce para o final de node.
    //   primeira chave de next sobe para pai->keys[index].
    //   Para nos internos: primeiro filho de next migra para o final de node.
    // =========================================================================
    void rotateLeft(Node *parent, int index)
    {
        Node *node = parent->values[index];
        Node *next = parent->values[index + 1]; // irmao direito

        // A chave separadora do pai vai para o final de node
        node->keys[node->keysSize++] = parent->keys[index];

        // A primeira chave de next sobe para o pai como novo separador
        parent->keys[index] = next->keys[0];
        // CORRECAO 1: remove fisicamente a primeira chave de next (shift para esquerda)
        for (int j = 0; j < next->keysSize - 1; j++)
        {
            next->keys[j] = next->keys[j + 1];
        }
        next->keysSize--;

        // Para nos internos: move o primeiro filho de next para o final de node
        if (!node->leaf)
        {
            node->values[node->valuesSize++] = next->values[0];

            // Remove o primeiro filho de next (shift para esquerda)
            for (int j = 0; j < next->valuesSize - 1; j++)
            {
                next->values[j] = next->values[j + 1];
            }
            next->valuesSize--;
        }
    }

    // =========================================================================
    // IMPRESSAO DA ARVORE (printTree)
    //
    // Realiza um BFS (busca em largura) nivel a nivel, imprimindo as chaves
    // de cada no entre colchetes. Usa dois arrays manuais como fila:
    //   - currentLevel: nos do nivel sendo processado
    //   - nextLevel:    nos do proximo nivel (filhos dos atuais)
    //
    // A cada iteracao do while externo, um nivel completo e impresso.
    // Os ponteiros dos arrays sao trocados (swap) para reusar a memoria.
    // =========================================================================
    void printTree()
    {
        int maxNodes = 1000; // capacidade maxima dos arrays de nivel
        Node **currentLevel = new Node *[maxNodes];
        int currentSize = 0;
        Node **nextLevel = new Node *[maxNodes];
        int nextSize = 0;

        currentLevel[currentSize++] = root; // começa com a raiz no nivel atual

        while (currentSize > 0)
        {
            nextSize = 0; // reseta o nivel seguinte

            // Processa cada no do nivel atual
            for (int n = 0; n < currentSize; n++)
            {
                Node *node = currentLevel[n];

                // Imprime as chaves do no no formato [k1, k2, ...]
                cout << "[";
                for (int i = 0; i < node->keysSize; ++i)
                {
                    cout << node->keys[i];
                    if (i < node->keysSize - 1)
                    {
                        cout << ", ";
                    }
                }
                cout << "] ";

                // Se no interno: enfileira todos os seus filhos para o proximo nivel
                if (!node->leaf)
                {
                    for (int i = 0; i < node->valuesSize; i++)
                    {
                        nextLevel[nextSize++] = node->values[i];
                    }
                }
            }

            cout << endl; // quebra de linha ao terminar o nivel

            // Troca os ponteiros: nextLevel vira o novo currentLevel
            // (sem copiar dados, apenas reaponta os ponteiros)
            Node **temp = currentLevel;
            currentLevel = nextLevel;
            nextLevel = temp;
            currentSize = nextSize;
        }

        delete[] currentLevel;
        delete[] nextLevel;
    }
};

// =============================================================================
// MAIN
// Demonstra a criacao, insercao, impressao e remocao na arvore B+.
// =============================================================================
int main()
{
    // Cria uma arvore B+ com degree=3 (maximo de 6 chaves por no)
    BPlusTree tree(3);

    // Insere as chaves 1 a 9 em ordem crescente
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(8);
    tree.insert(9);

    // Imprime a arvore nivel a nivel
    // Saida esperada:
    //   [3]
    //   [1, 2] [4, 5, 6, 7, 8, 9]
    tree.printTree();

    // Remove a chave 3 da arvore
    tree.remove(3);

    // Imprime a arvore apos a remocao
    // Saida esperada:
    //   [4]
    //   [1, 2] [5, 6, 7, 8, 9]
    tree.printTree();

    return 0;
}
