#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// Cada nó guarda o valor, a altura atual e os ponteiros para os filhos.
struct No {
	int valor;
	int altura;
	No* esquerda;
	No* direita;

	explicit No(int v)
		: valor(v), altura(1), esquerda(nullptr), direita(nullptr) {}
};

// Em uma AVL, altura de ponteiro nulo é 0.
int altura(No* no) {
	return no == nullptr ? 0 : no->altura;
}

// Aqui usamos max para descobrir qual lado da árvore ficou mais alto.
int maior(int a, int b) {
	return max(a, b);
}

// Recalcula a altura de um nó a partir das alturas dos filhos.
void atualizarAltura(No* no) {
	if (no != nullptr) {
		no->altura = 1 + maior(altura(no->esquerda), altura(no->direita));
	}
}

// Rotação simples à direita:
// - o filho esquerdo sobe
// - a raiz antiga desce para a direita
// - a subárvore que estava entre eles continua existindo, mas muda de lado
No* rotacaoSimplesDireita(No* raiz) {
	if (raiz == nullptr || raiz->esquerda == nullptr) {
		return raiz;
	}

	No* novaRaiz = raiz->esquerda;
	No* subarvoreDireitaNovaRaiz = novaRaiz->direita;

	// Reencaixa os ponteiros para fazer a troca de posições.
	novaRaiz->direita = raiz;
	raiz->esquerda = subarvoreDireitaNovaRaiz;

	// Depois da rotação, as alturas precisam ser recalculadas.
	atualizarAltura(raiz);
	atualizarAltura(novaRaiz);

	return novaRaiz;
}

// Impressão em pré-ordem: raiz, esquerda, direita.
void imprimirPreOrdem(No* no) {
	if (no == nullptr) {
		return;
	}

	cout << no->valor << ' ';
	imprimirPreOrdem(no->esquerda);
	imprimirPreOrdem(no->direita);
}















// Mostra a árvore por níveis para facilitar a leitura no terminal.
void imprimirArvore(No* raiz) {
	if (raiz == nullptr) {
		return;
	}

	queue<No*> fila;
	fila.push(raiz);
	int nivel = 0;

	while (!fila.empty()) {
		int quantidade = static_cast<int>(fila.size());
		vector<No*> proximos;

		cout << "Nivel " << nivel << ": ";

		for (int i = 0; i < quantidade; ++i) {
			No* atual = fila.front();
			fila.pop();

			cout << atual->valor << "(h=" << atual->altura << ") ";

			if (atual->esquerda != nullptr) {
				proximos.push_back(atual->esquerda);
			}
			if (atual->direita != nullptr) {
				proximos.push_back(atual->direita);
			}
		}

		cout << '\n';

		for (No* proximo : proximos) {
			fila.push(proximo);
		}

		++nivel;
	}
}

// Libera a memória alocada na árvore inteira.
void liberar(No* no) {
	if (no == nullptr) {
		return;
	}

	liberar(no->esquerda);
	liberar(no->direita);
	delete no;
}













int main() {
	// Exemplo propositalmente mais cheio para deixar a rotação fácil de enxergar.
	No* raiz = new No(50);
	raiz->esquerda = new No(30);
	raiz->direita = new No(80);
	raiz->esquerda->esquerda = new No(20);
	raiz->esquerda->direita = new No(40);
	raiz->esquerda->esquerda->esquerda = new No(10);
	raiz->esquerda->esquerda->direita = new No(25);
	raiz->direita->esquerda = new No(70);
	raiz->direita->direita = new No(90);

	// Atualiza as alturas de baixo para cima, como faria uma AVL.
	atualizarAltura(raiz->esquerda->esquerda->esquerda);
	atualizarAltura(raiz->esquerda->esquerda->direita);
	atualizarAltura(raiz->esquerda->esquerda);
	atualizarAltura(raiz->esquerda->direita);
	atualizarAltura(raiz->direita->esquerda);
	atualizarAltura(raiz->direita->direita);
	atualizarAltura(raiz->esquerda);
	atualizarAltura(raiz->direita);
	atualizarAltura(raiz);

	cout << "Antes da rotacao:\n";
	imprimirArvore(raiz);
	cout << "\nPre-ordem: ";
	imprimirPreOrdem(raiz);
	cout << "\n";

	// Aqui acontece a rotação simples à direita.
	raiz = rotacaoSimplesDireita(raiz);

	cout << "\nDepois da rotacao:\n";
	imprimirArvore(raiz);
	cout << "\nPre-ordem: ";
	imprimirPreOrdem(raiz);
	cout << "\n";

	liberar(raiz);
	return 0;
}
