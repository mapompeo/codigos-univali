// Testes da M2. Sem framework externo: cada verificacao imprime [ ok ]/[FAIL]
// e o programa retorna != 0 se alguma falhar.
//
// A prova principal e comportamental: um emulador RV32I executa o programa
// original numa maquina "ideal" (sem conflitos, sem slot de atraso) e os
// programas corrigidos no pipeline do enunciado (leitura de registrador
// defasada, slot de atraso). Se o estado final (registradores e memoria) for o
// mesmo, a correcao preservou o programa.
//
// Autores: Jose Gabriel Santos Gomes, Matheus Pompeo Dias e Nathan Gustavo Reichert

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "hazards.hpp"
#include "riscv.hpp"

namespace {

using riscv::Class;
using riscv::Instruction;

int failures = 0;
int checks = 0;

void expect(const std::string& name, bool ok, const std::string& detail = "") {
    ++checks;
    if (ok) return;
    ++failures;
    std::cout << "[FAIL] " << name << (detail.empty() ? "" : ": " + detail) << '\n';
}

// ---------------------------------------------------------------------------
// Emulador
// ---------------------------------------------------------------------------

struct State {
    std::array<uint32_t, 32> regs{};
    std::map<uint32_t, uint8_t> mem;
    std::string out;  // texto impresso pelas chamadas de sistema (ecall)
    bool operator==(const State& o) const { return regs == o.regs && mem == o.mem && out == o.out; }
};

constexpr uint32_t kDataBase = 0x10010000;  // mapa de memoria padrao do RARS

class Machine {
public:
    // pipeline = true: slot de atraso e leitura defasada (escrita em WB so e
    // vista por quem le 3 instrucoes depois). false: maquina ideal.
    Machine(const std::vector<uint32_t>& words, uint32_t base, bool pipeline,
            const std::array<uint32_t, 32>& init, const std::vector<uint32_t>& data = {},
            const std::vector<int32_t>& inputs = {}, bool strictEcall = false)
        : base_(base), visibleAfter_(pipeline ? 3 : 1), pipeline_(pipeline), strictEcall_(strictEcall),
          inputs_(inputs) {
        for (std::size_t i = 0; i < data.size(); ++i) store(kDataBase + 4u * static_cast<uint32_t>(i), data[i], 4);
        for (std::size_t i = 0; i < words.size(); ++i) {
            prog_.push_back(riscv::decode(words[i], base + 4u * static_cast<uint32_t>(i)));
        }
        for (int r = 0; r < 32; ++r) hist_[r].push_back({-100, init[r]});
        hist_[0].back().second = 0;
    }

    State run(long maxSteps = 200000) {
        uint32_t pc = base_;
        long m = 0;
        while (inRange(pc) && m < maxSteps && !halted_) {
            const Instruction& inst = at(pc);
            const Exec ex = step(inst, pc, m++);
            if (pipeline_ && isControl(inst)) {
                if (inRange(pc + 4)) step(at(pc + 4), pc + 4, m++);
                pc = ex.taken ? ex.target : pc + 8;
            } else {
                pc = ex.taken ? ex.target : pc + 4;
            }
        }
        State s;
        for (int r = 0; r < 32; ++r) s.regs[r] = hist_[r].back().second;
        s.mem = mem_;
        s.out = out_;
        return s;
    }

private:
    struct Exec {
        bool taken = false;
        uint32_t target = 0;
    };

    uint32_t base_;
    long visibleAfter_;
    bool pipeline_;
    bool strictEcall_;  // ecall le a7/a0 e escreve a0 como instrucao comum (com defasagem)
    std::vector<Instruction> prog_;
    std::array<std::vector<std::pair<long, uint32_t>>, 32> hist_;
    std::map<uint32_t, uint8_t> mem_;
    std::vector<int32_t> inputs_;
    std::size_t inputPos_ = 0;
    std::string out_;
    bool halted_ = false;

