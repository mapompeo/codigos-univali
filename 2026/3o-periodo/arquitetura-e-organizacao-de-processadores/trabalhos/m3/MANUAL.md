# Manual do Processador RISC-V 32-bit Monociclo

> Trabalho M3 — Arquitetura e Organização de Processadores  
> Curso: Ciência da Computação — UNIVALI

---

## Índice

1. [O que é este projeto?](#1-o-que-é-este-projeto)
2. [Visão geral da arquitetura](#2-visão-geral-da-arquitetura)
3. [Arquivos do projeto](#3-arquivos-do-projeto)
4. [Componentes explicados um a um](#4-componentes-explicados-um-a-um)
   - [PC — Program Counter](#41-pc--program-counter)
   - [InstrFile — Memória de Instruções](#42-instrfile--memória-de-instruções)
   - [ImmGen — Gerador de Imediatos](#43-immgen--gerador-de-imediatos)
   - [Control — Unidade de Controle](#44-control--unidade-de-controle)
   - [RegFile — Banco de Registradores](#45-regfile--banco-de-registradores)
   - [ULA — Unidade Lógica e Aritmética](#46-ula--unidade-lógica-e-aritmética)
   - [MUX21 — Multiplexador](#47-mux21--multiplexador)
   - [Adder — Somador](#48-adder--somador)
   - [DataMem — Memória de Dados](#49-datamem--memória-de-dados)
5. [O arquivo topo: design.vhd](#5-o-arquivo-topo-designvhd)
   - [Caminho do PC](#51-caminho-do-pc)
   - [Lógica de desvio (branch)](#52-lógica-de-desvio-branch)
   - [Datapath](#53-datapath)
   - [Mux de writeback](#54-mux-de-writeback)
6. [Formatos de instrução RISC-V](#6-formatos-de-instrução-risc-v)
7. [Fluxo completo de uma instrução](#7-fluxo-completo-de-uma-instrução)
   - [Exemplo: ADDI x5, x0, 6](#71-exemplo-addi-x5-x0-6)
   - [Exemplo: ADD x7, x5, x6](#72-exemplo-add-x7-x5-x6)
   - [Exemplo: SW x5, 0(x0)](#73-exemplo-sw-x5-0x0)
   - [Exemplo: LW x11, 0(x0)](#74-exemplo-lw-x11-0x0)
   - [Exemplo: BEQ x5, x11, +8](#75-exemplo-beq-x5-x11-8)
8. [O programa de teste](#8-o-programa-de-teste)
9. [O testbench](#9-o-testbench)
10. [Como rodar no EDA Playground](#10-como-rodar-no-eda-playground)
11. [Resultados esperados na simulação](#11-resultados-esperados-na-simulação)
12. [Glossário](#12-glossário)

---

## 1. O que é este projeto?

Este projeto implementa um **processador RISC-V 32 bits monociclo** usando a linguagem de descrição de hardware **VHDL**.

Um processador é o "cérebro" do computador: ele lê instruções da memória, decodifica o que cada instrução pede, executa a operação e salva o resultado. Aqui implementamos isso do zero, componente por componente.

**RISC-V** é uma arquitetura de conjunto de instruções (ISA) aberta e gratuita, criada pela Universidade de Berkeley. É usada em chips reais hoje em dia (em dispositivos IoT, processadores de SSD, etc.) e é muito usada em cursos de arquitetura porque sua especificação é simples e limpa.

**Monociclo** significa que cada instrução é executada em exatamente **um ciclo de clock**. O processador lê a instrução, processa e grava o resultado tudo dentro de um único pulso do relógio. É o design mais simples possível — não há pipeline, não há cache, não há hazards.

---

## 2. Visão geral da arquitetura

O processador é composto por dois grandes blocos:

- **Caminho de dados (Datapath):** os fios e componentes que movem e transformam os dados (registradores, ULA, memórias).
- **Unidade de controle (Control):** o "cérebro" que lê a instrução e decide o que cada componente deve fazer.

O fluxo geral de uma instrução é sempre este:

```
1. PC fornece o endereço
        ↓
2. InstrFile entrega a instrução de 32 bits
        ↓
3. Control decodifica a instrução e ativa os sinais certos
4. ImmGen extrai o valor imediato (se houver)
5. RegFile fornece os valores dos registradores
        ↓
6. ULA executa a operação (soma, subtração, comparação...)
        ↓
7. DataMem lê ou escreve na memória (se for LW/SW)
        ↓
8. Resultado é gravado de volta no RegFile
        ↓
9. PC avança para a próxima instrução (PC+4, ou desvio)
```

Tudo isso acontece **ao mesmo tempo**, de forma combinacional (instantânea). O clock só serve para sincronizar as escritas (RegFile, DataMem e PC atualizam apenas na borda de subida).

---

## 3. Arquivos do projeto

| Arquivo | Tipo | Descrição |
|---|---|---|
| `pc.vhd` | Sequencial | Program Counter — guarda o endereço da instrução atual |
| `instrfile.vhd` | Combinacional | Memória ROM com as instruções do programa |
| `immgen.vhd` | Combinacional | Extrai e estende o campo imediato da instrução |
| `control.vhd` | Combinacional | Decodifica o opcode e gera todos os sinais de controle |
| `regfile.vhd` | Sequencial | Banco de 32 registradores de 32 bits |
| `ula.vhd` | Combinacional | Executa operações aritméticas e lógicas |
| `mux21.vhd` | Combinacional | Multiplexador 2:1 — escolhe entre dois valores |
| `adder.vhd` | Combinacional | Somador de 32 bits |
| `subtractor.vhd` | Combinacional | Subtrator de 32 bits (disponível, não instanciado) |
| `dmem.vhd` | Sequencial | Memória de dados para leitura (LW) e escrita (SW) |
| `design.vhd` | Top-level | Liga todos os componentes acima |
| `testbench.sv` | Simulação | Gera o clock, fornece reset e observa os resultados |

---

## 4. Componentes explicados um a um

### 4.1 PC — Program Counter

**Arquivo:** `pc.vhd`

O PC é um registrador de 32 bits que guarda o **endereço da instrução que está sendo executada agora**.

```
Entradas:  i_clk     — clock
           i_rst     — reset (coloca PC em 0)
           i_pc_next — próximo endereço a carregar

Saída:     o_pc      — endereço atual
```

**Como funciona:** a cada borda de subida do clock, o PC carrega o valor de `i_pc_next`. Se `i_rst='1'`, ignora `i_pc_next` e volta para 0.

O que define `i_pc_next` é uma lógica no `design.vhd` que escolhe entre:
- `PC + 4` (próxima instrução, caso normal)
- `PC + imm` (desvio condicional / JAL)
- `rs1 + imm` com bit 0 = 0 (JALR)

**Por que PC+4?** Cada instrução ocupa 4 bytes na memória (32 bits = 4 bytes). Para ir para a instrução seguinte, soma-se 4 ao endereço.

---

### 4.2 InstrFile — Memória de Instruções

**Arquivo:** `instrfile.vhd`

É uma **ROM** (memória somente de leitura) que contém o programa. Dado um endereço (o PC), entrega a instrução de 32 bits correspondente.

```
Entrada:  i_addr  — endereço (vem do PC)
Saída:    o_instr — instrução de 32 bits
```

**Como funciona:** A memória é um array de 32 palavras de 32 bits cada. O índice é calculado como `addr[6:2]` — isso divide o endereço por 4, convertendo endereço de byte para índice de palavra.

Exemplo:
- PC = 0  → índice 0 → ADDI x5, x0, 6
- PC = 4  → índice 1 → ADDI x6, x0, 4
- PC = 8  → índice 2 → ADD x7, x5, x6
- PC = 40 → índice 10 → ADDI x13, x0, 1

**Por que ignorar os 2 bits menos significativos?** Porque instruções RISC-V são sempre alinhadas em 4 bytes — os endereços válidos são 0, 4, 8, 12... Nunca haverá instrução no endereço 1, 2 ou 3.

---

### 4.3 ImmGen — Gerador de Imediatos

**Arquivo:** `immgen.vhd`

Muitas instruções precisam de um valor constante embutido na própria instrução (chamado de **imediato**). O ImmGen extrai esse valor dos bits da instrução e faz a **extensão de sinal** para 32 bits.

```
Entrada:  instr — instrução de 32 bits
Saída:    imm   — imediato de 32 bits com sinal estendido
```

**O que é extensão de sinal?** É copiar o bit mais significativo do imediato para preencher os bits restantes até 32 bits. Isso preserva o valor com sinal. Por exemplo, o imediato `-1` em 12 bits é `111111111111`. Estendido para 32 bits fica `11111111111111111111111111111111`, que ainda é -1 em complemento de 2.

**Formatos de imediato suportados:**

| Formato | Instruções | Como é extraído |
|---|---|---|
| I-type | ADDI, LW, JALR | bits [31:20] |
| S-type | SW | bits [31:25] e [11:7] |
| B-type | BEQ, BNE, BLT... | bits espalhados, já inclui deslocamento |
| U-type | LUI, AUIPC | bits [31:12], shifta 12 à esquerda |
| J-type | JAL | bits espalhados, já inclui deslocamento |

Os bits ficam espalhados nos formatos B e J porque os projetistas do RISC-V tentaram minimizar a lógica de hardware para decodificar os campos `rd`, `rs1` e `rs2`, que ficam sempre na mesma posição.

---

### 4.4 Control — Unidade de Controle

**Arquivo:** `control.vhd`

É o componente que "entende" cada instrução. Recebe os campos de controle da instrução e ativa os sinais certos para fazer o datapath se comportar corretamente.

```
Entradas:  opcode   — bits [6:0]  — qual instrução é
           funct3   — bits [14:12] — subtipo da instrução
           funct7b5 — bit [30]    — distingue ADD de SUB, SRL de SRA

Saídas (sinais de controle):
  reg_write  — '1' permite escrita no banco de registradores
  alu_src    — '0' usa rs2 na ULA | '1' usa o imediato
  mem_read   — '1' habilita leitura da memória de dados
  mem_write  — '1' habilita escrita na memória de dados
  mem_to_reg — '1' o dado da memória vai para o registrador
  branch     — '1' instrução é um desvio condicional
  jump       — '1' instrução é JAL ou JALR
  lui        — '1' instrução é LUI
  auipc      — '1' instrução é AUIPC
  jalr       — '1' instrução é JALR especificamente
  alu_op     — código de 4 bits dizendo qual operação a ULA faz
```

**Tabela de controle resumida:**

| Instrução | reg_write | alu_src | mem_write | mem_to_reg | branch | jump | alu_op |
|---|---|---|---|---|---|---|---|
| ADD/SUB/... (R-type) | 1 | 0 | 0 | 0 | 0 | 0 | varia |
| ADDI/... (I-arith)   | 1 | 1 | 0 | 0 | 0 | 0 | varia |
| LW                   | 1 | 1 | 0 | 1 | 0 | 0 | ADD |
| SW                   | 0 | 1 | 1 | 0 | 0 | 0 | ADD |
| BEQ/BNE/...          | 0 | 0 | 0 | 0 | 1 | 0 | SUB/SLT |
| JAL                  | 1 | 0 | 0 | 0 | 0 | 1 | — |
| JALR                 | 1 | 1 | 0 | 0 | 0 | 1 | ADD |
| LUI                  | 1 | — | 0 | 0 | 0 | 0 | — |
| AUIPC                | 1 | — | 0 | 0 | 0 | 0 | ADD |

---

### 4.5 RegFile — Banco de Registradores

**Arquivo:** `regfile.vhd`

O RISC-V tem **32 registradores de uso geral**, chamados de `x0` a `x31`. Cada um guarda 32 bits. São a "área de trabalho" do processador — operações não podem ser feitas diretamente na memória, então os valores precisam estar nos registradores primeiro.

```
Entradas:  clk    — clock
           we     — write enable ('1' = pode escrever)
           rs1    — endereço (0-31) do registrador a ler (porta 1)
           rs2    — endereço (0-31) do registrador a ler (porta 2)
           rd     — endereço (0-31) do registrador a escrever
           wdata  — valor a escrever

Saídas:    rdata1 — valor do registrador rs1
           rdata2 — valor do registrador rs2
```

**Regra especial: `x0` é sempre zero.** Qualquer escrita em `x0` é ignorada, e qualquer leitura de `x0` retorna 0. Isso é útil para zerar registradores (`ADDI x5, x0, 0`) e para descartar resultados.

**Leitura é combinacional:** os valores saem imediatamente quando `rs1`/`rs2` mudam, sem esperar o clock.

**Escrita é síncrona:** o valor só é efetivamente gravado na borda de subida do clock quando `we='1'`.

---

### 4.6 ULA — Unidade Lógica e Aritmética

**Arquivo:** `ula.vhd`

A ULA executa **todas as operações** do processador. Recebe dois valores de 32 bits e um código de operação, e produz o resultado.

```
Entradas:  i_a      — operando A (sempre vem de rs1)
           i_b      — operando B (rs2 ou imediato, escolhido pelo MUX)
           i_ula_op — código de 4 bits dizendo qual operação fazer

Saídas:    o_ula  — resultado de 32 bits
           o_zero — '1' se o resultado for zero (usado pelo BEQ)
```

**Operações suportadas:**

| Código | Operação | Descrição |
|---|---|---|
| 0000 | ADD  | `A + B` (com sinal) |
| 0001 | SUB  | `A - B` (com sinal) |
| 0010 | AND  | `A & B` (bit a bit) |
| 0011 | OR   | `A \| B` (bit a bit) |
| 0100 | XOR  | `A ^ B` (bit a bit) |
| 0101 | SLL  | `A << B[4:0]` (shift lógico à esquerda) |
| 0110 | SRL  | `A >> B[4:0]` (shift lógico à direita, preenche com 0) |
| 0111 | SRA  | `A >> B[4:0]` (shift aritmético, preenche com sinal) |
| 1000 | SLT  | `1 se A < B (com sinal), senão 0` |
| 1001 | SLTU | `1 se A < B (sem sinal), senão 0` |

**Por que `o_zero`?** O sinal de branch BEQ (branch if equal) precisa saber se dois valores são iguais. `A == B` é a mesma coisa que `A - B == 0`. Então o Control manda a ULA fazer uma SUB, e o `o_zero` indica se o resultado foi zero (ou seja, se são iguais).

**Shifts:** o número de bits a deslocar vem dos 5 bits menos significativos de `i_b`. Por quê 5 bits? Porque 2⁵ = 32, e não faz sentido deslocar mais de 31 posições num valor de 32 bits.

---

### 4.7 MUX21 — Multiplexador

**Arquivo:** `mux21.vhd`

Um multiplexador é simplesmente um "seletor": dado um sinal de controle (`sel`), escolhe entre duas entradas qual vai passar para a saída.

```
Entradas:  a   — opção 0
           b   — opção 1
           sel — '0' passa a, '1' passa b

Saída:     s   — valor escolhido
```

No projeto, o MUX é usado para escolher o segundo operando da ULA:
- `sel = 0` → usa `rs2` (registrador) — instruções R-type
- `sel = 1` → usa `imm` (imediato) — instruções I-type, LW, SW

Quem controla o `sel` é o sinal `alu_src` vindo do Control.

---

### 4.8 Adder — Somador

**Arquivo:** `adder.vhd`

Soma dois valores de 32 bits. É usado duas vezes no projeto:

1. **Para calcular PC+4** — próxima instrução sequencial
2. **Para calcular PC+imm** — alvo de branches, JAL e AUIPC

```
Entradas:  a, b — valores a somar
Saída:     s    — a + b
```

---

### 4.9 DataMem — Memória de Dados

**Arquivo:** `dmem.vhd`

Armazena dados que o programa precisa guardar temporariamente (variáveis, arrays, etc.). É usada pelas instruções `SW` (store word — escreve) e `LW` (load word — lê).

```
Entradas:  i_clk       — clock
           i_mem_write — '1' permite escrita
           i_addr      — endereço de 32 bits (vem da ULA)
           i_wdata     — valor a escrever (vem de rs2)

Saída:     o_rdata     — valor lido (vai para o mux de writeback)
```

**Leitura é combinacional:** sempre que o endereço muda, o valor lido muda instantaneamente.

**Escrita é síncrona:** o valor só é gravado na borda de subida do clock quando `mem_write='1'`.

**Como o endereço é calculado?** A ULA computa `rs1 + imm`. Por exemplo, `SW x5, 8(x2)` escreve o valor de `x5` no endereço `x2 + 8`. Esse `x2 + 8` é calculado pela ULA (por isso a ULA faz ADD para SW e LW, controlada pelo `alu_op` do Control).

---

## 5. O arquivo topo: design.vhd

O `design.vhd` é o **integrador**: ele declara todos os sinais internos e instancia cada componente, conectando as saídas de uns nas entradas de outros. É o "desenho do circuito".

### 5.1 Caminho do PC

```
w_pc → u_INSTRFILE → w_inst (instrução atual)
w_pc → u_PLUS4    → w_pc_plus4 (PC + 4)
w_pc → u_PC_IMM   → w_pc_imm  (PC + imm)

w_pc_next → u_PC → w_pc (registra na próxima borda de clock)
```

O `w_pc_next` é decidido pela **lógica de desvio** descrita na seção 5.2.

### 5.2 Lógica de desvio (branch)

Dois processos combinacionais decidem o próximo PC:

**Processo 1 — condição do branch:**
```vhdl
-- Se não é um branch, branch_taken = '0'
-- Se é um branch, verifica funct3 para saber qual tipo:
BEQ (000): branch se w_zero = '1'         (A == B)
BNE (001): branch se w_zero = '0'         (A != B)
BLT (100): branch se w_ula(0) = '1'       (A < B, com sinal)
BGE (101): branch se w_ula(0) = '0'       (A >= B, com sinal)
BLTU(110): branch se w_ula(0) = '1'       (A < B, sem sinal)
BGEU(111): branch se w_ula(0) = '0'       (A >= B, sem sinal)
```

Para BEQ/BNE a ULA faz SUB e usamos o `o_zero`.  
Para BLT/BGE/BLTU/BGEU a ULA faz SLT ou SLTU e usamos o bit 0 do resultado (que é o 1 ou 0 da comparação).

**Processo 2 — mux do próximo PC:**
```
JALR:              pc_next = (rs1 + imm) com bit 0 = 0  (w_ula & ~1)
JAL ou branch:     pc_next = PC + imm                   (w_pc_imm)
caso normal:       pc_next = PC + 4                     (w_pc_plus4)
```

### 5.3 Datapath

```
w_inst(19:15) → rs1 → RegFile → w_rs1_data → ULA entrada A
w_inst(24:20) → rs2 → RegFile → w_rs2_data → MUX entrada A
                                 w_imm       → MUX entrada B
                     alu_src  → MUX sel
                                 MUX saída   → ULA entrada B
w_inst(14:12) + w_inst(30) + w_inst(6:0) → Control → todos os sinais

ULA resultado (w_ula) → DataMem endereço
w_rs2_data            → DataMem dado de entrada
mem_write             → DataMem escrita habilitada
```

### 5.4 Mux de writeback

Antes de gravar no banco de registradores, um último mux escolhe **qual valor** vai ser escrito em `rd`:

```
lui = '1'       → escreve o imediato diretamente (LUI: rd = imm)
auipc = '1'     → escreve PC + imm              (AUIPC: rd = PC + imm)
jump = '1'      → escreve PC + 4                (JAL/JALR: rd = endereço de retorno)
mem_to_reg = '1'→ escreve o dado da memória     (LW: rd = mem[addr])
senão           → escreve o resultado da ULA    (R-type, I-type)
```

---

## 6. Formatos de instrução RISC-V

Toda instrução RISC-V tem 32 bits. O campo `opcode` (bits 6:0) indica o formato geral:

```
R-type (operações entre registradores):
  [31:25] funct7 | [24:20] rs2 | [19:15] rs1 | [14:12] funct3 | [11:7] rd | [6:0] opcode

I-type (imediato de 12 bits):
  [31:20] imm[11:0] | [19:15] rs1 | [14:12] funct3 | [11:7] rd | [6:0] opcode

S-type (store):
  [31:25] imm[11:5] | [24:20] rs2 | [19:15] rs1 | [14:12] funct3 | [11:7] imm[4:0] | [6:0] opcode

B-type (branch):
  [31] imm[12] | [30:25] imm[10:5] | [24:20] rs2 | [19:15] rs1 | [14:12] funct3 | [11:8] imm[4:1] | [7] imm[11] | [6:0] opcode

U-type (imediato de 20 bits no topo):
  [31:12] imm[31:12] | [11:7] rd | [6:0] opcode

J-type (jump):
  [31] imm[20] | [30:21] imm[10:1] | [20] imm[11] | [19:12] imm[19:12] | [11:7] rd | [6:0] opcode
```

**Por que os campos ficam fora de ordem nos formatos B e J?**  
Para manter `rs1`, `rs2` e `rd` sempre na mesma posição em todos os formatos. Assim o hardware pode decodificar esses campos sem nem saber qual instrução é — economizando transistores.

**Campo `funct7b5` (bit 30):**  
Distingue ADD de SUB e SRL de SRA. Usar apenas 1 bit em vez de todo o `funct7` é suficiente porque esses são os únicos pares que compartilham o mesmo `opcode` e `funct3`.

---

## 7. Fluxo completo de uma instrução

### 7.1 Exemplo: ADDI x5, x0, 6

**Instrução em binário:** `00000000011000000000001010010011`

**O que faz:** soma 6 ao valor de x0 (que é sempre 0) e guarda em x5. Resultado: x5 = 6.

**Passo a passo:**

1. **PC = 0** → InstrFile entrega `00000000011000000000001010010011`
2. **Control** decodifica opcode=`0010011` (I-type aritmético):
   - `reg_write = 1` (vai escrever em rd)
   - `alu_src = 1` (usar imediato, não rs2)
   - `alu_op = 0000` (ADD)
3. **ImmGen** extrai bits [31:20] = `000000000110` → imm = 6
4. **RegFile** lê rs1 = bits [19:15] = `00000` = x0 → rdata1 = 0
5. **MUX** com `alu_src=1` → seleciona imm=6 como entrada B da ULA
6. **ULA** computa: 0 + 6 = 6, `o_zero = 0`
7. **Mux writeback:** nenhum flag especial → usa resultado da ULA = 6
8. **Na borda de subida do clock:**
   - RegFile escreve 6 em rd = bits [11:7] = `00101` = x5 ✓
   - PC ← PC + 4 = 4

---

### 7.2 Exemplo: ADD x7, x5, x6

**Instrução em binário:** `00000000011000101000001110110011`

**O que faz:** soma x5 e x6 e guarda em x7. Com x5=6 e x6=4: x7 = 10.

**Passo a passo:**

1. **PC = 8** → InstrFile entrega a instrução ADD
2. **Control** decodifica opcode=`0110011` (R-type), funct3=`000`, funct7b5=`0`:
   - `reg_write = 1`
   - `alu_src = 0` (usar rs2, não imediato)
   - `alu_op = 0000` (ADD, porque funct7b5=0)
3. **ImmGen** não importa para R-type
4. **RegFile** lê rs1=x5 → 6, rs2=x6 → 4
5. **MUX** com `alu_src=0` → seleciona rs2=4 como entrada B
6. **ULA** computa: 6 + 4 = 10
7. **Na borda do clock:** x7 ← 10, PC ← 12

---

### 7.3 Exemplo: SW x5, 0(x0)

**Instrução em binário:** `00000000010100000010000000100011`

**O que faz:** escreve o valor de x5 (=6) na memória de dados no endereço x0+0 = 0.

**Passo a passo:**

1. **PC = 24** → instrução SW
2. **Control** decodifica opcode=`0100011` (S-type):
   - `reg_write = 0` (SW não escreve em registrador)
   - `alu_src = 1` (imediato para calcular endereço)
   - `mem_write = 1` (vai escrever na memória)
   - `alu_op = 0000` (ADD, para calcular rs1 + imm)
3. **ImmGen** extrai S-type: imm = 0
4. **RegFile** lê rs1=x0 → 0 (base do endereço), rs2=x5 → 6 (dado a guardar)
5. **MUX** `alu_src=1` → entrada B = imm = 0
6. **ULA** computa: 0 + 0 = 0 (esse é o endereço de memória)
7. **DataMem** recebe addr=0 e wdata=6
8. **Na borda do clock:** mem[0] ← 6, PC ← 28
9. **Mux writeback:** não importa, `reg_write=0` vai bloquear a escrita no RegFile

---

### 7.4 Exemplo: LW x11, 0(x0)

**Instrução em binário:** `00000000000000000010010110000011`

**O que faz:** lê o valor da memória no endereço x0+0 = 0 e guarda em x11. Com mem[0]=6: x11 = 6.

**Passo a passo:**

1. **PC = 28** → instrução LW
2. **Control** decodifica opcode=`0000011`:
   - `reg_write = 1`
   - `alu_src = 1` (imediato para calcular endereço)
   - `mem_read = 1`, `mem_to_reg = 1` (dado da memória vai para o registrador)
   - `alu_op = 0000` (ADD)
3. **ImmGen** extrai I-type: imm = 0
4. **RegFile** lê rs1=x0 → 0
5. **ULA** computa: 0 + 0 = 0 (endereço)
6. **DataMem** com addr=0 → lê mem[0] = 6 (leitura combinacional, instantânea)
7. **Mux writeback:** `mem_to_reg=1` → usa dado da memória = 6
8. **Na borda do clock:** x11 ← 6, PC ← 32

---

### 7.5 Exemplo: BEQ x5, x11, +8

**Instrução em binário:** `00000000101100101000010001100011`

**O que faz:** se x5 == x11, desvia para PC+8. Como x5=6 e x11=6, o desvio é tomado.

**Passo a passo:**

1. **PC = 32** → instrução BEQ
2. **Control** decodifica opcode=`1100011`, funct3=`000`:
   - `branch = 1`
   - `alu_op = 0001` (SUB — para testar igualdade)
   - `reg_write = 0` (branch não escreve em registrador)
3. **ImmGen** extrai B-type: imm = 8
4. **RegFile** lê rs1=x5 → 6, rs2=x11 → 6
5. **MUX** `alu_src=0` → usa rs2=6 como entrada B
6. **ULA** computa: 6 - 6 = 0 → `o_zero = '1'`
7. **Lógica de branch:** `branch=1` e funct3=`000` (BEQ) → `branch_taken = w_zero = '1'`
8. **u_PC_IMM** (adder) computa: 32 + 8 = 40
9. **Mux do PC_next:** `branch_taken=1` → `pc_next = w_pc_imm = 40`
10. **Na borda do clock:** PC ← 40 (pula para ADDI x13,x0,1), instrução no PC=36 **nunca executa**

---

## 8. O programa de teste

O `instrfile.vhd` contém 11 instruções que testam os principais tipos de instrução:

| PC | Instrução | Operação | Resultado esperado |
|---|---|---|---|
| 0  | `ADDI x5, x0, 6`    | x5 = 0 + 6     | **x5 = 6** |
| 4  | `ADDI x6, x0, 4`    | x6 = 0 + 4     | **x6 = 4** |
| 8  | `ADD  x7, x5, x6`   | x7 = 6 + 4     | **x7 = 10** |
| 12 | `SUB  x8, x7, x6`   | x8 = 10 - 4    | **x8 = 6** |
| 16 | `AND  x9, x5, x6`   | x9 = 0110 & 0100 | **x9 = 4** |
| 20 | `OR  x10, x5, x6`   | x10 = 0110 \| 0100 | **x10 = 6** |
| 24 | `SW  x5, 0(x0)`     | mem[0] = 6     | **mem[0] = 6** |
| 28 | `LW  x11, 0(x0)`    | x11 = mem[0]   | **x11 = 6** |
| 32 | `BEQ x5, x11, +8`   | 6==6 → pula    | **PC vai para 40** |
| 36 | `ADDI x12, x0, 99`  | *(não executa)* | **x12 = 0** |
| 40 | `ADDI x13, x0, 1`   | x13 = 1        | **x13 = 1** |
| 44+| NOP                 | —              | — |

A prova de que o branch funcionou é que **x12 nunca recebe 99** — o processador pulou direto para o PC=40.

---

## 9. O testbench

**Arquivo:** `testbench.sv`

O testbench é um ambiente de simulação. Ele não é hardware real — é um programa que **simula** o hardware para verificar se está correto.

**O que ele faz:**
1. Instancia o processador (`design.vhd`) como o componente `DUT` (Device Under Test)
2. Gera um clock de 10ns de período (5ns alto, 5ns baixo)
3. Aplica um pulso de reset no início (1 ciclo)
4. Aguarda 13 ciclos para o programa executar
5. Para a simulação

**O que observar no waveform:**
- `w_pc` — mostra qual instrução está executando (0, 4, 8, 12... e depois pula de 32 para 40)
- `w_ula` — mostra o resultado da ULA em cada ciclo (6, 4, 10, 6, 4, 6, 0, 0, 0, 1)
- `w_zero` — fica `'1'` no ciclo do BEQ (porque 6-6=0)

---

## 10. Como rodar no EDA Playground

1. Acesse [edaplayground.com](https://www.edaplayground.com) e faça login
2. Crie um novo playground ou abra o existente
3. **Testbench:** cole o conteúdo de `testbench.sv`
4. **Design files:** crie um arquivo para cada `.vhd` e cole o conteúdo correspondente
5. Em **Tools & Simulators**, selecione **GHDL 0.37** (ou versão disponível)
6. Marque **Open EPWave after run** para ver o waveform
7. Clique em **Run**
8. No EPWave, adicione os sinais `w_pc`, `w_ula` e `w_zero` ao waveform

**Dica:** se der erro de compilação, verifique se todos os arquivos foram criados corretamente e se o nome da entidade bate com o nome do arquivo.

---

## 11. Resultados esperados na simulação

| Tempo | Evento | w_pc | w_ula | w_zero |
|---|---|---|---|---|
| 0–10ns | Reset ativo | 0 | — | — |
| 10–20ns | ADDI x5, x0, 6 | 0 | 6 | 0 |
| 20–30ns | ADDI x6, x0, 4 | 4 | 4 | 0 |
| 30–40ns | ADD x7, x5, x6 | 8 | 10 | 0 |
| 40–50ns | SUB x8, x7, x6 | 12 | 6 | 0 |
| 50–60ns | AND x9, x5, x6 | 16 | 4 | 0 |
| 60–70ns | OR x10, x5, x6 | 20 | 6 | 0 |
| 70–80ns | SW x5, 0(x0)   | 24 | 0 | 1 |
| 80–90ns | LW x11, 0(x0)  | 28 | 0 | 1 |
| 90–100ns | BEQ x5, x11 | 32 | 0 | **1** |
| 100–110ns | ADDI x13,x0,1 | **40** | 1 | 0 |

> **Atenção:** no ciclo do BEQ (PC=32), `w_zero='1'` confirma que 6-6=0, e o PC vai para 40 (e não para 36), confirmando que o branch funcionou.

> **Atenção 2:** `w_ula` no ciclo de SW e LW mostra 0 porque a ULA calcula o endereço (rs1+imm = 0+0 = 0). O valor 6 está em `w_rs2_data` (para SW) e em `w_dmem_rdata` (para LW), que não são expostos como saídas, mas podem ser adicionados ao waveform se necessário.

---

## 12. Glossário

| Termo | Significado |
|---|---|
| **ISA** | Instruction Set Architecture — o "idioma" que o processador entende |
| **RISC-V** | ISA aberta e gratuita criada pela UC Berkeley |
| **Monociclo** | Cada instrução executa em exatamente 1 ciclo de clock |
| **VHDL** | Very High Speed Integrated Circuit Hardware Description Language — linguagem usada para descrever circuitos digitais |
| **Combinacional** | Circuito cuja saída depende apenas das entradas atuais (sem memória) |
| **Sequencial** | Circuito que tem estado interno e atualiza na borda do clock |
| **Datapath** | Caminho dos dados — os componentes que manipulam os valores |
| **Opcode** | Os 7 bits menos significativos da instrução, que indicam o tipo |
| **funct3 / funct7** | Campos extras da instrução que refinam o opcode (ex: ADD vs SUB) |
| **Imediato** | Valor constante embutido na instrução |
| **Extensão de sinal** | Copiar o bit de sinal para preencher bits superiores, preservando o valor com sinal |
| **Borda de subida** | Momento em que o clock vai de 0 para 1 — quando o estado muda |
| **RegFile** | Banco de registradores — 32 "variáveis" de 32 bits |
| **ULA / ALU** | Unidade Lógica e Aritmética — faz as contas |
| **Writeback** | A etapa final onde o resultado é gravado de volta no registrador destino |
| **Branch** | Desvio condicional — pula para outro endereço se uma condição for verdadeira |
| **PC** | Program Counter — registrador que guarda o endereço da instrução atual |
| **ROM** | Read-Only Memory — memória somente de leitura (instrfile) |
| **RAM** | Random Access Memory — memória de leitura e escrita (dmem) |
| **Waveform** | Gráfico que mostra como os sinais digitais variam ao longo do tempo na simulação |
| **Testbench** | Código de simulação que "testa" o hardware sem ser hardware real |
| **Top-level** | O componente principal que instancia todos os outros (design.vhd) |
| **Port map** | Em VHDL, a conexão entre os pinos de um componente e os sinais do circuito |
