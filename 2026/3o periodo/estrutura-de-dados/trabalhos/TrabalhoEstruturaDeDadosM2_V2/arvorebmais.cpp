#include "arvorebmais.h"

// ─── No ───────────────────────────────────────────────────────────────────────

No::No(bool eFolha, int grau)
    : folha(eFolha), proximo(nullptr), tamanhoChaves(0), tamanhoFilhos(0) {
    chaves = new int[2 * grau + 2];
    filhos = new No*[2 * grau + 3];
}

No::~No() {
    delete[] chaves;
    delete[] filhos;
}

// ─── ArvoreBMais ──────────────────────────────────────────────────────────────

ArvoreBMais::ArvoreBMais(int grau) : grau(grau) {
    raiz = new No(true, grau);
}

ArvoreBMais::~ArvoreBMais() {
    destruirArvore(raiz);
}

void ArvoreBMais::destruirArvore(No* no) {
    if (no == nullptr) return;
    if (!no->folha) {
        for (int i = 0; i < no->tamanhoFilhos; i++) {
            destruirArvore(no->filhos[i]);
        }
    }
    delete no;
}

bool ArvoreBMais::buscar(int chave) {
    No* atual = raiz;
    while (!atual->folha) {
        int i = 0;
        while (i < atual->tamanhoChaves) {
            if (chave < atual->chaves[i]) break;
            i++;
        }
        atual = atual->filhos[i];
    }

    int i = 0;
    while (i < atual->tamanhoChaves) {
        if (chave == atual->chaves[i]) return true;
        i++;
    }
    return false;
}

void ArvoreBMais::inserir(int chave) {
    No* atual = raiz;
    if (atual->tamanhoChaves == 2 * grau) {
        No* novaRaiz = new No(false, grau);
        raiz = novaRaiz;
        novaRaiz->filhos[0] = atual;
        novaRaiz->tamanhoFilhos = 1;
        dividir(novaRaiz, 0, atual);
        inserirNaoCheio(novaRaiz, chave);
    } else {
        inserirNaoCheio(atual, chave);
    }
}

void ArvoreBMais::inserirNaoCheio(No* atual, int chave) {
    int i = 0;
    while (i < atual->tamanhoChaves) {
        if (chave < atual->chaves[i]) break;
        i++;
    }

    if (atual->folha) {
        for (int j = atual->tamanhoChaves; j > i; j--) {
            atual->chaves[j] = atual->chaves[j - 1];
        }
        atual->chaves[i] = chave;
        atual->tamanhoChaves++;
    } else {
        if (atual->filhos[i]->tamanhoChaves == 2 * grau) {
            dividir(atual, i, atual->filhos[i]);
            if (chave > atual->chaves[i]) i++;
        }
        inserirNaoCheio(atual->filhos[i], chave);
    }
}

void ArvoreBMais::dividir(No* pai, int indice, No* no) {
    No* novoNo = new No(no->folha, grau);

    for (int j = pai->tamanhoFilhos; j > indice + 1; j--) {
        pai->filhos[j] = pai->filhos[j - 1];
    }
    pai->filhos[indice + 1] = novoNo;
    pai->tamanhoFilhos++;

    for (int j = pai->tamanhoChaves; j > indice; j--) {
        pai->chaves[j] = pai->chaves[j - 1];
    }
    pai->chaves[indice] = no->chaves[grau - 1];
    pai->tamanhoChaves++;

    int tamanhoChavesAntigo = no->tamanhoChaves;
    novoNo->tamanhoChaves = 0;
    for (int j = grau; j < tamanhoChavesAntigo; j++) {
        novoNo->chaves[novoNo->tamanhoChaves++] = no->chaves[j];
    }
    no->tamanhoChaves = grau - 1;

    if (!no->folha) {
        int tamanhoFilhosAntigo = no->tamanhoFilhos;
        novoNo->tamanhoFilhos = 0;
        for (int j = grau; j < tamanhoFilhosAntigo; j++) {
            novoNo->filhos[novoNo->tamanhoFilhos++] = no->filhos[j];
        }
        no->tamanhoFilhos = grau;
    }

    // CORRECAO 7: atualiza encadeamento de folhas (ponteiro proximo)
    if (no->folha) {
        novoNo->proximo = no->proximo;
        no->proximo = novoNo;
    }
}