    static bool isControl(const Instruction& i) { return i.cls == Class::Branch || i.cls == Class::Jump; }
    bool inRange(uint32_t pc) const { return pc >= base_ && (pc - base_) / 4 < prog_.size() && pc % 4 == 0; }
    const Instruction& at(uint32_t pc) const { return prog_[(pc - base_) / 4]; }

    uint32_t read(int r, long m) const {
        if (r == 0) return 0;
        for (auto it = hist_[r].rbegin(); it != hist_[r].rend(); ++it) {
            if (it->first <= m - visibleAfter_) return it->second;
        }
        return hist_[r].front().second;
    }
    // O ecall e atendido pelo ambiente (o RARS), em ordem de programa: le e
    // escreve registradores sem passar pelo pipeline, como no decodificador da M1.
    uint32_t readNow(int r) const { return r == 0 ? 0 : hist_[r].back().second; }
    void write(int r, uint32_t v, long m) {
        if (r != 0) hist_[r].push_back({m, v});
    }
    uint32_t load(uint32_t addr, int bytes) const {
        uint32_t v = 0;
        for (int i = 0; i < bytes; ++i) {
            const auto it = mem_.find(addr + static_cast<uint32_t>(i));
            v |= static_cast<uint32_t>(it == mem_.end() ? 0 : it->second) << (8 * i);
        }
        return v;
    }
    void store(uint32_t addr, uint32_t v, int bytes) {
        for (int i = 0; i < bytes; ++i) mem_[addr + static_cast<uint32_t>(i)] = static_cast<uint8_t>(v >> (8 * i));
    }

