#ifndef app
#define app


struct No
{
    int data;
    No *next;
    No *prev;
};

struct ListaControle
{
    No *cabeca;
};

void inserir_fim(ListaControle *lista, int data);
void inserir_inicio(ListaControle *lista, int data);
void limpar_lista(ListaControle *lista);
void exibir_direto(ListaControle *lista);
void exibir_inverso(ListaControle *lista);


#endif