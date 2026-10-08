# Conflitos de Dados e de Controle no Pipeline: Etapa 2

Avaliação Prática M2 - Organização de Computadores (UNIVALI)

Programa em C++17 que lê o mesmo arquivo de memória de instruções (ROM) da
Etapa 1, detecta os conflitos de dados e de controle de um pipeline de cinco
estágios sem proteção alguma e gera duas versões corrigidas do programa: uma só
com `nop`s e outra com reordenação de instruções. Os desvios são recalculados, os
arquivos de saída podem ser lidos de volta pelo decodificador da Etapa 1 e o
console mostra o custo de cada técnica.

Este README é também o **guia de estudo do trabalho, do zero, para quem nunca
viu o assunto**: o que é um pipeline, por que ele erra, como o programa
encontra e conserta cada erro e como responder às perguntas da defesa.

> Para só compilar e rodar, pule para [Como rodar](#13-como-rodar).

---

## Sumário

1. [O trabalho em uma frase](#1-o-trabalho-em-uma-frase)
2. [O pipeline de cinco estágios](#2-o-pipeline-de-cinco-estágios)
3. [Conflitos de dados e a regra da distância](#3-conflitos-de-dados-e-a-regra-da-distância)
4. [Conflitos de controle e o slot de atraso](#4-conflitos-de-controle-e-o-slot-de-atraso)
5. [Blocos básicos](#5-blocos-básicos)
6. [As premissas do enunciado](#6-as-premissas-do-enunciado)
7. [Detectando os conflitos (R1 e R2)](#7-detectando-os-conflitos-r1-e-r2)
8. [Corrigindo com nops (R3 e R4)](#8-corrigindo-com-nops-r3-e-r4)
9. [Reordenando as instruções (R5)](#9-reordenando-as-instruções-r5)
10. [Recalculando endereços e deslocamentos (R6)](#10-recalculando-endereços-e-deslocamentos-r6)
11. [Arquivos de saída e relatório de custo (R7 e R8)](#11-arquivos-de-saída-e-relatório-de-custo-r7-e-r8)
12. [Como o nosso programa está organizado](#12-como-o-nosso-programa-está-organizado)
13. [Como rodar](#13-como-rodar)
14. [Preparação para a defesa](#14-preparação-para-a-defesa)
15. [Glossário](#15-glossário)

---

## 1. O trabalho em uma frase

> Escrever um programa que lê o código de um programa RISC-V e o **reescreve**
> para que ele dê o resultado certo num processador que executa várias
> instruções ao mesmo tempo e não se protege de nada.

O processador-alvo é um pipeline "pelado": não adianta valores, não detecta
conflitos, não segura instruções. Quem faz o papel dessas proteções é o
**programa**, trocando o código de entrada por outro, equivalente, que funciona
nesse processador.

A Etapa 1 já entregou o que isto precisa: de cada instrução, **quais registradores
ela lê e qual ela escreve** (`readsRegisters()` e `writesRegister()`) e o destino
dos desvios (`branchTarget()`). A Etapa 2 usa esses três ganchos sem alterar o
decodificador.

**Por que a Etapa 1 precisava ser precisa:** se o decodificador informasse um
`rd` inexistente num `beq` ou num `sw`, a Etapa 2 acusaria conflitos que não
existem. É por isso que tipos S e B têm `rd` vazio de verdade.

---

## 2. O pipeline de cinco estágios

Em vez de terminar uma instrução inteira antes de começar a próxima, o processador
trabalha como uma linha de montagem: cada instrução passa por cinco estágios, e
enquanto uma está no segundo, a seguinte já está no primeiro.

| Estágio | O que faz | Analogia (lavanderia) |
|---|---|---|
| **IF** | Busca a instrução na memória | Pegar a roupa no cesto |
| **ID** | Decodifica e **lê os registradores**; desvios são resolvidos aqui | Separar e conferir a etiqueta |
| **EX** | Executa a operação (ULA) | Lavar |
| **MEM** | Acessa a memória (load e store) | Secar |
| **WB** | **Escreve** o resultado no registrador | Guardar no armário |

```
ciclo:            1    2    3    4    5    6    7
instrução 1       IF   ID   EX   MEM  WB
instrução 2            IF   ID   EX   MEM  WB
instrução 3                 IF   ID   EX   MEM  WB
```

Com isso, em regime, **uma instrução termina por ciclo**. O problema é que a
instrução 2 já está lendo registradores (ID) enquanto a instrução 1 ainda não
escreveu o resultado (WB).

---

## 3. Conflitos de dados e a regra da distância

### O erro

```
lw  t0, 0(a0)       # t0 só é escrito no estágio WB (ciclo 5)
add t1, t0, s3      # lê t0 no estágio ID (ciclo 3): pega o valor VELHO
```

```
ciclo:             1    2    3    4    5    6
lw  t0, 0(a0)      IF   ID   EX   MEM  WB
add t1, t0, s3          IF   ID   EX   MEM  WB
                             ↑              ↑
                       lê t0 aqui     escreve t0 só aqui
```

Isso se chama **conflito de dados** (*data hazard*). Mais precisamente, é uma
**leitura após escrita** (RAW): a instrução que lê depende da que escreve.

### A regra da distância

O banco de registradores é **escrito na primeira metade do ciclo e lido na
segunda**. Então, se o WB do produtor cai no mesmo ciclo do ID do consumidor, o
consumidor já enxerga o valor novo.

Chamamos de **d** a distância entre produtor e consumidor (1 = instruções
vizinhas).

| d | O que acontece | Nops necessários |
|:-:|---|:-:|
| 1 | O consumidor lê dois ciclos antes da escrita | **2** |
| 2 | O consumidor lê um ciclo antes da escrita | **1** |
| 3 ou mais | O WB do produtor coincide com o ID do consumidor, ou vem antes | 0 |

> **Regra:** conflito quando **d = 1 ou d = 2**, e são necessários **3 − d** nops.

Um `nop` é a instrução `addi zero, zero, 0` (`0x00000013`): ocupa um lugar no
pipeline e não faz nada.

```
lw  t0, 0(a0)
nop
nop
add t1, t0, s3      # agora d = 3: o WB do lw coincide com o ID do add
```

### O que conta e o que não conta

| Situação | Gera conflito? | Por quê |
|---|:-:|---|
| Leitura após escrita (RAW) | ✅ | O consumidor pega o valor velho |
| Escrita após leitura (WAR) | - | Num pipeline em ordem a leitura já aconteceu |
| Escrita após escrita (WAW) | - | As escritas saem na ordem do programa |
| Registrador `zero` (`x0`) | - | Seu valor nunca muda |
| Produtor que não é o mais recente | - | Vale sempre o **mais recente** do registrador |

WAR e WAW não geram conflito, mas **proíbem trocar a ordem** das duas
instruções. Isso volta na seção [9](#9-reordenando-as-instruções-r5).

### ⚠️ O erro mais comum

Tratar o campo `rd` de um formato que não o tem. Um `sw` (tipo S) e um `beq`
(tipo B) **leem** dois registradores e **não escrevem** nenhum. Se o código
considerasse um `rd` neles, acusaria conflitos que não existem. A Etapa 1 já
deixa esses campos vazios (`std::nullopt`), e a Etapa 2 só precisa perguntar:

```cpp
for (uint8_t reg : items[c].inst.readsRegisters()) { ... }   // o que a instrução lê
writes(items[q].inst) == reg                                  // o que o produtor escreve
```

---

## 4. Conflitos de controle e o slot de atraso

### O erro

O desvio só é resolvido no estágio ID. Nesse momento, a instrução seguinte **já
foi buscada** (está no IF). O que fazer com ela?

```
ciclo:              1    2    3    4
beq t0, t1, alvo    IF   ID   EX   MEM
instrução seguinte       IF   ...        ← já entrou no pipeline
```

A premissa 4 do enunciado resolve assim: essa instrução, o **slot de atraso**
(*delay slot*), **sempre executa**, o desvio sendo tomado ou não.

### Por que isso muda o código

O programa original foi escrito pensando que, **se o desvio for tomado, a
instrução seguinte não executa**. No pipeline, ela executa. Isso quebra o
programa, e a correção é preencher o slot:

1. com um `nop` (R4, a solução simples), ou
2. com uma instrução **útil que veio de antes do desvio** (R5).

> **Por que só uma instrução de antes do desvio pode ir para o slot?** Porque ela
> já executava nos **dois** caminhos. Uma instrução de depois do desvio, ou do
> alvo, executaria só em um deles e quebraria o outro.

### Efeito colateral nos dados

O slot fica entre o desvio e o início dos dois caminhos. Então o produtor de um
registrador que está antes do desvio pode estar a **1 ou 2 posições** do
consumidor que está logo depois, e isso é um conflito de dados que **atravessa a
fronteira** do desvio. O enunciado pede para detectá-lo (R2), e o programa o
mostra numa seção própria.

---

## 5. Blocos básicos

Um **bloco básico** é um trecho de instruções que sempre executa de ponta a
ponta: não tem desvio no meio e não tem entrada no meio.

- Todo **alvo** de desvio **inicia** um bloco.
- Toda **instrução de desvio encerra** um bloco.

Exemplo (`laco.s`):

```
        addi a1, zero, 4
        addi t2, zero, 0
loop:   lw   t0, 0(a0)        ┐
        add  t2, t2, t0       │ bloco 1 (começa no alvo `loop`,
        addi a0, a0, 4        │ termina no desvio)
        addi a1, a1, -1       │
        bne  a1, zero, loop   ┘
        sw   t2, 0(s0)          bloco 2 (logo após o desvio)
```

O bloco 0 são as duas primeiras instruções. Por que isso importa? A
reordenação só é segura **dentro** de um bloco: dentro dele todas as instruções
executam, na mesma situação. A única instrução que pode atravessar o desvio é a
do slot.

---

## 6. As premissas do enunciado

Todas as decisões do programa seguem estas sete regras.

| # | Premissa | Onde aparece no código |
|:-:|---|---|
| 1 | Pipeline de cinco estágios (IF, ID, EX, MEM, WB) | Modelo inteiro |
| 2 | Sem adiantamento, detecção de conflitos, descarte ou predição | O programa faz o papel desses mecanismos |
| 3 | Escrita na 1ª metade do ciclo, leitura na 2ª: conflito com d = 1 ou 2, nops = 3 − d | `findHazards`, `need` em `insertDataNops` |
| 4 | Desvio resolvido no ID; o slot de atraso sempre executa | `successors`, `buildItems` |
| 5 | `x0` é constante: escrita descartada, leitura nunca conflita | `readsRegisters`/`writesRegister` da Etapa 1 |
| 6 | Mesmo registrador-base com deslocamentos diferentes = endereços distintos | `depends` |
| 7 | `nop` é `addi zero, zero, 0` = `0x00000013` | `kNopWord` |

---

## 7. Detectando os conflitos (R1 e R2)

### A ideia central: o fluxo de execução

O programa é visto como uma sequência de posições e a pergunta é: **quem executa
logo depois de cada uma?**

- Normalmente, a próxima posição (`i + 1`).
- Depois de um desvio ou salto vem **sempre o slot**, e só então o fluxo se
  divide: o **alvo** e (se o desvio for condicional) a **instrução seguinte**.

```cpp
std::vector<int> successors(const std::vector<Item>& items, const std::vector<int>& start, int i) {
    std::vector<int> out;
    if (i > 0 && items[i - 1].isControl()) {            // i é o slot de atraso
        const Item& branch = items[i - 1];
        if (branch.targetBlock >= 0) out.push_back(start[branch.targetBlock]);   // tomado
        if (branch.inst.cls == Class::Branch && i + 1 < n) out.push_back(i + 1); // não tomado
    } else if (i + 1 < n) {
        out.push_back(i + 1);
    }
    return out;
}
```

Com essa relação, "consumidor a distância d do produtor" vale **por qualquer
caminho**, inclusive atravessando desvios, e **a mesma função** serve para
detectar os conflitos do original (R1, R2) e para **verificar** o resultado
final.

### R1: conflitos de dados

Para cada instrução `c` e cada registrador `r` que ela lê:

1. Para cada predecessor `q1` de `c` (distância 1): se `q1` escreve `r`, é
   conflito com d = 1 e a busca naquele caminho **termina** (o produtor mais
   recente é `q1`).
2. Senão, para cada predecessor `q2` de `q1` (distância 2): se `q2` escreve `r`,
   é conflito com d = 2.

A saída é a do enunciado:

```
CONFLITOS DE DADOS
  0x00400004  add   t1, t0, s3     <- 0x00400000  lw    t0, 0(a0)     [t0, d=1, 2 nops]
```

### R2: conflitos de controle

- Lista cada desvio e a instrução que ocupa o seu slot.
- Separa os conflitos de dados cujo caminho **sai de um slot** (`crossesBranch`):
  são os que atravessam a fronteira do desvio.
- Imprime os blocos básicos, com o endereço da primeira e da última instrução.

---

## 8. Corrigindo com nops (R3 e R4)

### R4: o slot

Depois de **cada** desvio e salto entra um `nop` (`Tag::NopSlot`). A instrução
que era a seguinte no original passa a ser a primeira do bloco seguinte.

### R3: nops de dados

O algoritmo é um laço simples:

1. Calcula todos os conflitos da sequência atual.
2. Pega o consumidor **mais cedo** no programa e descobre quantos nops ele pede
   (o maior `3 − d` entre os conflitos dele).
3. Insere esses nops **imediatamente antes do consumidor**.
4. Volta ao passo 1, até não sobrar conflito.

```cpp
const int at = leavesSlot(items, consumer) ? consumer - 1 : consumer;
items.insert(items.begin() + at, need, makeNop(block, Tag::NopData));
```

Três detalhes que costumam cair na defesa:

- **Por que antes do consumidor?** Em código reto dá no mesmo que depois do
  produtor. Mas antes do consumidor serve a **todos os caminhos** que chegam
  nele, inclusive um desvio que aponta para ele. Se o consumidor é um alvo, o
  desvio passa a apontar para o **primeiro nop**.
- **Por que recalcular a cada inserção?** Um nop inserido para um consumidor
  pode aumentar a distância de outros. Se inserisse tudo de uma vez, contaria nops
  que não são necessários.
- **E se o consumidor é o slot?** Os nops vão **antes do desvio**: entre o desvio
  e o slot não cabe nada.

### Exemplo: `laco.s` só com nops (8 → 13 instruções)

```
0x00400008  lw    t0, 0(a0)
0x0040000C  nop                       <- nop inserido (conflito de dados)
0x00400010  nop                       <- nop inserido (conflito de dados)
0x00400014  add   t2, t2, t0
0x00400018  addi  a0, a0, 4
0x0040001C  addi  a1, a1, -1
0x00400020  nop                       <- nop inserido (conflito de dados)
0x00400024  nop                       <- nop inserido (conflito de dados)
0x00400028  bne   a1, zero, 0x00400008
0x0040002C  nop                       <- nop no slot de atraso
0x00400030  sw    t2, 0(s0)
```

---

## 9. Reordenando as instruções (R5)

Em vez de preencher a distância com `nop`, usa-se uma instrução **independente**
que estava em outro lugar do mesmo bloco.

### Quais instruções podem trocar de lugar

Duas instruções `i` (antes) e `j` (depois) **não** podem trocar quando há:

| Dependência | Quando | Por quê |
|---|---|---|
| Leitura após escrita (RAW) | `i` escreve o que `j` lê | `j` precisa do valor novo |
| Escrita após leitura (WAR) | `i` lê o que `j` escreve | `i` precisa do valor velho |
| Escrita após escrita (WAW) | os dois escrevem o mesmo registrador | o último a escrever vence |
| Memória | um dos dois é store e não se sabe que os endereços são distintos | podem ser o mesmo endereço |
| Barreira | `ecall`, CSR, `fence` ou palavra inválida | efeitos fora dos registradores |

Na memória, a premissa 6 libera um caso: **mesmo registrador-base e
deslocamentos diferentes** são endereços distintos, desde que a base não mude
entre as duas.

```cpp
bool distinct = a.rs1 == b.rs1 && a.imm != b.imm;
for (int k = i; distinct && k < j; ++k) {
    if (writes(ctx.prog[k]) == *a.rs1) distinct = false;   // a base mudou no meio
}
```

### Escolhendo a ordem

Dentro de cada bloco, o programa procura a ordem com **menos nops**, por busca
exata com memoização. O estado é:

> (conjunto de instruções já colocadas, o que a última escreve, o que a penúltima escreve)

Colocar uma instrução custa **2** nops se ela lê o que a última escreve, **1** se
lê o que a penúltima escreve, e 0 caso contrário. Em empate, fica a ordem
original. Acima de 14 instruções no bloco, usa-se um algoritmo guloso.

O desvio depende de **todas** as instruções do bloco, então fica sempre por
último.

### O slot de atraso

Uma instrução do bloco pode ocupar o slot se **nenhuma outra instrução do bloco,
nem o próprio desvio, depende dela**:

```cpp
for (int y = x + 1; y <= b.last && free; ++y) free = !depends(ctx, x, y);
```

Isso exclui, por exemplo, o `addi a1, a1, -1` do `laco.s`: o `bne` lê `a1`.

> **Preencher o slot costuma criar um conflito novo.** No `laco.s`, mover
> `addi a0, a0, 4` para o slot deixaria o `lw t0, 0(a0)` do início do laço a uma
> posição de distância do seu produtor (d = 1) e custaria **2** nops, mais do que
> o nop do slot. Por isso o programa testa **cada candidata contra o programa
> inteiro** (com os nops de dados já inseridos) e só preenche o slot se isso
> reduzir o total. O slot com `nop` só é mantido quando nenhuma candidata ganha.

### Exemplo: `sem_nops.s` (9 → 15 com nops, 9 com reordenação)

```
addi t0, zero, 1                 addi t0, zero, 1
add  t1, t0, t0   # d=1          addi t2, zero, 2
addi t2, zero, 2                 addi t4, zero, 3
add  t3, t2, t2   # d=1   ──▶    add  t1, t0, t0   # d=3
addi t4, zero, 3                 add  t3, t2, t2   # d=3
add  t5, t4, t4   # d=1          add  t5, t4, t4   # d=3
sw ... (3 stores)                sw ... (3 stores)
```

Intercalando os três pares produtor/consumidor, cada consumidor fica a
distância 3 e **nenhum nop** é necessário.

### Quando não há o que reordenar

Numa **cadeia** (`cadeia.s`), cada instrução usa o resultado da anterior. Não há
instrução independente para preencher o espaço, e o resultado da reordenação é
**igual** ao de só nops (5 → 13).

---

## 10. Recalculando endereços e deslocamentos (R6)

Cada nop inserido e cada instrução movida **desloca todas as seguintes**. Um
desvio que dizia "volte 16 bytes" pode agora precisar de "volte 32". O programa
guarda o alvo de cada desvio como um **bloco**, não como um endereço, e só no fim,
com a sequência final fixada, converte para número:

```
novo deslocamento = endereço novo do alvo − endereço novo do desvio
```

Depois é preciso **reescrever os bits**. Como na Etapa 1, os imediatos de B e J
são embaralhados, então a montagem inversa usa as mesmas posições:

```cpp
// Tipo B: imm[12|10:5|4:1|11]
const uint32_t imm = ((u >> 12 & 1u) << 31) | ((u >> 5 & 0x3Fu) << 25) |
                     ((u >> 1 & 0xFu) << 8) | ((u >> 11 & 1u) << 7);
return (raw & 0x01FFF07Fu) | imm;      // mantém opcode, funct3, rs1 e rs2
```

Casos especiais tratados:

- **Desvio para logo depois da última instrução** acompanha o novo fim do
  programa.
- **Destino fora do programa** mantém o endereço absoluto.
- **Fora do alcance** (±4 KiB no tipo B, ±1 MiB no tipo J) é erro, com mensagem.

### Exemplo: o `bne` do `laco.s`

| | Endereço do `bne` | Alvo (`lw`) | Deslocamento | Palavra |
|---|---|---|---|---|
| Original | `0x00400018` | `0x00400008` | −16 | `0xFE0598E3` |
| Só nops | `0x00400028` | `0x00400008` | −32 | `0xFE0590E3` |

Para conferir o resultado, os arquivos de saída podem ser lidos de volta pelo
`riscv-decoder` da Etapa 1: se o desvio foi reescrito errado, o alvo mostrado
será outro.

---

## 11. Arquivos de saída e relatório de custo (R7 e R8)

### R7: dois arquivos, mais uma listagem de cada

Para a entrada `prog.hex`:

| Arquivo | Conteúdo |
|---|---|
| `prog_nops.hex` (ou `.bin`) | Programa corrigido só com nops, no formato da entrada |
| `prog_nops.lst` | Listagem: endereço, código, instrução e marcação |
| `prog_reordenado.hex` (ou `.bin`) | Programa corrigido com reordenação e nops |
| `prog_reordenado.lst` | A mesma listagem para a versão reordenada |

A marcação da listagem diz o que cada linha é:

| Marcação | Significado |
|---|---|
| (vazia) | Instrução do original, na mesma ordem |
| `<- reordenada (era 0x...)` | Instrução do original, em outra posição do bloco |
| `<- movida para o slot de atraso (era 0x...)` | Instrução do original colocada no slot |
| `<- nop inserido (conflito de dados)` | Nop de R3 |
| `<- nop no slot de atraso` | Nop de R4 |

### R8: relatório de custo

```
CUSTO
  original ..........   9 instrucoes
  somente nops ......   17 instrucoes  (+8, +88,9%)
  nops + reordenacao    14 instrucoes  (+5, +55,6%)

Comparacao: a reordenacao evitou 3 de 8 nops (37,5%).
```

---

## 12. Como o nosso programa está organizado

A Etapa 2 **acrescenta** arquivos à Etapa 1. Nada do decodificador foi alterado.

| Arquivo | Responsabilidade |
|---|---|
| `include/riscv.hpp` | (Etapa 1) `Instruction` e as assinaturas do decodificador |
| `src/parser.cpp`, `decoder.cpp`, `disassembler.cpp`, `statistics.cpp` | (Etapa 1) leitura da ROM, decodificação, assembly e CPI |
| `src/main.cpp` | (Etapa 1) executável `riscv-decoder` |
| `include/hazards.hpp` | **Novo.** `Item`, `Tag`, `DataHazard`, `Analysis`, `Corrected` e as funções públicas |
| `src/hazards.cpp` | **Novo.** Blocos, fluxo, detecção, nops, escalonador, slot e recálculo de desvios |
| `src/main_m2.cpp` | **Novo.** Executável `riscv-hazards`: console, arquivos de saída e custo |
| `tests/test_decoder.cpp` | (Etapa 1) testes do decodificador |
| `tests/test_hazards.cpp` | **Novo.** Emulador RV32I, casos conhecidos, fuzz e busca exaustiva |
| `tests/asm.py` | **Novo.** Montador mínimo que gera os `.hex` a partir dos `.s` |
| `tests/data/*.s` e `*.hex` | **Novo.** Casos de teste: `cadeia`, `laco`, `frente`, `sem_nops`, `enunciado` |

### Estrutura de dados central

```cpp
struct Item {
    Instruction inst;        // decodificada (a posição final está em inst.pc)
    int orig;                // índice na entrada; -1 para nop inserido
    int block;               // bloco básico
    int targetBlock;         // bloco de destino do desvio (-1 = nenhum; -2 = fim do programa)
    Tag tag;                 // original, reordenada, movida para o slot, nop de dados, nop de slot
};
```

O mesmo tipo representa o programa de entrada e os de saída, e o alvo de um desvio
é guardado como **bloco**, e não como endereço. Por isso inserir um nop não
quebra nada: o endereço só é calculado no final.

### Fluxo do programa

```
ROM  ──▶  parseStream  ──▶  decode (Etapa 1)  ──▶  analyze  ──▶  conflitos (console)
                                   │
                                   ├──▶ correct(reorder = false) ──▶ _nops.hex + .lst
                                   └──▶ correct(reorder = true)  ──▶ _reordenado.hex + .lst
                                                      └──▶ findHazards no resultado (deve dar 0)
```

---

## 13. Como rodar

Requisitos: compilador C++17 e CMake 3.16 ou superior.

```bash
cmake -S . -B build -G Ninja          # ou -G "MinGW Makefiles" / "Unix Makefiles"
cmake --build build
```

Executar o programa da Etapa 2:

```bash
build/riscv-hazards tests/data/enunciado.hex -b 0x00400000 -o saida
```

| Opção | Efeito |
|---|---|
| `<arquivo-rom>` | Programa em hexadecimal ou binário (sem argumento, o programa pergunta) |
| `-b`, `--base <endereço>` | Endereço-base do PC (padrão `0x00000000`) |
| `-o`, `--out <pasta>` | Pasta dos arquivos de saída (padrão `.`) |
| `-h`, `--help` | Ajuda |

Executar os testes:

```bash
build/test_decoder        # Etapa 1
build/test_hazards        # Etapa 2 (o fuzz leva alguns minutos)
```

O que `test_hazards` verifica:

1. **Equivalência de comportamento.** Um emulador RV32I tem duas máquinas: a
   **ideal** (sem conflitos, sem slot) e o **pipeline do enunciado** (leitura
   defasada de 3 posições e slot de atraso). O original na máquina ideal tem de dar
   o mesmo estado final (registradores e memória) que cada saída no pipeline.
   Passou em 1500 programas aleatórios (laços, desvios para frente, `jal`, loads
   e stores com bases diferentes que apontam para a mesma região), e 1306
   deles **mudam de resultado se não forem corrigidos**: o teste detecta erro de
   verdade.
2. **Sem conflitos restantes.** A detecção roda de novo sobre cada saída.
3. **Otimalidade.** Em 400 trechos retos curtos, uma busca exaustiva independente
   de todas as permutações válidas confirma que o tamanho é sempre o mínimo.
4. **Codificação de desvios.** Reescreve deslocamentos e decodifica de volta.

Para gerar os `.hex` a partir de um `.s`:

```bash
python tests/asm.py tests/data/laco.s > tests/data/laco.hex
```

### Resultados nos casos de teste

| Caso | O que cobre | Original | Só nops | Reordenado |
|---|---|---:|---:|---:|
| `cadeia` | Cadeia de dependências sem instrução independente | 5 | 13 | 13 |
| `sem_nops` | Reordenação elimina todos os nops | 9 | 15 | **9** |
| `laco` | Laço com desvio para trás | 8 | 13 | 9 |
| `frente` | Desvio para frente | 7 | 14 | 14 |
| `enunciado` | Laço no estilo do exemplo do enunciado | 9 | 17 | 14 |

---

## 14. Preparação para a defesa

Perguntas prováveis, com a resposta curta:

**1. Por que o conflito é com d = 1 ou 2, e não com d = 3?**
O produtor escreve no WB, na primeira metade do ciclo; o consumidor lê no ID, na
segunda. Com d = 3 os dois caem no mesmo ciclo e já funciona. Com d = 1 ou 2 o
consumidor lê antes da escrita.

**2. Quantos nops cada distância pede?**
3 − d: dois para d = 1, um para d = 2.

**3. Por que o nop vai antes do consumidor e não depois do produtor?**
Em código reto dá no mesmo, mas antes do consumidor serve a todos os caminhos que
chegam nele, inclusive um desvio que aponta para ele.

**4. O que acontece com um desvio que aponta para um consumidor que ganhou nops?**
O desvio passa a apontar para o primeiro nop. Senão o caminho "tomado" pularia
os nops e o conflito voltaria.

**5. Por que o slot de atraso existe e por que ele sempre executa?**
O desvio é resolvido no ID, e a instrução seguinte já foi buscada. A premissa 4
manda executá-la sempre.

**6. Por que o programa original quebra no pipeline?**
Porque foi escrito supondo que a instrução depois do desvio não executa quando ele é
tomado. Por isso entra um nop (ou uma instrução útil) logo depois de cada desvio.

**7. Por que só uma instrução de antes do desvio pode ir para o slot?**
Ela já executava nos dois caminhos. Uma de depois, ou do alvo, executaria só em
um deles.

**8. O que impede mover uma instrução para o slot?**
Se o desvio lê o registrador que ela escreve (ou, num `jal`, escreve o que ela
lê), ou se outra instrução do bloco depende dela, o resultado muda. Mesmo quando é
válido, pode criar um conflito novo no início dos dois caminhos.

**9. Duas instruções de memória podem trocar de ordem?**
Só se o mesmo registrador-base e deslocamentos diferentes valerem (premissa 6) e a
base não mudar entre elas. Qualquer outro par com um store mantém a ordem.

**10. Como o programa sabe que o resultado está certo?**
Roda a detecção de novo e exige 0 conflitos, e os testes comparam a execução do
original numa máquina ideal com a do corrigido num emulador do pipeline.

**11. A reordenação é ótima?**
Dentro de cada bloco a busca é exata (até 14 instruções; acima disso é gulosa). A
escolha do slot é feita bloco a bloco. Nos 400 testes de força bruta o tamanho foi
sempre o mínimo.

**12. Por que o alvo do desvio é guardado como bloco e não como endereço?**
Porque os endereços mudam a cada nop inserido. O endereço só é calculado no final,
com a sequência fixada.

**13. Quais as limitações?**
`jalr` não tem alvo conhecido (o slot é tratado, o destino não). `auipc` é relativo
ao PC e muda quando entram nops (o programa avisa). Acessos de tamanhos diferentes
que se sobrepõem, com a mesma base e deslocamentos diferentes, seguem a premissa 6.
`jal` e `jalr` têm slot de atraso como os desvios condicionais.

---

## 15. Glossário

| Termo | Significado |
|---|---|
| **Pipeline** | Execução de várias instruções ao mesmo tempo, cada uma num estágio |
| **Estágios** | IF (busca), ID (decodifica e lê registradores), EX (executa), MEM (memória), WB (escreve) |
| **Hazard / conflito** | Situação em que o pipeline daria resultado errado |
| **RAW** | *Read after write*: leitura após escrita; o único conflito de dados verdadeiro |
| **WAR / WAW** | Escrita após leitura / escrita após escrita; não geram conflito, mas proíbem trocar a ordem |
| **Distância (d)** | Posições entre o produtor e o consumidor de um registrador |
| **Produtor / consumidor** | Instrução que escreve um registrador / instrução que o lê |
| **Nop** | `addi zero, zero, 0` (`0x00000013`): ocupa um lugar sem fazer nada |
| **Slot de atraso** | Instrução logo depois de um desvio; sempre executa |
| **Bloco básico** | Trecho sem desvio no meio e sem entrada no meio |
| **Fluxo de execução** | Quem executa logo depois de cada instrução |
| **Adiantamento (forwarding)** | Hardware que passa o resultado direto, sem esperar o WB; **não existe** aqui |
| **Reordenação** | Trocar a ordem de instruções independentes para preencher a distância sem nops |
| **Deslocamento** | Distância, em bytes, entre um desvio e o seu alvo |
| **Barreira** | Instrução que não pode ser movida (`ecall`, CSR, `fence`, palavra inválida) |
| **Equivalência de comportamento** | O corrigido, no pipeline, deixa os mesmos registradores e a mesma memória que o original numa máquina ideal |