    Exec step(const Instruction& in, uint32_t pc, long m) {
        const std::string& op = in.mnemonic;
        Exec ex;
        const uint32_t a = in.rs1 ? read(*in.rs1, m) : 0;
        const uint32_t b = in.rs2 ? read(*in.rs2, m) : 0;
        const int32_t imm = in.imm.value_or(0);
        const uint32_t ui = static_cast<uint32_t>(imm);
        const int rd = in.rd.value_or(0);
        auto w = [&](uint32_t v) { write(rd, v, m); };

        if (op == "add") w(a + b);
        else if (op == "sub") w(a - b);
        else if (op == "sll") w(a << (b & 31));
        else if (op == "slt") w(static_cast<int32_t>(a) < static_cast<int32_t>(b));
        else if (op == "sltu") w(a < b);
        else if (op == "xor") w(a ^ b);
        else if (op == "srl") w(a >> (b & 31));
        else if (op == "sra") w(static_cast<uint32_t>(static_cast<int32_t>(a) >> (b & 31)));
        else if (op == "or") w(a | b);
        else if (op == "and") w(a & b);
        else if (op == "mul") w(a * b);
        else if (op == "div") w(b == 0 ? 0xFFFFFFFFu : static_cast<uint32_t>(static_cast<int32_t>(a) / static_cast<int32_t>(b)));
        else if (op == "rem") w(b == 0 ? a : static_cast<uint32_t>(static_cast<int32_t>(a) % static_cast<int32_t>(b)));
        else if (op == "ecall") {
            const uint32_t service = strictEcall_ ? read(17, m) : readNow(17);
            const uint32_t arg = strictEcall_ ? read(10, m) : readNow(10);
            if (service == 1) out_ += std::to_string(static_cast<int32_t>(arg));
            else if (service == 4) {
                for (uint32_t addr = arg; load(addr, 1) != 0; ++addr) out_ += static_cast<char>(load(addr, 1));
            } else if (service == 11) out_ += static_cast<char>(arg);
            else if (service == 5) {
                if (inputPos_ >= inputs_.size()) halted_ = true;  // sem mais entrada: encerra
                else if (strictEcall_) write(10, static_cast<uint32_t>(inputs_[inputPos_++]), m);
                else hist_[10].push_back({-50, static_cast<uint32_t>(inputs_[inputPos_++])});
            } else if (service == 10 || service == 93) halted_ = true;
        }
        else if (op == "addi") w(a + ui);
        else if (op == "slti") w(static_cast<int32_t>(a) < imm);
        else if (op == "sltiu") w(a < ui);
        else if (op == "xori") w(a ^ ui);
        else if (op == "ori") w(a | ui);
        else if (op == "andi") w(a & ui);
        else if (op == "slli") w(a << (ui & 31));
        else if (op == "srli") w(a >> (ui & 31));
        else if (op == "srai") w(static_cast<uint32_t>(static_cast<int32_t>(a) >> (ui & 31)));
        else if (op == "lui") w(ui);
        else if (op == "lw") w(load(a + ui, 4));
        else if (op == "lh") w(static_cast<uint32_t>(riscv::signExtend(load(a + ui, 2), 16)));
        else if (op == "lb") w(static_cast<uint32_t>(riscv::signExtend(load(a + ui, 1), 8)));
        else if (op == "lhu") w(load(a + ui, 2));
        else if (op == "lbu") w(load(a + ui, 1));
        else if (op == "sw") store(a + ui, b, 4);
        else if (op == "sh") store(a + ui, b, 2);
        else if (op == "sb") store(a + ui, b, 1);
        else if (in.cls == Class::Branch) {
            const int32_t sa = static_cast<int32_t>(a), sb = static_cast<int32_t>(b);
            ex.taken = (op == "beq" && a == b) || (op == "bne" && a != b) || (op == "blt" && sa < sb) ||
                       (op == "bge" && sa >= sb) || (op == "bltu" && a < b) || (op == "bgeu" && a >= b);
            ex.target = pc + ui;
        } else if (op == "jal") {
            w(pc + 4);
            ex.taken = true;
            ex.target = pc + ui;
        } else if (op == "jalr") {
            w(pc + 4);
            ex.taken = true;
            ex.target = (a + ui) & ~1u;
        }
        return ex;
    }
};

std::array<uint32_t, 32> initialRegisters(uint32_t seed) {
    std::mt19937 g(seed);
    std::array<uint32_t, 32> r{};
    for (int i = 1; i < 32; ++i) r[i] = g() % 1000;
    r[10] = 0x1000;  // duas bases que apontam para a mesma regiao (teste de aliasing)
    r[11] = 0x1000;
    r[31] = 0;
    return r;
}

std::vector<Instruction> decodeAll(const std::vector<uint32_t>& words, uint32_t base) {
    std::vector<Instruction> out;
    for (std::size_t i = 0; i < words.size(); ++i) out.push_back(riscv::decode(words[i], base + 4u * static_cast<uint32_t>(i)));
    return out;
}

// ---------------------------------------------------------------------------
// Verificacao de um programa: correcao + equivalencia
// ---------------------------------------------------------------------------

struct Outcome {
    std::size_t original = 0, nops = 0, reorder = 0;
    bool brokenWithoutFix = false;  // o original, no pipeline cru, muda de resultado?
};

Outcome verify(const std::string& name, const std::vector<uint32_t>& words, uint32_t base, bool strict = true) {
    const std::vector<Instruction> program = decodeAll(words, base);
    Outcome out;
    out.original = words.size();

    const riscv::Corrected nops = riscv::correct(program, base, false);
    const riscv::Corrected reorder = riscv::correct(program, base, true);
    out.nops = nops.items.size();
    out.reorder = reorder.items.size();

    expect(name + ": sem conflitos (nops)", nops.residualHazards == 0);
    expect(name + ": sem conflitos (reordenacao)", reorder.residualHazards == 0);
    expect(name + ": nao decodificavel? (nops)", [&] {
        for (const auto& it : nops.items) if (!it.inst.valid()) return false;
        return true;
    }());
    expect(name + ": reordenacao nunca pior que so nops", out.reorder <= out.nops,
           std::to_string(out.reorder) + " > " + std::to_string(out.nops));

    for (uint32_t seed = 1; seed <= 3; ++seed) {
        const auto init = initialRegisters(seed);
        const State ideal = Machine(words, base, false, init).run();
        const State a = Machine(nops.words, base, true, init).run();
        const State b = Machine(reorder.words, base, true, init).run();
        expect(name + ": resultado igual (so nops), semente " + std::to_string(seed), a == ideal);
        expect(name + ": resultado igual (reordenacao), semente " + std::to_string(seed), b == ideal);
        const State raw = Machine(words, base, true, init).run();
        if (!(raw == ideal)) out.brokenWithoutFix = true;
    }
    (void)strict;
    return out;
}

// ---------------------------------------------------------------------------
// Casos escritos a mao (tests/data/*.hex) com resposta conhecida
// ---------------------------------------------------------------------------

std::vector<uint32_t> loadHex(const std::string& file) {
    std::ifstream in(std::string(TEST_DATA_DIR) + "/" + file);
    std::vector<uint32_t> words;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        words.push_back(static_cast<uint32_t>(std::stoul(line, nullptr, 16)));
    }
    return words;
}

