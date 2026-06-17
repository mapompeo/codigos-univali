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

## Como clonar e usar

### Pré-requisitos

Você só precisa ter o [Git](https://git-scm.com/) instalado. Para rodar os projetos, veja os requisitos de cada pasta — alguns precisam de compilador C, outros de Java ou Node.js.

### Clonar o repositório

```bash
git clone https://github.com/seu-usuario/codigos-univali.git
cd codigos-univali
```

> Substitua `seu-usuario` pelo usuário correto do GitHub onde o repositório está hospedado.

### Navegar pela estrutura

O repositório segue a convenção `{ano}/{periodo}/{disciplina}/{tipo}/{conteudo}`:

```bash
# Ver todos os anos disponíveis
ls

# Entrar em um semestre específico
cd 2026/3o-periodo

# Listar disciplinas do período
ls

# Entrar em uma disciplina
cd estrutura-de-dados

# Ver trabalhos avaliativos
ls trabalhos/

# Abrir um projeto específico no VS Code
code trabalhos/m3-mercado
```

### Compilar um projeto em C (exemplo: trabalho M3)

```bash
cd 2026/3o-periodo/estrutura-de-dados/trabalhos/m3-mercado

# Windows — script de compilação incluído
build.bat

# O executável gerado é: mercadinho.exe
```

> Para projetos que usam Raylib, é necessário instalar o [w64devkit](https://github.com/skeeto/w64devkit/releases) e o [Raylib para MinGW](https://github.com/raysan5/raylib/releases). Veja o `build.bat` de cada projeto para os caminhos esperados.

### Compilar um projeto Java (exemplo: POO)

```bash
cd 2026/3o-periodo/programacao-orientada-a-objetos/atividades/f1

# Com javac diretamente
javac -d out src/**/*.java

# Ou abrir no NetBeans / IntelliJ IDEA — os projetos têm estrutura de pacotes padrão
```

### Rodar um projeto Node.js (exemplo: Programação Web)

```bash
cd 2026/3o-periodo/programacao-web/08-06-2026/backend

npm install
npm start
```

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

Dentro de cada disciplina, a organização segue este padrão:

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

## Disciplinas e Conteúdos

### Algoritmos e Programação — C (1º e 2º períodos, 2025)

Progressão de lógica básica até manipulação de memória em C:

- Variáveis, tipos, operadores e controle de fluxo
- Funções com passagem por valor e por referência (`*ponteiro`)
- Vetores e matrizes (alocação estática)
- Strings em C (`char[]`, `scanf`, `printf`)
- Arquivos texto com `fopen`/`fclose`/`fscanf`

---

### Circuitos Digitais (2º período, 2025)

- Álgebra de Boole e simplificação de expressões
- Portas lógicas (AND, OR, NOT, NAND, NOR, XOR)
- Circuitos combinacionais e sequenciais
- Flip-flops e registradores

---

### Arquitetura e Organização de Processadores (3º período, 2026)

- Arquitetura de conjunto de instruções (ISA)
- Modos de endereçamento
- Pipeline e hazards
- Exercícios práticos em Assembly
- Análise de desempenho de processadores

---

### Estrutura de Dados (3º período, 2026)

Implementação das principais estruturas de dados do zero em **C puro**, com análise de complexidade assintótica. Professor: Welington Gadelha.

| Estrutura | Onde encontrar |
|---|---|
| TAD (Tipo Abstrato de Dado) | `atividades/TAD/` |
| Listas lineares, simples e duplas | `atividades/Listas-*/` |
| Pilha dinâmica | `atividades/pilha/` |
| Fila dinâmica | `atividades/fila/` |
| Quick Sort | `atividades/quick-sort/` |
| Árvore AVL | `atividades/avl/` |
| BST com remoção | `trabalhos/remocao-em-bst/` |
| AVL vs Red-Black | `trabalhos/avl-e-redblack/` |
| Análise empírica de complexidade | `trabalhos/analise-empirica-*/` |
| **Sistema completo (M3 — Mercadinho)** | `trabalhos/m3-mercado/` |

**Destaque — Trabalho M3:** sistema de gerenciamento de mercadinho com interface gráfica em Raylib integrando Tabela Hash (estoque), Fila (atendimento), Pilha (carrinho com undo), BST (histórico de vendas) e Quick Sort (ranking de faturamento). Documentação linha a linha disponível em `trabalhos/m3-mercado/DOCUMENTACAO.md`.

---

### Programação Orientada a Objetos — Java (3º período, 2026)

Paradigma OO com estrutura de pacotes padrão Java (`br.univali.cc.*`):

| Projeto | Conceitos aplicados |
|---|---|
| `calculo-imc/` | Classes, atributos, métodos |
| `banco/`, `BancoDoBrasil/` | Encapsulamento, herança |
| `empresa/`, `zoo/` | Polimorfismo, sobrescrita |
| `financas/` | Coleções (`ArrayList`) |
| `f1/` | Arquitetura MVC (Model / Service / UI) |
| `modelagem/` | Modelagem UML, diagrama de classes |

---

### Programação Web (3º período, 2026)

Evolução do HTML estático até aplicações full-stack com Node.js:

| Etapa | Tecnologias |
|---|---|
| Front-end estático | HTML5, CSS3 |
| Interatividade | JavaScript, manipulação do DOM |
| Back-end | Node.js, Express.js |
| Full-stack | REST API + front-end separado, upload de arquivos |

O projeto mais completo está em `08-06-2026/`, com pastas `backend/` e `frontend/` separadas, rotas REST, controladores, modelos e camada de dados.

---

## Tecnologias

| Linguagem / Ferramenta | Contexto no curso |
|---|---|
| **C** (C99) | Algoritmos e Programação, Estrutura de Dados |
| **C++** | Alguns trabalhos de Estrutura de Dados |
| **Assembly** | Arquitetura e Organização de Processadores |
| **Java** | Programação Orientada a Objetos |
| **JavaScript** | Programação Web (cliente e servidor) |
| **HTML / CSS** | Programação Web |
| **Node.js / Express** | Back-end em Programação Web |
| **Raylib** | Interface gráfica nos trabalhos de ED (M1 e M3) |
| **CMake** | Build system dos projetos C que usam Raylib |

---

## Convenções

### O que está versionado

- Código-fonte (`.c`, `.h`, `.java`, `.js`, `.html`, `.css`, `.s`)
- Scripts de build (`build.bat`, `CMakeLists.txt`, `package.json`)
- Documentação (`.md`)

### O que **não** está versionado

- Binários compilados (`.exe`, `.out`, `.class`)
- Pastas de build (`/build`, `/bin`, `/out`)
- Dependências instaladas (`node_modules/`)
- Configurações locais de IDE (`.vscode/`, `.idea/`, `nbproject/`)
- Arquivos de sistema e temporários

---

## Licença

Todos os arquivos deste repositório estão sob licença aberta. Você pode usar, estudar e adaptar o código para fins educacionais, com os devidos créditos.

<div align="center">

---

UNIVALI — Universidade do Vale do Itajaí &nbsp;·&nbsp; Ciência da Computação &nbsp;·&nbsp; Itajaí, SC

</div>
