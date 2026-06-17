#include "sort.h"

static void trocar(Venda *a, Venda *b) {
    Venda tmp = *a;
    *a = *b;
    *b = tmp;
}

static int particionar(Venda *v, int low, int high) {
    float pivot = v[high].valor_total;
    int   i     = low - 1;
    for (int j = low; j < high; j++) {
        if (v[j].valor_total >= pivot) {   /* descrescente: >= pivot sobe */
            i++;
            trocar(&v[i], &v[j]);
        }
    }
    trocar(&v[i + 1], &v[high]);
    return i + 1;
}

void quicksort_vendas_desc(Venda *v, int low, int high) {
    if (low < high) {
        int pi = particionar(v, low, high);
        quicksort_vendas_desc(v, low, pi - 1);
        quicksort_vendas_desc(v, pi + 1, high);
    }
}
