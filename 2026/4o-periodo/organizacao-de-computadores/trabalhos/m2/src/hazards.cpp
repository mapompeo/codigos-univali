// Deteccao e correcao de conflitos de pipeline (Pratica M2).
//
// Autores: Jose Gabriel Santos Gomes, Matheus Pompeo Dias e Nathan Gustavo Reichert
//
// Estrutura do arquivo:
//   1. predicados e utilitarios sobre instrucoes
//   2. blocos basicos e itens (R2)
//   3. sucessores/predecessores no fluxo de execucao e deteccao (R1, R2)
//   4. insercao de nops de dados (R3)
//   5. escalonamento dentro do bloco e preenchimento do slot (R5)
//   6. recalculo dos deslocamentos (R6) e funcao principal (correct)

#include "hazards.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <set>
#include <stdexcept>
#include <tuple>
#include <unordered_map>

namespace riscv {
namespace {

// ---------------------------------------------------------------------------
// 1. Predicados
// ---------------------------------------------------------------------------

constexpr uint32_t kNopWord = 0x00000013;  // addi zero, zero, 0
constexpr int kProgramEnd = -2;            // Item::targetBlock: fim do programa

bool controlInst(const Instruction& i) { return i.cls == Class::Branch || i.cls == Class::Jump; }

// Instrucoes que nunca se movem: sistema (ecall, csr, fence) e palavras invalidas
// (podem ser dados). Dependem de tudo e tudo depende delas.
bool barrierInst(const Instruction& i) { return !i.valid() || i.cls == Class::System; }

bool memInst(const Instruction& i) { return i.cls == Class::Load || i.cls == Class::Store; }

// Registrador escrito (0 = nenhum). Escritas em x0 ja vem descartadas.
uint8_t writes(const Instruction& i) {
    const auto w = i.writesRegister();
    return w ? *w : 0;
}

bool contains(const std::vector<uint8_t>& regs, uint8_t r) {
    return r != 0 && std::find(regs.begin(), regs.end(), r) != regs.end();
}

Item makeNop(int block, Tag tag) {
    Item item;
    item.inst = decode(kNopWord, 0);
    item.block = block;
    item.tag = tag;
    return item;
}

// ---------------------------------------------------------------------------
// 2. Blocos basicos
// ---------------------------------------------------------------------------

struct Context {
    const std::vector<Instruction>& prog;
    uint32_t base;
    std::vector<Block> blocks;
    std::vector<int> blockOf;  // bloco de cada instrucao da entrada

    int indexOf(uint32_t address) const {
        if (address < base || (address - base) % 4 != 0) return -1;
        const uint32_t index = (address - base) / 4;
        return index < prog.size() ? static_cast<int>(index) : -1;
    }