void testEncoding() {
    for (int32_t off : {-4096, -2048, -4, 4, 8, 2046, 4094}) {
        const uint32_t raw = riscv::withBranchOffset(0x00B50063 /* beq a0,a1,0 */, riscv::Format::B, off);
        expect("beq deslocamento " + std::to_string(off), riscv::decode(raw, 0).imm.value_or(0) == off);
        expect("beq preserva registradores", riscv::decode(raw, 0).rs1.value_or(99) == 10 &&
                                                 riscv::decode(raw, 0).rs2.value_or(99) == 11);
    }
    for (int32_t off : {-1048576, -2048, -4, 4, 2048, 1048574}) {
        const uint32_t raw = riscv::withBranchOffset(0x0000006F | (1u << 7) /* jal ra,0 */, riscv::Format::J, off);
        expect("jal deslocamento " + std::to_string(off), riscv::decode(raw, 0).imm.value_or(0) == off);
        expect("jal preserva rd", riscv::decode(raw, 0).rd.value_or(99) == 1);
    }
    bool threw = false;
    try { riscv::withBranchOffset(0x00B50063, riscv::Format::B, 5000); } catch (const std::exception&) { threw = true; }
    expect("deslocamento fora do alcance lanca excecao", threw);
}

void testKnownCases() {
    const uint32_t base = 0x00400000;
    {
        const auto w = loadHex("cadeia.hex");
        const Outcome o = verify("cadeia", w, base);
        expect("cadeia: 4 consumidores x 2 nops = 8 (com ou sem reordenacao)",
               o.nops == o.original + 8 && o.reorder == o.original + 8,
               std::to_string(o.nops) + "/" + std::to_string(o.reorder));
    }
    {
        const Outcome o = verify("sem_nops", loadHex("sem_nops.hex"), base);
        expect("sem_nops: so nops acrescenta 6", o.nops == o.original + 6);
        expect("sem_nops: reordenacao elimina todos os nops", o.reorder == o.original);
    }
    {
        const Outcome o = verify("laco", loadHex("laco.hex"), base);
        std::cout << "  laco: " << o.original << " -> " << o.nops << " (nops) / " << o.reorder << " (reordenado)\n";
    }
    {
        const Outcome o = verify("frente", loadHex("frente.hex"), base);
        std::cout << "  frente: " << o.original << " -> " << o.nops << " (nops) / " << o.reorder << " (reordenado)\n";
    }
    {
        const Outcome o = verify("enunciado", loadHex("enunciado.hex"), base);
        std::cout << "  enunciado: " << o.original << " -> " << o.nops << " (nops) / " << o.reorder << " (reordenado)\n";
    }
    {   // base 0: o endereco-base nao pode alterar nada
        verify("laco base 0", loadHex("laco.hex"), 0);
    }
}

// ---------------------------------------------------------------------------
// Fuzz: programas aleatorios com desvios para frente e lacos
// ---------------------------------------------------------------------------

