// Deteccao e correcao de conflitos de pipeline (Pratica M2) -- interface publica.
//
// Autores: Jose Gabriel Santos Gomes, Matheus Pompeo Dias e Nathan Gustavo Reichert
//
// Entrada: o programa ja decodificado pela M1. Saida: duas versoes corrigidas
// (so com nops; com reordenacao + nops), em linguagem de maquina.
//
// Modelo (mesmas premissas do enunciado): pipeline de 5 estagios sem
// adiantamento; consumidor a distancia d = 1 ou 2 do produtor sofre conflito e
// precisa de 3 - d nops; desvios e saltos resolvem no ID e tem um slot de
// atraso sempre executado; x0 nunca gera conflito.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "riscv.hpp"

namespace riscv {

// Origem de cada posicao do programa final (usada no marcador da listagem).
enum class Tag {
    Original,   // instrucao do programa de entrada, na mesma ordem relativa
    Reordered,  // instrucao do programa de entrada, em outra posicao do bloco
    MovedSlot,  // instrucao do programa de entrada movida para o slot de atraso
    NopData,    // nop inserido para resolver conflito de dados
    NopSlot,    // nop que preenche o slot de atraso
};

// Uma posicao do programa. O mesmo tipo representa o programa de entrada
// (todas Original) e os programas corrigidos.
struct Item {
    Instruction inst;         // decodificada; pc e raw refletem a posicao final
    int orig = -1;            // indice na entrada; -1 para nops inseridos
    int block = 0;            // bloco basico a que a posicao pertence
    int targetBlock = -1;     // bloco de destino de B/jal; -1 = nenhum/externo; -2 = fim do programa
    bool hasExternal = false;  // destino fora do programa: o endereco absoluto e mantido
    uint32_t externalTarget = 0;
    Tag tag = Tag::Original;
    std::vector<uint8_t> reads;  // registradores lidos (sem x0), conforme as Options
    uint8_t wr = 0;              // registrador escrito (0 = nenhum)

    bool isControl() const;  // desvio condicional ou salto (jal/jalr)
};

// Escolhas de modelagem que o enunciado deixa em aberto.
struct Options {
    // O decodificador da M1 trata ecall sem registradores (ele e atendido pelo
    // ambiente). Com true, o ecall passa a ler a7 e a0 e a escrever a0, como um
    // consumidor/produtor comum do pipeline: mais conservador, com mais nops.
    bool ecallRegisters = false;
};

struct Block {
    int first = 0;  // indices na entrada, inclusive
    int last = 0;
};

// Dependencia verdadeira (leitura apos escrita) que viola a distancia minima.
struct DataHazard {
    int producer = 0;  // indices em Analysis::items
    int consumer = 0;
    uint8_t reg = 0;
    int distance = 0;          // 1 ou 2
    bool crossesBranch = false;  // o caminho produtor->consumidor passa pela resolucao de um desvio
    int nops() const { return 3 - distance; }
};

struct ControlSlot {
    int branch = 0;  // indice da instrucao de desvio/salto
    int slot = -1;   // indice da instrucao que ocupa o slot; -1 se o programa termina no desvio
};

struct Analysis {
    std::vector<Block> blocks;
    std::vector<Item> items;  // o programa de entrada, ja com os blocos e destinos resolvidos
    std::vector<DataHazard> hazards;
    std::vector<ControlSlot> controls;
};

// R1 e R2: blocos basicos, conflitos de dados (inclusive os que atravessam
// desvios) e slots de atraso do programa original.
Analysis analyze(const std::vector<Instruction>& program, uint32_t baseAddress, const Options& options = {});

struct Corrected {
    std::vector<Item> items;
    std::vector<uint32_t> words;  // linguagem de maquina, desvios ja recalculados (R6)
    std::size_t nopsData = 0;
    std::size_t nopsSlot = 0;
    std::size_t movedToSlot = 0;
    std::size_t reordered = 0;
    std::size_t residualHazards = 0;  // deve ser 0; verificado sobre o resultado final
    // Custo dinamico estimado: cada posicao pesa 10^(laços que contem o bloco),
    // pois um nop dentro de um laco executa a cada volta (dica da lista de exercicios).
    std::size_t weightedCost = 0;
    std::size_t weightedNops = 0;
};

// R3 + R4 (reorder = false) ou R5 (reorder = true); ambos incluem R6.
// Lanca std::runtime_error se um deslocamento recalculado nao couber na instrucao.
Corrected correct(const std::vector<Instruction>& program, uint32_t baseAddress, bool reorder,
                  const Options& options = {});

// Conflitos de dados remanescentes em uma sequencia (usado para verificar o resultado).
std::vector<DataHazard> findHazards(const std::vector<Item>& items);

// Reescreve o deslocamento de um desvio (B) ou salto (jal) ja codificado.
uint32_t withBranchOffset(uint32_t raw, Format format, int32_t offset);

}  // namespace riscv