    bool endsWithControl(int block) const { return controlInst(prog[blocks[block].last]); }
};

// Todo alvo de desvio inicia um bloco e toda instrucao de desvio encerra um.
Context makeContext(const std::vector<Instruction>& prog, uint32_t base) {
    Context ctx{prog, base, {}, {}};
    const int n = static_cast<int>(prog.size());
    std::vector<bool> leader(static_cast<std::size_t>(n), false);
    if (n > 0) leader[0] = true;
    for (int i = 0; i < n; ++i) {
        if (!controlInst(prog[i])) continue;
        if (i + 1 < n) leader[i + 1] = true;
        if (const auto target = prog[i].branchTarget()) {
            const int t = ctx.indexOf(*target);
            if (t >= 0) leader[t] = true;
        }
    }
    ctx.blockOf.assign(static_cast<std::size_t>(n), 0);
    for (int i = 0; i < n; ++i) {
        if (leader[i]) ctx.blocks.push_back({i, i});
        ctx.blocks.back().last = i;
        ctx.blockOf[i] = static_cast<int>(ctx.blocks.size()) - 1;
    }
    return ctx;
}

Item makeItem(const Context& ctx, int i) {
    Item item;
    item.inst = ctx.prog[i];
    item.orig = i;
    item.block = ctx.blockOf[i];
    if (controlInst(item.inst)) {
        if (const auto target = item.inst.branchTarget()) {
            const int t = ctx.indexOf(*target);
            if (t >= 0) {
                item.targetBlock = ctx.blockOf[t];
            } else if (*target == ctx.base + 4u * static_cast<uint32_t>(ctx.prog.size())) {
                item.targetBlock = kProgramEnd;  // desvio para logo depois da ultima instrucao
            } else {
                item.hasExternal = true;
                item.externalTarget = *target;
            }
        }
    }
    return item;
}

// ---------------------------------------------------------------------------
// 3. Fluxo de execucao e deteccao de conflitos de dados
// ---------------------------------------------------------------------------

// Primeira posicao de cada bloco na sequencia (os nops inseridos antes do
// primeiro consumidor pertencem ao bloco, entao o alvo do desvio os inclui).
std::vector<int> blockStarts(const std::vector<Item>& items) {
    int maxBlock = -1;
    for (const Item& it : items) maxBlock = std::max(maxBlock, it.block);
    std::vector<int> start(static_cast<std::size_t>(maxBlock + 1), -1);
    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        if (start[items[i].block] < 0) start[items[i].block] = i;
    }
    return start;
}

// Quem executa logo depois de i. O slot de atraso (posicao seguinte a um
// desvio) e sempre executado; so depois dele o fluxo se divide.
std::vector<int> successors(const std::vector<Item>& items, const std::vector<int>& start, int i) {
    std::vector<int> out;
    const int n = static_cast<int>(items.size());
    if (i > 0 && items[i - 1].isControl()) {
        const Item& branch = items[i - 1];
        if (branch.targetBlock >= 0) out.push_back(start[branch.targetBlock]);
        if (branch.inst.cls == Class::Branch && i + 1 < n) out.push_back(i + 1);
    } else if (i + 1 < n) {
        out.push_back(i + 1);
    }
    return out;
}

bool leavesSlot(const std::vector<Item>& items, int from) { return from > 0 && items[from - 1].isControl(); }

}  // namespace

bool Item::isControl() const { return controlInst(inst); }

std::vector<DataHazard> findHazards(const std::vector<Item>& items) {
    const int n = static_cast<int>(items.size());
    const std::vector<int> start = blockStarts(items);

    std::vector<std::vector<int>> preds(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        for (int s : successors(items, start, i)) preds[s].push_back(i);
    }

    std::set<std::tuple<int, int, int, int>> seen;
    std::vector<DataHazard> found;
    auto add = [&](int p, int c, uint8_t reg, int d, bool cross) {
        if (!seen.insert({p, c, reg, d}).second) return;
        found.push_back({p, c, reg, d, cross});
    };

    for (int c = 0; c < n; ++c) {
        for (uint8_t reg : items[c].inst.readsRegisters()) {
            for (int q1 : preds[c]) {
                const bool crossFirst = leavesSlot(items, q1);
                if (writes(items[q1].inst) == reg) {
                    add(q1, c, reg, 1, crossFirst);
                    continue;  // o produtor mais recente neste caminho e q1
                }
                for (int q2 : preds[q1]) {
                    if (writes(items[q2].inst) == reg) {
                        add(q2, c, reg, 2, crossFirst || leavesSlot(items, q2));
                    }
                }
            }
        }
    }
    std::sort(found.begin(), found.end(), [](const DataHazard& a, const DataHazard& b) {
        return std::tie(a.consumer, a.producer, a.reg) < std::tie(b.consumer, b.producer, b.reg);
    });
    return found;
}

Analysis analyze(const std::vector<Instruction>& program, uint32_t baseAddress) {
    const Context ctx = makeContext(program, baseAddress);
    Analysis result;
    result.blocks = ctx.blocks;
    for (int i = 0; i < static_cast<int>(program.size()); ++i) result.items.push_back(makeItem(ctx, i));
    result.hazards = findHazards(result.items);
    for (int i = 0; i < static_cast<int>(program.size()); ++i) {
        if (!controlInst(program[i])) continue;
        result.controls.push_back({i, i + 1 < static_cast<int>(program.size()) ? i + 1 : -1});
    }
    return result;
}