uint32_t encR(uint32_t f7, int rs2, int rs1, uint32_t f3, int rd) { return f7 << 25 | rs2 << 20 | rs1 << 15 | f3 << 12 | rd << 7 | 0x33; }
uint32_t encI(int32_t imm, int rs1, uint32_t f3, int rd, uint32_t op = 0x13) { return (static_cast<uint32_t>(imm) & 0xFFF) << 20 | rs1 << 15 | f3 << 12 | rd << 7 | op; }
uint32_t encS(int32_t imm, int rs2, int rs1) { const uint32_t u = static_cast<uint32_t>(imm) & 0xFFF; return (u >> 5) << 25 | rs2 << 20 | rs1 << 15 | 2u << 12 | (u & 0x1F) << 7 | 0x23; }
uint32_t encB(uint32_t f3, int rs1, int rs2, int32_t off) { return riscv::withBranchOffset(f3 << 12 | rs2 << 20 | rs1 << 15 | 0x63, riscv::Format::B, off); }
uint32_t encJ(int32_t off) { return riscv::withBranchOffset(0x6F, riscv::Format::J, off); }

struct Gen {
    std::mt19937 g;
    explicit Gen(uint32_t seed) : g(seed) {}
    int pick(int n) { return static_cast<int>(g() % static_cast<uint32_t>(n)); }
    int reg() {  // poucos registradores: muitos conflitos
        static const int pool[] = {0, 5, 6, 7, 28, 29};
        return pool[pick(6)];
    }
    int dst() { return reg() == 0 && pick(4) ? 5 + pick(3) : reg(); }
    uint32_t plain() {
        const int k = pick(10);
        const int base = pick(2) ? 10 : 11;
        const int off = 4 * pick(8);
        switch (k) {
            case 0: return encR(0, reg(), reg(), 0, dst());
            case 1: return encR(0x20, reg(), reg(), 0, dst());
            case 2: return encR(0, reg(), reg(), 7, dst());
            case 3: return encR(0, reg(), reg(), 6, dst());
            case 4: return encR(0, reg(), reg(), 4, dst());
            case 5: return encI(pick(40) - 20, reg(), 0, dst());
            case 6: return encI(pick(5), reg(), 1, dst());
            case 7: return encI(off, base, 2, dst(), 0x03);   // lw
            case 8: return encS(off, reg(), base);            // sw
            default: return encR(1, reg(), reg(), 0, dst());  // mul
        }
    }
    std::vector<uint32_t> program() {
        std::vector<uint32_t> w;
        const int segments = 1 + pick(3);
        for (int s = 0; s < segments; ++s) {
            if (pick(3) == 0) {  // laco: contador em x31, 2 a 4 voltas
                w.push_back(encI(2 + pick(3), 0, 0, 31));
                const int body = 2 + pick(5);
                const int first = static_cast<int>(w.size());
                for (int i = 0; i < body; ++i) w.push_back(plain());
                w.push_back(encI(-1, 31, 0, 31));
                w.push_back(encB(1, 31, 0, (first - static_cast<int>(w.size())) * 4));
            } else {  // trecho reto com desvios para frente (ate o fim do trecho)
                const int len = 3 + pick(10);
                const int start = static_cast<int>(w.size());
                for (int i = 0; i < len; ++i) {
                    const int here = start + i;
                    const int room = len - i - 1;  // posicoes ate o fim do trecho
                    if (room >= 1 && pick(5) == 0) {
                        const int target = here + 2 + pick(room);  // pula pelo menos 1 instrucao
                        const int off = (target - here) * 4;
                        if (pick(3) == 0) w.push_back(encJ(off));
                        else w.push_back(encB(pick(2) ? 0 : 1, reg(), reg(), off));
                    } else {
                        w.push_back(plain());
                    }
                }
            }
        }
        return w;
    }
};

