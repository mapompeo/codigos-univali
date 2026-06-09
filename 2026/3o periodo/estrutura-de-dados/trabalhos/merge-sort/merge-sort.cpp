// Estrutura de Dados - Merge Sort
// Aluno: Matheus Pompeo Dias

#include <iostream>
#include <vector>

using namespace std;

// Intercala as duas metades ja ordenadas: v[ini..meio] e v[meio+1..fim]
void merge(vector<int>& v, int ini, int meio, int fim) {
    vector<int> temp;
    int i = ini, j = meio + 1;

    // copia sempre o menor elemento entre as duas metades
    while (i <= meio && j <= fim) {
        if (v[i] <= v[j])
            temp.push_back(v[i++]);
        else
            temp.push_back(v[j++]);
    }

    while (i <= meio) temp.push_back(v[i++]);
    while (j <= fim)  temp.push_back(v[j++]);

    for (int k = 0; k < (int)temp.size(); k++)
        v[ini + k] = temp[k];
}

// Divide o vetor ao meio recursivamente ate sobrar 1 elemento (caso base)
void mergeSort(vector<int>& v, int ini, int fim) {
    if (ini >= fim) return;

    int meio = (ini + fim) / 2;
    mergeSort(v, ini, meio);
    mergeSort(v, meio + 1, fim);
    merge(v, ini, meio, fim);
}

void imprime(const vector<int>& v) {
    cout << "[ ";
    for (int x : v) cout << x << " ";
    cout << "]" << endl;
}

int main() {
    vector<int> vetor = {38, 27, 43, 3, 9, 82, 10};

    cout << "Vetor original: ";
    imprime(vetor);

    mergeSort(vetor, 0, vetor.size() - 1);

    cout << "Vetor ordenado: ";
    imprime(vetor);

    return 0;
}