namespace {

// ---------------------------------------------------------------------------
// 4. R3: nops de dados
// ---------------------------------------------------------------------------

// Corrige o primeiro consumidor em conflito, recalcula, e repete. Inserir antes
// do consumidor serve a todos os caminhos que chegam nele (o alvo de um desvio
// aponta para o primeiro nop). Se o consumidor e o slot de atraso, os nops vao
// antes do desvio: entre o desvio e o slot nao ha lugar para eles.
void insertDataNops(std::vector<Item>& items) {
    for (;;) {
        const std::vector<DataHazard> hazards = findHazards(items);
        if (hazards.empty()) return;

        const int consumer = hazards.front().consumer;  // lista ordenada por consumidor
        int need = 0;
        for (const DataHazard& h : hazards) {
            if (h.consumer == consumer) need = std::max(need, h.nops());
        }
        const int at = leavesSlot(items, consumer) ? consumer - 1 : consumer;
        const int block = items[at].block;
        items.insert(items.begin() + at, static_cast<std::size_t>(need), makeNop(block, Tag::NopData));
    }
}

// ---------------------------------------------------------------------------
// 5. R5: escalonamento dentro do bloco
// ---------------------------------------------------------------------------

// Existe dependencia entre i e j (i antes de j na entrada) que impede trocar a ordem?
bool depends(const Context& ctx, int i, int j) {
    const Instruction& a = ctx.prog[i];
    const Instruction& b = ctx.prog[j];
    if (barrierInst(a) || barrierInst(b)) return true;

    const uint8_t wa = writes(a);
    const uint8_t wb = writes(b);
    if (contains(b.readsRegisters(), wa)) return true;  // leitura apos escrita
    if (contains(a.readsRegisters(), wb)) return true;  // escrita apos leitura
    if (wa != 0 && wa == wb) return true;               // escrita apos escrita

    // Memoria: so ha independencia garantida (premissa 6) com o mesmo
    // registrador-base, deslocamentos diferentes e base inalterada entre os dois.
    if (memInst(a) && memInst(b) && (a.cls == Class::Store || b.cls == Class::Store)) {
        bool distinct = a.rs1 == b.rs1 && a.imm != b.imm;
        for (int k = i; distinct && k < j; ++k) {
            if (writes(ctx.prog[k]) == *a.rs1) distinct = false;
        }
        if (!distinct) return true;
    }
    return false;
}

// Instrucoes do bloco que podem ir para o slot de atraso: nao podem ser
// pre-requisito de nenhuma outra do bloco, nem do proprio desvio (o desvio le
// rs1/rs2 antes do slot executar; um salto que escreve rd tambem conta).
std::vector<int> slotCandidates(const Context& ctx, int block) {
    std::vector<int> out;
    if (!ctx.endsWithControl(block)) return out;
    const Block& b = ctx.blocks[block];
    for (int x = b.first; x < b.last; ++x) {
        if (barrierInst(ctx.prog[x])) continue;
        bool free = true;
        for (int y = x + 1; y <= b.last && free; ++y) free = !depends(ctx, x, y);
        if (free) out.push_back(x);
    }
    return out;
}

// Escalonador: ordena os nos de um bloco minimizando os nops de dados, dado o
// que as duas ultimas instrucoes ja emitidas escrevem. Busca exata com
// memoizacao em (conjunto emitido, escrita da ultima, escrita da penultima);
// acima do limite usa um guloso.
class Scheduler {
public:
    Scheduler(const Context& ctx, const std::vector<int>& nodes, int control)
        : m_(static_cast<int>(nodes.size())) {
        for (int a = 0; a < m_; ++a) {
            const Instruction& inst = ctx.prog[nodes[a]];
            writes_.push_back(writes(inst));
            reads_.push_back(inst.readsRegisters());
            uint32_t mask = 0;
            for (int b = 0; b < a; ++b) {
                if (nodes[a] == control || depends(ctx, nodes[b], nodes[a])) mask |= 1u << b;
            }
            deps_.push_back(mask);
        }
    }

    std::vector<int> order(uint8_t h1, uint8_t h2) {
        if (m_ <= kExactLimit) {
            std::vector<int> result;
            overflow_ = false;
            memo_.clear();
            pick_.clear();
            solve(0, h1, h2);
            if (!overflow_) {
                uint32_t mask = 0;
                while (mask != full()) {
                    const int k = pick_.at(key(mask, h1, h2));
                    result.push_back(k);
                    advance(k, h1, h2);
                    mask |= 1u << k;
                }
                return result;
            }
        }
        return greedy(h1, h2);
    }

private:
    static constexpr int kExactLimit = 14;
    static constexpr std::size_t kStateCap = 2000000;

    int m_;
    std::vector<uint8_t> writes_;
    std::vector<std::vector<uint8_t>> reads_;
    std::vector<uint32_t> deps_;
    std::unordered_map<uint64_t, int> memo_;
    std::unordered_map<uint64_t, int> pick_;
    bool overflow_ = false;