// Verificador independente da otimalidade para trechos sem desvios: enumera
// todas as permutacoes que respeitam dependencias (reescritas aqui, sem usar o
// codigo da biblioteca) e calcula os nops pelo modelo do enunciado.
bool depends(const std::vector<Instruction>& p, int i, int j) {
    const Instruction& a = p[i];
    const Instruction& b = p[j];
    const int wa = a.writesRegister() ? *a.writesRegister() : 0;
    const int wb = b.writesRegister() ? *b.writesRegister() : 0;
    auto reads = [](const Instruction& x, int r) { for (uint8_t q : x.readsRegisters()) if (q == r) return true; return false; };
    if (wa && reads(b, wa)) return true;
    if (wb && reads(a, wb)) return true;
    if (wa && wa == wb) return true;
    const bool ma = a.cls == Class::Load || a.cls == Class::Store, mb = b.cls == Class::Load || b.cls == Class::Store;
    if (ma && mb && (a.cls == Class::Store || b.cls == Class::Store)) {
        bool distinct = a.rs1 == b.rs1 && a.imm != b.imm;
        for (int k = i; k < j; ++k) if (p[k].writesRegister() && *p[k].writesRegister() == *a.rs1) distinct = false;
        if (!distinct) return true;
    }
    return false;
}

std::size_t bruteForceMinSize(const std::vector<Instruction>& p) {
    const int n = static_cast<int>(p.size());
    std::vector<int> perm(static_cast<std::size_t>(n));
    std::iota(perm.begin(), perm.end(), 0);
    std::size_t best = SIZE_MAX;
    do {
        bool ok = true;
        for (int x = 0; x < n && ok; ++x)
            for (int y = x + 1; y < n && ok; ++y)
                if (perm[x] > perm[y] && depends(p, perm[y], perm[x])) ok = false;  // ordem relativa invertida
        if (!ok) continue;
        // nops pelo modelo: simula a sequencia com os nops colocados antes de cada consumidor
        std::array<long, 32> pos{};
        pos.fill(-100);
        long out = 0;
        for (int x = 0; x < n; ++x) {
            const Instruction& in = p[perm[x]];
            long need = 0;
            for (uint8_t r : in.readsRegisters()) need = std::max(need, pos[r] + 3 - out);  // distancia minima 3
            out += std::max(0L, need);
            if (in.writesRegister()) pos[*in.writesRegister()] = out;
            ++out;
        }
        best = std::min(best, static_cast<std::size_t>(out));
    } while (std::next_permutation(perm.begin(), perm.end()));
    return best;
}

void testFuzz() {
    const uint32_t base = 0x00400000;
    int programs = 0, withBranches = 0, brokenRaw = 0, strictlyBetter = 0, equalSize = 0, worse = 0;
    int optimalChecked = 0;
    std::size_t sumNops = 0, sumReorder = 0, sumOriginal = 0;

    for (uint32_t seed = 1; seed <= 1500; ++seed) {
        Gen gen(seed);
        const std::vector<uint32_t> w = gen.program();
        const Outcome o = verify("fuzz " + std::to_string(seed), w, base);
        ++programs;
        brokenRaw += o.brokenWithoutFix ? 1 : 0;
        sumOriginal += o.original;
        sumNops += o.nops;
        sumReorder += o.reorder;
        if (o.reorder < o.nops) ++strictlyBetter;
        else if (o.reorder == o.nops) ++equalSize;
        else ++worse;
        for (uint32_t x : w) if ((x & 0x7F) == 0x63 || (x & 0x7F) == 0x6F) { ++withBranches; break; }
    }

    // Otimalidade em trechos retos curtos: so ha 1 bloco sem desvio, entao o
    // minimo de instrucoes e exatamente o da busca exaustiva.
    for (uint32_t seed = 1; seed <= 400; ++seed) {
        Gen gen(10000 + seed);
        std::vector<uint32_t> w;
        const int len = 3 + gen.pick(5);
        for (int i = 0; i < len; ++i) w.push_back(gen.plain());
        const auto program = decodeAll(w, base);
        const std::size_t best = bruteForceMinSize(program);
        const std::size_t ours = riscv::correct(program, base, true).items.size();
        ++optimalChecked;
        expect("otimo em trecho reto, semente " + std::to_string(seed), ours == best,
               "obtido " + std::to_string(ours) + ", minimo " + std::to_string(best));
    }

    std::cout << "  fuzz: " << programs << " programas (" << withBranches << " com desvios), "
              << brokenRaw << " mudam de resultado sem correcao\n"
              << "  fuzz: instrucoes " << sumOriginal << " -> " << sumNops << " (so nops) / " << sumReorder
              << " (reordenado); reordenado menor em " << strictlyBetter << ", igual em " << equalSize
              << ", pior em " << worse << "\n"
              << "  otimalidade conferida por forca bruta em " << optimalChecked << " trechos retos\n";
    expect("fuzz: reordenacao nunca pior que so nops", worse == 0);
    expect("fuzz: o teste tem poder (programas quebram sem correcao)", brokenRaw > programs / 4);
}


