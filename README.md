<div align="center">

# Repositório Acadêmico — UNIVALI
### Bacharelado em Ciência da Computação

**Matheus Pompeo** &nbsp;·&nbsp; Universidade do Vale do Itajaí &nbsp;·&nbsp; Itajaí, SC

---

![Linguagens](https://img.shields.io/badge/Linguagens-C%20%7C%20C%2B%2B%20%7C%20Java%20%7C%20JS%20%7C%20ASM-blue?style=flat-square)
![Instituição](https://img.shields.io/badge/Instituição-UNIVALI-orange?style=flat-square)
![Licença](https://img.shields.io/badge/Licença-Aberta-brightgreen?style=flat-square)

</div>

---

## Sobre

Registro completo do percurso acadêmico no curso de **Bacharelado em Ciência da Computação** da UNIVALI. Cada pasta corresponde a um ano letivo, com subpastas por período, disciplina e, dentro delas, por exercício, atividade ou trabalho avaliativo.

O repositório serve três propósitos:

- **Versionamento** do trabalho produzido em aula e fora dela
- **Portfólio** de evolução técnica ao longo do curso
- **Referência** para revisitar conceitos e soluções já implementados

---

## Como clonar

```bash
git clone https://github.com/seu-usuario/codigos-univali.git
cd codigos-univali
```

> Substitua `seu-usuario` pelo usuário correto do GitHub onde o repositório está hospedado.

---

## Como navegar

O repositório segue a convenção `{ano}/{periodo}/{disciplina}/{tipo}/{conteudo}`:

```bash
# Ver todos os anos disponíveis
ls

# Entrar em um semestre específico
cd 2026/3o-periodo

# Listar disciplinas do período
ls

# Entrar em uma disciplina e ver os trabalhos
cd estrutura-de-dados
ls trabalhos/

# Abrir um projeto específico no VS Code
code trabalhos/m3-mercado
```

Para compilar ou executar um projeto, consulte o `README.md` ou o script de build dentro da própria pasta do projeto — cada um tem suas instruções e dependências específicas.

---

## Estrutura do Repositório

```
codigos-univali/
│
├── 2025/
│   ├── 1o periodo/
│   │   └── algoritmos-e-programacao/    ← Introdução à lógica e programação em C
│   │
│   └── 2o periodo/
│       ├── algoritmos-e-programacao/    ← C avançado: vetores, matrizes, ponteiros
│       └── circuitos-digitais/          ← Álgebra booleana e portas lógicas
│
└── 2026/
    └── 3o-periodo/
        ├── arquitetura-e-organizacao-de-processadores/   ← Assembly e ISA
        ├── estrutura-de-dados/                           ← Hash, fila, pilha, BST, AVL
        ├── programacao-orientada-a-objetos/              ← Java, herança, polimorfismo
        └── programacao-web/                              ← HTML/CSS/JS, Node.js, Express
```

Dentro de cada disciplina:

```
{disciplina}/
├── exercicios/
│   └── {DD-MM-AAAA}/      ← exercício feito em aula naquela data
├── atividades/
│   └── {nome}/            ← atividade de fixação com tema específico
└── trabalhos/
    └── {identificador}/   ← trabalho avaliativo (M1, M2, M3...)
```

---

## Licença

Todos os arquivos deste repositório estão sob licença aberta. Você pode usar, estudar e adaptar o código para fins educacionais, com os devidos créditos.

<div align="center">

---

UNIVALI — Universidade do Vale do Itajaí &nbsp;·&nbsp; Ciência da Computação &nbsp;·&nbsp; Itajaí, SC

</div>
