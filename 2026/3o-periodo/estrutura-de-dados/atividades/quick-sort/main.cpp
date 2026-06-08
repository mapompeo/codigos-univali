#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

// ================================================================
//  UTILITARIOS
// ================================================================

void imprimirArray(int *arr, int n) {
    cout << "[ ";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i < n - 1) cout << ", ";
    }
    cout << " ]" << endl;
}

// Imprime o array de 10 elementos com marcadores visuais:
//   (xx) = fora do subarray ativo
//   [xx] = dentro do subarray ativo
//   {xx} = pivo atual
void imprimirDebug(int *arr, int n, int inicio, int fim, int posPivo) {
    cout << "  ";
    for (int i = 0; i < n; i++) {
        if (i == posPivo) {
            cout << "{" << setw(2) << arr[i] << "}";
        } else if (i >= inicio && i <= fim) {
            cout << "[" << setw(2) << arr[i] << "]";
        } else {
            cout << "(" << setw(2) << arr[i] << ")";
        }
        if (i < n - 1) cout << " ";
    }
    cout << endl;
}

void gerarArray(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100 + 1; // valores de 1 a 100
    }
}

void pressEnter() {
    cout << "\n  [ENTER para proxima etapa...]";
    cin.ignore();
    cin.get();
}

// ================================================================
//  QUICKSORT DEBUG (array de 10 elementos, passo a passo)
// ================================================================

int particaoDebug(int *arr, int n, int inicio, int fim) {
    int pivo    = arr[fim];
    int i       = inicio - 1;
    int trocas  = 0;

    cout << "\n  Subarray ativo: indices [" << inicio << " .. " << fim << "]" << endl;
    cout << "  Pivo escolhido: " << pivo
         << " (ultima posicao, indice " << fim << ")" << endl;
    imprimirDebug(arr, n, inicio, fim, fim);
    pressEnter();

    for (int j = inicio; j < fim; j++) {
        cout << "  Comparando arr[" << j << "] = " << arr[j]
             << " com pivo = " << pivo << " → ";

        if (arr[j] <= pivo) {
            i++;
            cout << arr[j] << " <= " << pivo
                 << "  | TROCA arr[" << i << "] <-> arr[" << j << "]" << endl;
            int tmp = arr[i];
            arr[i]  = arr[j];
            arr[j]  = tmp;
            trocas++;
            imprimirDebug(arr, n, inicio, fim, fim);
            pressEnter();
        } else {
            cout << arr[j] << " >  " << pivo << "  | sem troca" << endl;
        }
    }

    // Coloca o pivo na posicao definitiva
    int posCorreta = i + 1;
    cout << "\n  Inserindo pivo " << pivo
         << " na posicao definitiva [" << posCorreta << "]" << endl;
    int tmp        = arr[posCorreta];
    arr[posCorreta] = arr[fim];
    arr[fim]        = tmp;
    imprimirDebug(arr, n, inicio, fim, posCorreta);

    cout << "  Pivo " << pivo << " FIXADO na posicao " << posCorreta
         << "! (" << trocas << " troca(s) realizadas)" << endl;

    if (posCorreta - 1 >= inicio)
        cout << "  Proxima chamada: subarray esquerdo  [" << inicio
             << " .. " << posCorreta - 1 << "]" << endl;
    else
        cout << "  Subarray esquerdo vazio, nada a fazer." << endl;

    if (posCorreta + 1 <= fim)
        cout << "  Proxima chamada: subarray direito   ["
             << posCorreta + 1 << " .. " << fim << "]" << endl;
    else
        cout << "  Subarray direito vazio, nada a fazer." << endl;

    pressEnter();
    return posCorreta;
}

void quicksortDebug(int *arr, int n, int inicio, int fim, int nivel) {
    if (inicio < fim) {
        cout << "\n========================================" << endl;
        cout << "  NIVEL DE RECURSAO: " << nivel << endl;
        cout << "========================================";
        int pos = particaoDebug(arr, n, inicio, fim);
        quicksortDebug(arr, n, inicio, pos - 1, nivel + 1);
        quicksortDebug(arr, n, pos + 1, fim,   nivel + 1);
    }
}