// ---------------------------------------------------------------------------
// Programas reais de Arquitetura I (tests/data/arq1), com entrada simulada
// ---------------------------------------------------------------------------

struct RealProgram {
    std::string name;
    std::vector<std::vector<int32_t>> inputs;  // varios roteiros de entrada
};

// Mesmo comportamento: texto impresso, registradores (menos ra, que guarda
// enderecos de retorno e muda de valor quando entram nops) e a area de dados.
bool sameBehavior(const State& a, const State& b) {
    if (a.out != b.out) return false;
    for (int r = 0; r < 32; ++r) {
        if (r != 1 && a.regs[r] != b.regs[r]) return false;
    }
    for (const auto& [addr, byte] : a.mem) {
        if (addr >= kDataBase && addr < kDataBase + 0x10000) {
            const auto it = b.mem.find(addr);
            if (it == b.mem.end() || it->second != byte) return false;
        }
    }
    for (const auto& [addr, byte] : b.mem) {
        if (addr >= kDataBase && addr < kDataBase + 0x10000 && !a.mem.count(addr)) return false;
    }
    return true;
}

void testRealPrograms(bool ecallRegisters) {
    const uint32_t base = 0x00400000;
    riscv::Options model;
    model.ecallRegisters = ecallRegisters;
    const std::vector<RealProgram> programs = {
        {"m1_programa_01", {{0, 3, 1, 0, 3, 0, 5, 31, 1}, {2, 4, 1, 2, 4, 1}}},
        {"m1_programa_02", {{6, 7}, {13, 0}, {100, 100}}},
        {"m2_programa_01", {{17, 5}, {4, 9}, {10, 3}}},
        {"m2_programa_02", {{13}, {15}, {2}, {1}, {97}}},
        {"m2_programa_03", {{4821}, {7}, {99999}}},
        {"fibonacci", {{10}, {1}, {20}}},
        {"mdc", {{48, 18}, {7, 13}}},
        {"soma", {{3, 4}}},
        {"vetor", {{}}},
    };
    std::cout << "  programa          original  so nops  reordenado   conflitos no original\n";
    for (const RealProgram& p : programs) {
        const auto words = loadHex("arq1/" + p.name + ".hex");
        const auto data = loadHex("arq1/" + p.name + ".data");
        const auto decoded = decodeAll(words, base);
        const riscv::Analysis analysis = riscv::analyze(decoded, base, model);
        const riscv::Corrected nops = riscv::correct(decoded, base, false, model);
        const riscv::Corrected reorder = riscv::correct(decoded, base, true, model);
        expect(p.name + ": sem conflitos (nops)", nops.residualHazards == 0);
        expect(p.name + ": sem conflitos (reordenacao)", reorder.residualHazards == 0);
        expect(p.name + ": reordenacao nunca pior", reorder.items.size() <= nops.items.size());

        int run = 0;
        for (const auto& input : p.inputs) {
            auto init = initialRegisters(1);
            for (int r = 1; r < 32; ++r) init[r] = 0;
            init[2] = 0x7fffeffc;  // sp, como no RARS
            const State ideal = Machine(words, base, false, init, data, input).run();
            const State a = Machine(nops.words, base, true, init, data, input, ecallRegisters).run();
            const State b = Machine(reorder.words, base, true, init, data, input, ecallRegisters).run();
            ++run;
            expect(p.name + " #" + std::to_string(run) + ": programa terminou", !ideal.out.empty() || p.name == "vetor");
            expect(p.name + " #" + std::to_string(run) + ": mesmo comportamento (so nops)", sameBehavior(ideal, a),
                   "saida ideal <" + ideal.out + "> obtida <" + a.out + ">");
            expect(p.name + " #" + std::to_string(run) + ": mesmo comportamento (reordenacao)", sameBehavior(ideal, b),
                   "saida ideal <" + ideal.out + "> obtida <" + b.out + ">");
        }
        std::cout << "  " << std::left << std::setw(18) << p.name << std::right << std::setw(8) << words.size()
                  << std::setw(9) << nops.items.size() << std::setw(12) << reorder.items.size()
                  << std::setw(14) << analysis.hazards.size() << '\n';
    }
}

