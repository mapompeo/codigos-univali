#ifndef DADOS_H
#define DADOS_H

#include "types.h"
#include "hash.h"
#include "fila.h"

void gerar_produtos_txt(const char *caminho);
void gerar_clientes_txt(const char *caminho);

int carregar_produtos(const char *caminho, TabelaHash *hash);
int carregar_clientes(const char *caminho, Fila *fila);

#endif /* DADOS_H */