    uint32_t full() const { return m_ >= 32 ? 0xFFFFFFFFu : ((1u << m_) - 1u); }

    static uint64_t key(uint32_t mask, uint8_t h1, uint8_t h2) {
        return static_cast<uint64_t>(mask) | (static_cast<uint64_t>(h1) << 32) |
               (static_cast<uint64_t>(h2) << 40);
    }

    // Nops necessarios antes de k: 2 se le o que a ultima escreve, 1 se le o
    // que a penultima escreve (a mais recente de cada registrador manda).
    int stall(int k, uint8_t h1, uint8_t h2) const {
        int need = 0;
        for (uint8_t r : reads_[k]) {
            if (r == h1) need = std::max(need, 2);
            else if (r == h2) need = std::max(need, 1);
        }
        return need;
    }

    // Historico depois de emitir k (com os nops, se houve): os nops nao escrevem.
    void advance(int k, uint8_t& h1, uint8_t& h2) const {
        const int s = stall(k, h1, h2);
        h2 = s >= 1 ? 0 : h1;
        h1 = writes_[k];
    }

    int solve(uint32_t mask, uint8_t h1, uint8_t h2) {
        if (mask == full()) return 0;
        const uint64_t id = key(mask, h1, h2);
        if (const auto it = memo_.find(id); it != memo_.end()) return it->second;
        if (memo_.size() > kStateCap) {
            overflow_ = true;
            return 0;
        }

        int best = std::numeric_limits<int>::max();
        int bestK = -1;
        for (int k = 0; k < m_; ++k) {
            if ((mask >> k & 1u) || (deps_[k] & ~mask) != 0) continue;
            uint8_t n1 = h1, n2 = h2;
            const int s = stall(k, h1, h2);
            advance(k, n1, n2);
            const int cost = s + solve(mask | (1u << k), n1, n2);
            if (cost < best) {  // empate: fica o de menor indice (ordem original)
                best = cost;
                bestK = k;
            }
        }
        memo_[id] = best;
        pick_[id] = bestK;
        return best;
    }