// ---------------------------------------------------------------------------
// Lista de exercicios do professor (Arq2_Lista_Conflitos_Pipeline.pdf)
// ---------------------------------------------------------------------------

struct ListaCase {
    std::string name;
    std::size_t original, nops, reorder;  // numero de instrucoes, pela analise feita a mao
    std::string nota;
};

void testLista() {
    const uint32_t base = 0x00400000;
    const std::vector<ListaCase> cases = {
        {"a1", 5, 10, 10, "5 nops: sub e sw esperam 2, or espera 1"},
        {"a2", 5, 9, 9, "4 nops; addi t0 nao pode subir (escrita apos leitura do add)"},
        {"a3", 5, 9, 8, "4 nops; o add zero (sem efeito) preenche uma espera: 3"},
        {"b2", 5, 13, 13, "8 nops: cadeia sem nada independente"},
        {"c1", 6, 14, 8, "8 nops caem para 2 intercalando as duas metades"},
        {"c2", 5, 7, 5, "2 nops somem: as duas instrucoes do meio preenchem"},
        {"d1", 9, 16, 12, "laco de 6 (1 nop no slot); slli+sw fora do laco pedem 2"},
        {"d2", 5, 10, 10, "o slot so teria instrucao de depois do desvio"},
        {"d3", 7, 8, 7, "uma instrucao independente preenche o slot"},
    };
    std::cout << "  caso   original  so nops  reordenado   nota\n";
    for (const ListaCase& c : cases) {
        const Outcome o = verify("lista " + c.name, loadHex("lista/" + c.name + ".hex"), base);
        expect("lista " + c.name + ": instrucoes originais", o.original == c.original);
        expect("lista " + c.name + ": so nops", o.nops == c.nops,
               "obtido " + std::to_string(o.nops) + ", esperado " + std::to_string(c.nops));
        expect("lista " + c.name + ": reordenado", o.reorder == c.reorder,
               "obtido " + std::to_string(o.reorder) + ", esperado " + std::to_string(c.reorder));
        std::cout << "  " << std::left << std::setw(7) << c.name << std::right << std::setw(8) << o.original
                  << std::setw(9) << o.nops << std::setw(12) << o.reorder << "   " << c.nota << '\n';
    }
}

}  // namespace

int main() {
    std::cout << "== codificacao de deslocamentos ==\n";
    testEncoding();
    std::cout << "== casos conhecidos ==\n";
    testKnownCases();
    std::cout << "== lista de exercicios do professor ==\n";
    testLista();
    std::cout << "== programas reais de Arquitetura I (ecall sem registradores, como na M1) ==\n";
    testRealPrograms(false);
    std::cout << "== programas reais de Arquitetura I (--ecall-regs: ecall como instrucao comum) ==\n";
    testRealPrograms(true);
    std::cout << "== fuzz ==\n";
    testFuzz();
    std::cout << '\n' << (checks - failures) << " de " << checks << " verificacoes passaram";
    std::cout << (failures ? "  -- FALHAS: " + std::to_string(failures) : std::string("  -- tudo certo")) << '\n';
    return failures == 0 ? 0 : 1;
}
