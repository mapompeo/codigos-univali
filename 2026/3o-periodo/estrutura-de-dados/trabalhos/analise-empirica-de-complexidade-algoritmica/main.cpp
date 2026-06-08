#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
using namespace std;

const int HASH_SIZE = 100003;

bool forcaBruta(int *arr, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] == arr[j])
                return true;
    return false;
}

int comparar(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

bool ordenacaoVarredura(int *arr, int n)
{
    int *copia = (int *)malloc(n * sizeof(int));
    memcpy(copia, arr, n * sizeof(int));
    qsort(copia, n, sizeof(int), comparar);

    for (int i = 0; i < n - 1; i++)
        if (copia[i] == copia[i + 1])
        {
            free(copia);
            return true;
        }
    free(copia);
    return false;
}

bool hashTable(int *arr, int n)
{
    vector<int> hash(HASH_SIZE, -1);
    for (int i = 0; i < n; i++)
    {
        int pos = abs(arr[i]) % HASH_SIZE;
        while (hash[pos] != -1)
        {
            if (hash[pos] == arr[i])
                return true;
            pos = (pos + 1) % HASH_SIZE;
        }
        hash[pos] = arr[i];
    }
    return false;
}

int main()
{
    srand(42);
    int tamanhos[] = {5000, 20000, 50000};

    // ===== PIOR CASO =====
    cout << "\n+==============================================================+" << endl;
    cout << "|        PIOR CASO (Vetor sem duplicatas)                     |" << endl;
    cout << "+==============================================================+" << endl;
    cout << "|  N       |  Algoritmo A  |  Algoritmo B  |  Algoritmo C   |" << endl;
    cout << "|          | (Forca Bruta) | (Ord.+Varr.)  |  (Hash Table)  |" << endl;
    cout << "+==============================================================+" << endl;

    for (int n : tamanhos)
    {
        int *arr = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
            arr[i] = i;

        int rep = (n <= 5000) ? 50 : (n <= 20000) ? 5
                                                  : 1;

        clock_t t1 = clock();
        for (int r = 0; r < rep; r++)
        {
            int *t = (int *)malloc(n * sizeof(int));
            memcpy(t, arr, n * sizeof(int));
            forcaBruta(t, n);
            free(t);
        }
        double tempo_a = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000 / rep;

        t1 = clock();
        for (int r = 0; r < rep; r++)
        {
            int *t = (int *)malloc(n * sizeof(int));
            memcpy(t, arr, n * sizeof(int));
            ordenacaoVarredura(t, n);
            free(t);
        }
        double tempo_b = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000 / rep;

        t1 = clock();
        for (int r = 0; r < rep; r++)
        {
            int *t = (int *)malloc(n * sizeof(int));
            memcpy(t, arr, n * sizeof(int));
            hashTable(t, n);
            free(t);
        }
        double tempo_c = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000 / rep;

        printf("|  %5d   |   %8.3fms   |   %8.3fms   |   %8.3fms  |\n", n, tempo_a, tempo_b, tempo_c);
        free(arr);
    }

    cout << "+==============================================================+" << endl
         << endl;

    // ===== CASO MÉDIO =====
    cout << "+==============================================================+" << endl;
    cout << "|       CASO MÉDIO (Valores aleatórios, [0, 10*n))            |" << endl;
    cout << "+==============================================================+" << endl;
    cout << "|  N       |  Algoritmo A  |  Algoritmo B  |  Algoritmo C   |" << endl;
    cout << "|          | (Forca Bruta) | (Ord.+Varr.)  |  (Hash Table)  |" << endl;
    cout << "+==============================================================+" << endl;

    for (int n : tamanhos)
    {
        int *arr = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
            arr[i] = rand() % (10 * n);

        int rep = (n <= 5000) ? 20 : (n <= 20000) ? 5
                                                  : 1;

        clock_t t1 = clock();
        for (int r = 0; r < rep; r++)
        {
            int *t = (int *)malloc(n * sizeof(int));
            memcpy(t, arr, n * sizeof(int));
            forcaBruta(t, n);
            free(t);
        }
        double tempo_a = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000 / rep;

        t1 = clock();
        for (int r = 0; r < rep; r++)
        {
            int *t = (int *)malloc(n * sizeof(int));
            memcpy(t, arr, n * sizeof(int));
            ordenacaoVarredura(t, n);
            free(t);
        }
        double tempo_b = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000 / rep;

        t1 = clock();
        for (int r = 0; r < rep; r++)
        {
            int *t = (int *)malloc(n * sizeof(int));
            memcpy(t, arr, n * sizeof(int));
            hashTable(t, n);
            free(t);
        }
        double tempo_c = (double)(clock() - t1) / CLOCKS_PER_SEC * 1000 / rep;

        printf("|  %5d   |   %8.3fms   |   %8.3fms   |   %8.3fms  |\n", n, tempo_a, tempo_b, tempo_c);
        free(arr);
    }

    cout << "+==============================================================+" << endl
         << endl;
    return 0;
}