#include "dados.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Dados para geracao dos arquivos                                     */
/* ------------------------------------------------------------------ */

static const char *nomes_base[] = {
    "Arroz", "Feijao", "Acucar", "Sal", "Oleo de Soja", "Leite Integral",
    "Manteiga", "Queijo Mussarela", "Iogurte Natural", "Cream Cheese",
    "Margarina", "Requeijao", "Creme de Leite", "Leite Condensado",
    "Frango Inteiro", "Peito de Frango", "Carne Moida", "Picanha",
    "Linguica Calabresa", "Salsicha", "Presunto Fatiado", "Bacon",
    "File de Salmao", "Atum em Lata", "Sardinha em Lata",
    "Pao de Forma", "Biscoito Recheado", "Biscoito de Agua",
    "Macarrao Espaguete", "Farinha de Trigo", "Fermento Biologico",
    "Tomate", "Cebola", "Alho", "Batata", "Cenoura", "Alface",
    "Brocolis", "Couve", "Espinafre", "Abobrinha",
    "Maca Gala", "Banana Prata", "Laranja Pera", "Uva Italia",
    "Pera Williams", "Mamao", "Melancia", "Abacate",
    "Agua Mineral", "Refrigerante Cola", "Refrigerante Guarana",
    "Suco de Laranja", "Suco de Uva", "Cerveja Lata", "Vinho Tinto",
    "Cafe Moido", "Cha Verde", "Achocolatado",
    "Sabonete", "Shampoo", "Condicionador", "Pasta de Dente",
    "Detergente Liquido", "Desinfetante", "Agua Sanitaria", "Sabao em Po",
    "Papel Higienico", "Toalha de Papel", "Guardanapo", "Fralda",
    "Absorvente", "Cotonete",
    "Chocolate ao Leite", "Chocolate Amargo", "Sorvete", "Geleia",
    "Mel Puro", "Ketchup", "Mostarda", "Maionese", "Azeite de Oliva",
    "Vinagre", "Molho de Tomate", "Extrato de Tomate",
    "Pizza Congelada", "Lasanha Congelada", "Hamburguer Congelado",
    "Batata Frita Congelada", "Nuggets de Frango",
    "Ervilha em Lata", "Milho em Lata", "Palmito",
    "Cereal Matinal", "Aveia", "Granola", "Amendoim",
    "Pipoca de Micro-ondas", "Salgadinho", "Barra de Cereal"
};
#define NUM_NOMES_BASE 96

static const char *complementos[] = {
    "Tipo 1", "Tipo 2", "Premium", "Light", "Zero Lactose", "Integral",
    "500g", "1kg", "2kg", "5kg", "200ml", "500ml", "1L", "2L",
    "com Ervas", "Defumado", "Especial", "Organico",
    "Diet", "Tradicional", "Extra", "Sem Gluten", "Zero Acucar"
};
#define NUM_COMPLEMENTOS 23

static const char *nomes_clientes[] = {
    "Ana", "Carlos", "Maria", "Joao", "Pedro", "Julia", "Lucas", "Beatriz",
    "Rafael", "Fernanda", "Gustavo", "Camila", "Matheus", "Larissa", "Thiago",
    "Amanda", "Felipe", "Natalia", "Bruno", "Gabriela", "Leonardo", "Mariana",
    "Diego", "Patricia", "Rodrigo", "Juliana", "Eduardo", "Vanessa", "Marcos",
    "Aline", "Roberto", "Cristiane", "Alexandre", "Sandra", "Paulo", "Renata",
    "Sergio", "Monica", "Andre", "Claudia", "Ricardo", "Silvia", "Marcelo",
    "Tatiana", "Fabio", "Priscila", "Leandro", "Simone", "Daniel", "Debora"
};
#define NUM_NOMES_CLI 50

static const char *sobrenomes[] = {
    "Silva", "Santos", "Oliveira", "Souza", "Rodrigues", "Ferreira", "Alves",
    "Pereira", "Lima", "Gomes", "Costa", "Ribeiro", "Martins", "Carvalho",
    "Almeida", "Lopes", "Soares", "Fernandes", "Vieira", "Barbosa",
    "Rocha", "Dias", "Nascimento", "Araujo", "Moreira"
};
#define NUM_SOBRENOMES 25

/* gerador de pseudoaleatorio simples (para nao depender do rand() global) */
static unsigned int _seed = 42;
static int _rand(void) {
    _seed = _seed * 1664525u + 1013904223u;
    return (int)((_seed >> 1) & 0x7FFFFFFF);
}

void gerar_produtos_txt(const char *caminho) {
    FILE *f = fopen(caminho, "w");
    if (!f) return;
    _seed = 42;
    for (int i = 0; i < 1000; i++) {
        int  codigo      = 1000 + i;
        int  base_idx    = i % NUM_NOMES_BASE;
        int  comp_idx    = (i / NUM_NOMES_BASE) % NUM_COMPLEMENTOS;
        char nome[MAX_NOME];
        snprintf(nome, MAX_NOME, "%s %s", nomes_base[base_idx], complementos[comp_idx]);
        float preco      = 2.50f + (_rand() % 9750) / 100.0f;
        int   quantidade = 10 + _rand() % 191;
        int   dia        = 1  + _rand() % 28;
        int   mes        = 1  + _rand() % 6;
        int   ano        = 2026;
        fprintf(f, "%d;%s;%.2f;%d;%02d;%02d;%d\n",
                codigo, nome, preco, quantidade, dia, mes, ano);
    }
    fclose(f);
}

void gerar_clientes_txt(const char *caminho) {
    FILE *f = fopen(caminho, "w");
    if (!f) return;
    _seed = 123;
    for (int i = 0; i < 1000; i++) {
        int  ni  = _rand() % NUM_NOMES_CLI;
        int  si  = _rand() % NUM_SOBRENOMES;
        int  dia = 1 + _rand() % 28;
        int  mes = 1 + _rand() % 6;
        fprintf(f, "%s %s;%02d;%02d;2026\n",
                nomes_clientes[ni], sobrenomes[si], dia, mes);
    }
    fclose(f);
}

/* ------------------------------------------------------------------ */
/*  Leitura dos arquivos                                                */
/* ------------------------------------------------------------------ */

int carregar_produtos(const char *caminho, TabelaHash *hash) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;

    int  count = 0;
    char linha[MAX_TEXTO];
    while (fgets(linha, sizeof(linha), f)) {
        Produto p;
        if (sscanf(linha, "%d;%99[^;];%f;%d;%d;%d;%d",
                   &p.codigo, p.nome, &p.preco, &p.quantidade,
                   &p.dia, &p.mes, &p.ano) == 7) {
            hash_inserir(hash, p);
            count++;
        }
    }
    fclose(f);
    return count;
}

int carregar_clientes(const char *caminho, Fila *fila) {
    FILE *f = fopen(caminho, "r");
    if (!f) return 0;

    int  count = 0;
    char linha[MAX_TEXTO];
    while (fgets(linha, sizeof(linha), f)) {
        Cliente c;
        if (sscanf(linha, "%99[^;];%d;%d;%d",
                   c.nome, &c.dia, &c.mes, &c.ano) == 4) {
            fila_enfileirar(fila, c);
            count++;
        }
    }
    fclose(f);
    return count;
}