    std::vector<int> greedy(uint8_t h1, uint8_t h2) const {
        std::vector<int> result;
        uint32_t mask = 0;
        while (mask != full()) {
            int best = -1;
            int bestStall = std::numeric_limits<int>::max();
            for (int k = 0; k < m_; ++k) {
                if ((mask >> k & 1u) || (deps_[k] & ~mask) != 0) continue;
                const int s = stall(k, h1, h2);
                if (s < bestStall) {
                    bestStall = s;
                    best = k;
                }
            }
            result.push_back(best);
            advance(best, h1, h2);
            mask |= 1u << best;
        }
        return result;
    }
};

// Monta a sequencia final antes de inserir os nops de dados. slotChoice[b] e a
// instrucao movida para o slot do bloco b (-1 = slot preenchido com nop).
std::vector<Item> buildItems(const Context& ctx, bool reorder, const std::vector<int>& slotChoice) {
    std::vector<Item> out;
    auto history = [&](int back) -> uint8_t {
        const int idx = static_cast<int>(out.size()) - back;
        return idx >= 0 ? writes(out[idx].inst) : 0;
    };

    for (int b = 0; b < static_cast<int>(ctx.blocks.size()); ++b) {
        const Block& blk = ctx.blocks[b];
        const bool hasControl = ctx.endsWithControl(b);
        const int control = hasControl ? blk.last : -1;
        const int moved = reorder ? slotChoice[b] : -1;

        std::vector<int> nodes;  // em ordem original, sem a movida para o slot
        for (int i = blk.first; i <= blk.last; ++i) {
            if (i != moved) nodes.push_back(i);
        }

        std::vector<int> order(nodes.size());
        for (std::size_t k = 0; k < order.size(); ++k) order[k] = static_cast<int>(k);
        if (reorder) {
            Scheduler scheduler(ctx, nodes, control);
            order = scheduler.order(history(1), history(2));
        }

        for (std::size_t pos = 0; pos < order.size(); ++pos) {
            Item item = makeItem(ctx, nodes[order[pos]]);
            if (order[pos] != static_cast<int>(pos)) item.tag = Tag::Reordered;
            out.push_back(item);
        }
        if (hasControl) {
            if (moved >= 0) {
                Item item = makeItem(ctx, moved);
                item.block = b;  // o slot pertence ao bloco do desvio
                item.tag = Tag::MovedSlot;
                out.push_back(item);
            } else {
                out.push_back(makeNop(b, Tag::NopSlot));
            }
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// 6. R6: recalculo dos deslocamentos
// ---------------------------------------------------------------------------

void finalize(std::vector<Item>& items, uint32_t base) {
    const std::vector<int> start = blockStarts(items);
    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        Item& item = items[i];
        const uint32_t pc = base + 4u * static_cast<uint32_t>(i);
        uint32_t raw = item.inst.raw;
        if (item.isControl() && (item.targetBlock != -1 || item.hasExternal)) {
            uint32_t target = item.externalTarget;
            if (item.targetBlock >= 0) target = base + 4u * static_cast<uint32_t>(start[item.targetBlock]);
            if (item.targetBlock == kProgramEnd) target = base + 4u * static_cast<uint32_t>(items.size());
            raw = withBranchOffset(raw, item.inst.format, static_cast<int32_t>(target - pc));
        }
        item.inst = decode(raw, pc);
    }
}

}  // namespace

uint32_t withBranchOffset(uint32_t raw, Format format, int32_t offset) {
    const uint32_t u = static_cast<uint32_t>(offset);
    if (format == Format::B) {
        if (offset < -4096 || offset > 4094 || (offset & 1)) {
            throw std::runtime_error("deslocamento de desvio fora do alcance de 13 bits");
        }
        const uint32_t imm = ((u >> 12 & 1u) << 31) | ((u >> 5 & 0x3Fu) << 25) |
                             ((u >> 1 & 0xFu) << 8) | ((u >> 11 & 1u) << 7);
        return (raw & 0x01FFF07Fu) | imm;
    }
    if (format == Format::J) {
        if (offset < -(1 << 20) || offset > (1 << 20) - 2 || (offset & 1)) {
            throw std::runtime_error("deslocamento de salto fora do alcance de 21 bits");
        }
        const uint32_t imm = ((u >> 20 & 1u) << 31) | ((u >> 1 & 0x3FFu) << 21) |
                             ((u >> 11 & 1u) << 20) | ((u >> 12 & 0xFFu) << 12);
        return (raw & 0xFFFu) | imm;
    }
    return raw;
}

Corrected correct(const std::vector<Instruction>& program, uint32_t baseAddress, bool reorder) {
    const Context ctx = makeContext(program, baseAddress);
    std::vector<int> choice(ctx.blocks.size(), -1);

    auto make = [&]() {
        std::vector<Item> items = buildItems(ctx, reorder, choice);
        insertDataNops(items);
        return items;
    };

    // Qual instrucao vai para cada slot? Depende do resto do programa (preencher
    // um slot muda quem fica perto do inicio dos caminhos), entao testa-se cada
    // candidata contra o programa inteiro, bloco a bloco, ate estabilizar.
    if (reorder) {
        for (int sweep = 0; sweep < 3; ++sweep) {
            bool changed = false;
            for (int b = 0; b < static_cast<int>(ctx.blocks.size()); ++b) {
                const std::vector<int> candidates = slotCandidates(ctx, b);
                if (candidates.empty()) continue;

                const int before = choice[b];
                std::size_t best = std::numeric_limits<std::size_t>::max();
                int bestPick = -1;
                for (int cand : candidates) {  // empate entre candidatas: a primeira
                    choice[b] = cand;
                    const std::size_t size = make().size();
                    if (size < best) {
                        best = size;
                        bestPick = cand;
                    }
                }
                choice[b] = -1;  // slot com nop so vence se for estritamente melhor
                if (make().size() < best) bestPick = -1;
                choice[b] = bestPick;
                changed = changed || bestPick != before;
            }
            if (!changed) break;
        }
    }

    Corrected result;
    result.items = make();
    finalize(result.items, baseAddress);
    for (const Item& item : result.items) {
        result.words.push_back(item.inst.raw);
        if (item.tag == Tag::NopData) ++result.nopsData;
        if (item.tag == Tag::NopSlot) ++result.nopsSlot;
        if (item.tag == Tag::MovedSlot) ++result.movedToSlot;
        if (item.tag == Tag::Reordered) ++result.reordered;
    }
    result.residualHazards = findHazards(result.items).size();
    return result;
}

}  // namespace riscv