void ArvoreBMais::roubarDaEsquerda(No* pai, int i) {
    No* no = pai->filhos[i];
    No* irmaoEsquerdo = pai->filhos[i - 1];

    for (int j = no->tamanhoChaves; j > 0; j--) {
        no->chaves[j] = no->chaves[j - 1];
    }
    no->chaves[0] = pai->chaves[i - 1];
    no->tamanhoChaves++;

    pai->chaves[i - 1] = irmaoEsquerdo->chaves[irmaoEsquerdo->tamanhoChaves - 1];
    // CORRECAO 2: remove a chave cedida do irmao esquerdo
    irmaoEsquerdo->tamanhoChaves--;

    if (!no->folha) {
        for (int j = no->tamanhoFilhos; j > 0; j--) {
            no->filhos[j] = no->filhos[j - 1];
        }
        no->filhos[0] = irmaoEsquerdo->filhos[irmaoEsquerdo->tamanhoFilhos - 1];
        no->tamanhoFilhos++;
        irmaoEsquerdo->tamanhoFilhos--;
    }
}

void ArvoreBMais::roubarDaDireita(No* pai, int i) {
    No* no = pai->filhos[i];
    No* irmaoDireito = pai->filhos[i + 1];

    no->chaves[no->tamanhoChaves++] = pai->chaves[i];
    pai->chaves[i] = irmaoDireito->chaves[0];

    // CORRECAO 3: remove a primeira chave do irmao direito (shift para esquerda)
    for (int j = 0; j < irmaoDireito->tamanhoChaves - 1; j++) {
        irmaoDireito->chaves[j] = irmaoDireito->chaves[j + 1];
    }
    irmaoDireito->tamanhoChaves--;

    if (!no->folha) {
        no->filhos[no->tamanhoFilhos++] = irmaoDireito->filhos[0];
        for (int j = 0; j < irmaoDireito->tamanhoFilhos - 1; j++) {
            irmaoDireito->filhos[j] = irmaoDireito->filhos[j + 1];
        }
        irmaoDireito->tamanhoFilhos--;
    }
}

// CORRECAO 6: remover agora usa removerEm que desce corretamente a arvore
void ArvoreBMais::remover(int chave) {
    removerEm(raiz, chave);

    // CORRECAO 5: libera raiz antiga quando a arvore encolhe um nivel
    if (!raiz->folha && raiz->tamanhoChaves == 0) {
        No* raizAntiga = raiz;
        raiz = raiz->filhos[0];
        delete raizAntiga;
    }
}

void ArvoreBMais::removerEm(No* no, int chave) {
    int i = 0;
    while (i < no->tamanhoChaves && chave > no->chaves[i]) i++;

    // caso base: folha — remove diretamente
    if (no->folha) {
        int j = 0;
        for (int k = 0; k < no->tamanhoChaves; k++) {
            if (no->chaves[k] != chave) no->chaves[j++] = no->chaves[k];
        }
        no->tamanhoChaves = j;
        return;
    }

    bool noSeparador = (i < no->tamanhoChaves && no->chaves[i] == chave);

    if (noSeparador) {
        // chave e um separador neste no interno: substituir por anterior ou sucessor
        No* anterior = no->filhos[i];
        No* sucessor = no->filhos[i + 1];
        if (anterior->tamanhoChaves >= grau) {
            int chaveAnterior = obterChaveMaxima(anterior);
            no->chaves[i] = chaveAnterior;
            removerEm(anterior, chaveAnterior);
        } else if (sucessor->tamanhoChaves >= grau) {
            int chaveSucessora = obterChaveMinima(sucessor);
            no->chaves[i] = chaveSucessora;
            removerEm(sucessor, chaveSucessora);
        } else {
            fundir(no, i, anterior, sucessor);
            removerEm(anterior, chave);
        }
        return;
    }

    // chave esta na sub-arvore filhos[i]: garante que o filho tem chaves
    // suficientes ANTES de descer (balanceamento proativo)
    if (no->filhos[i]->tamanhoChaves < grau) {
        if (i != 0 && no->filhos[i - 1]->tamanhoChaves >= grau) {
            roubarDaEsquerda(no, i);
        } else if (i != no->tamanhoChaves && no->filhos[i + 1]->tamanhoChaves >= grau) {
            roubarDaDireita(no, i);
        } else {
            if (i == no->tamanhoChaves) i--;
            fundir(no, i, no->filhos[i], no->filhos[i + 1]);
        }
        // recalcula i pois separadores podem ter mudado
        i = 0;
        while (i < no->tamanhoChaves && chave > no->chaves[i]) i++;
    }
    removerEm(no->filhos[i], chave);
}

int ArvoreBMais::obterChaveMinima(No* no) {
    while (!no->folha) no = no->filhos[0];
    return no->chaves[0];
}

int ArvoreBMais::obterChaveMaxima(No* no) {
    while (!no->folha) no = no->filhos[no->tamanhoFilhos - 1];
    return no->chaves[no->tamanhoChaves - 1];
}

No* ArvoreBMais::encontrarPai(No* filho) {
    No* atual = raiz;
    while (!atual->folha) {
        int i = 0;
        while (i < atual->tamanhoFilhos) {
            if (filho == atual->filhos[i]) return atual;
            else if (filho->chaves[0] < atual->filhos[i]->chaves[0]) break;
            i++;
        }
        atual = atual->filhos[i];
    }
    return nullptr;
}