void opcaoDebug() {
    int n   = 10;
    int *arr = new int[n];
    gerarArray(arr, n);

    cout << "\n========================================" << endl;
    cout << "  MODO DEBUG - 10 ELEMENTOS" << endl;
    cout << "========================================" << endl;
    cout << "Array gerado aleatoriamente:" << endl;
    cout << "  ";
    imprimirArray(arr, n);
    cout << "\nLegenda: (xx) fora do subarray  "
         << "[xx] dentro do subarray  "
         << "{xx} pivo" << endl;
    pressEnter();

    quicksortDebug(arr, n, 0, n - 1, 1);

    cout << "\n========================================" << endl;
    cout << "  ORDENACAO CONCLUIDA!" << endl;
    cout << "========================================" << endl;
    cout << "Array final: ";
    imprimirArray(arr, n);

    delete[] arr;
}

// ================================================================
//  QUICKSORT NORMAL (sem debug, para benchmark)
// ================================================================

int particao(int *arr, int inicio, int fim) {
    int pivo = arr[fim];
    int i    = inicio - 1;
    for (int j = inicio; j < fim; j++) {
        if (arr[j] <= pivo) {
            i++;
            int tmp = arr[i];
            arr[i]  = arr[j];
            arr[j]  = tmp;
        }
    }
    int tmp      = arr[i + 1];
    arr[i + 1]  = arr[fim];
    arr[fim]     = tmp;
    return i + 1;
}

void quicksort(int *arr, int inicio, int fim) {
    if (inicio < fim) {
        int pos = particao(arr, inicio, fim);
        quicksort(arr, inicio, pos - 1);
        quicksort(arr, pos + 1, fim);
    }
}

void opcaoPerformance(int n) {
    int *arr = new int[n];
    gerarArray(arr, n);

    cout << "\n========================================" << endl;
    cout << "  MODO PERFORMANCE - " << n << " ELEMENTOS" << endl;
    cout << "========================================" << endl;
    cout << "Gerando array aleatorio... OK" << endl;
    cout << "Ordenando..." << endl;

    clock_t tempoInicio = clock();
    quicksort(arr, 0, n - 1);
    clock_t tempoFim = clock();

    double segundos = (double)(tempoFim - tempoInicio) / CLOCKS_PER_SEC;

    cout << "Concluido!" << endl;
    cout << fixed << setprecision(6);
    cout << "\nTempo de execucao : " << segundos << " s" << endl;
    cout << "                    " << segundos * 1000.0 << " ms" << endl;

    // Verifica se realmente ficou ordenado
    int correto = 1;
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            correto = 0;
            break;
        }
    }
    cout << "Ordenacao correta : " << (correto ? "SIM ✓" : "NAO ✗") << endl;

    delete[] arr;
}

// ================================================================
//  MENU PRINCIPAL
// ================================================================

int main() {
    srand((unsigned int)time(NULL));

    int opcao = -1;

    while (opcao != 0) {
        cout << "\n========================================" << endl;
        cout << "       QUICKSORT VISUALIZADOR" << endl;
        cout << "========================================" << endl;
        cout << " 1. Debug passo a passo  (10 elementos)" << endl;
        cout << " 2. Benchmark            (10.000 elementos)" << endl;
        cout << " 3. Benchmark            (100.000 elementos)" << endl;
        cout << " 4. Benchmark            (1.000.000 elementos)" << endl;
        cout << " 0. Sair" << endl;
        cout << "========================================" << endl;
        cout << " Opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1: opcaoDebug();                 break;
            case 2: opcaoPerformance(10000);      break;
            case 3: opcaoPerformance(100000);    break;
            case 4: opcaoPerformance(1000000);    break;
            case 0: cout << "\nAte mais!\n";      break;
            default: cout << "\nOpcao invalida.\n";
        }
    }

    return 0;
}