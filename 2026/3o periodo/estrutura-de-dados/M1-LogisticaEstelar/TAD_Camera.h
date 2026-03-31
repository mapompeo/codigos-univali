#ifndef TAD_CAMERA_H
#define TAD_CAMERA_H

#include <string>

using namespace std;

struct NoCamera
{
    int id;
    string local;
    NoCamera *prox;
};

struct ListaCameras
{
    NoCamera *head;
    int tamanho;
};

void inserirCamera(ListaCameras *lista, int id, string local);
void removerCamera(ListaCameras *lista, int id);

#endif