void ArvoreBMais::fundir(No* pai, int indice, No* anterior, No* sucessor) {
    // CORRECAO 8: nos internos precisam da chave separadora do pai para
    // manter o roteamento correto entre os filhos fundidos
    if (!anterior->folha) {
        anterior->chaves[anterior->tamanhoChaves++] = pai->chaves[indice];
    }

    for (int j = 0; j < sucessor->tamanhoChaves; j++) {
        anterior->chaves[anterior->tamanhoChaves++] = sucessor->chaves[j];
    }

    for (int j = 0; j < sucessor->tamanhoFilhos; j++) {
        anterior->filhos[anterior->tamanhoFilhos++] = sucessor->filhos[j];
    }

    // mantem o encadeamento de folhas ao fundir dois nos folha
    if (anterior->folha) {
        anterior->proximo = sucessor->proximo;
    }

    for (int j = indice + 1; j < pai->tamanhoFilhos - 1; j++) {
        pai->filhos[j] = pai->filhos[j + 1];
    }
    pai->tamanhoFilhos--;

    for (int j = indice; j < pai->tamanhoChaves - 1; j++) {
        pai->chaves[j] = pai->chaves[j + 1];
    }
    pai->tamanhoChaves--;

    if (pai == raiz && pai->tamanhoChaves == 0) {
        raiz = anterior;
    }

    delete sucessor;
}

void ArvoreBMais::rotacionarDireita(No* pai, int indice) {
    No* no = pai->filhos[indice];
    No* irmaoEsquerdo = pai->filhos[indice - 1];

    for (int j = no->tamanhoChaves; j > 0; j--) {
        no->chaves[j] = no->chaves[j - 1];
    }
    no->chaves[0] = pai->chaves[indice - 1];
    no->tamanhoChaves++;

    pai->chaves[indice - 1] = irmaoEsquerdo->chaves[irmaoEsquerdo->tamanhoChaves - 1];
    // CORRECAO 4: remove a chave cedida do irmao esquerdo
    irmaoEsquerdo->tamanhoChaves--;

    if (!no->folha) {
        for (int j = no->tamanhoFilhos; j > 0; j--) {
            no->filhos[j] = no->filhos[j - 1];
        }
        no->filhos[0] = irmaoEsquerdo->filhos[irmaoEsquerdo->tamanhoFilhos - 1];
        no->tamanhoFilhos++;
        irmaoEsquerdo->tamanhoFilhos--;
    }
}

void ArvoreBMais::rotacionarEsquerda(No* pai, int indice) {
    No* no = pai->filhos[indice];
    No* irmaoDireito = pai->filhos[indice + 1];

    no->chaves[no->tamanhoChaves++] = pai->chaves[indice];
    pai->chaves[indice] = irmaoDireito->chaves[0];

    // CORRECAO 1: remove a primeira chave do irmao direito (shift para esquerda)
    for (int j = 0; j < irmaoDireito->tamanhoChaves - 1; j++) {
        irmaoDireito->chaves[j] = irmaoDireito->chaves[j + 1];
    }
    irmaoDireito->tamanhoChaves--;

    if (!no->folha) {
        no->filhos[no->tamanhoFilhos++] = irmaoDireito->filhos[0];
        for (int j = 0; j < irmaoDireito->tamanhoFilhos - 1; j++) {
            irmaoDireito->filhos[j] = irmaoDireito->filhos[j + 1];
        }
        irmaoDireito->tamanhoFilhos--;
    }
}

void ArvoreBMais::imprimirArvore() {
    int maxNos = 1000;
    No** nivelAtual  = new No*[maxNos];
    int tamanhoAtual = 0;
    No** proximoNivel  = new No*[maxNos];
    int tamanhoProximo = 0;

    nivelAtual[tamanhoAtual++] = raiz;

    while (tamanhoAtual > 0) {
        tamanhoProximo = 0;

        for (int n = 0; n < tamanhoAtual; n++) {
            No* no = nivelAtual[n];
            cout << "[";
            for (int i = 0; i < no->tamanhoChaves; ++i) {
                cout << no->chaves[i];
                if (i < no->tamanhoChaves - 1) cout << ", ";
            }
            cout << "] ";

            if (!no->folha) {
                for (int i = 0; i < no->tamanhoFilhos; i++) {
                    proximoNivel[tamanhoProximo++] = no->filhos[i];
                }
            }
        }

        cout << endl;

        No** temporario = nivelAtual;
        nivelAtual      = proximoNivel;
        proximoNivel    = temporario;
        tamanhoAtual    = tamanhoProximo;
    }

    delete[] nivelAtual;
    delete[] proximoNivel;
}
