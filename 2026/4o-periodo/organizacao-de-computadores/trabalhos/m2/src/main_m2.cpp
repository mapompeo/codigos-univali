// Pratica M2 -- conflitos de dados e de controle no pipeline.
//
// Autores: Jose Gabriel Santos Gomes, Matheus Pompeo Dias e Nathan Gustavo Reichert
//
// Le a mesma memoria de instrucoes da M1, mostra os conflitos detectados,
// gera duas versoes corrigidas (so nops; reordenacao + nops) e o relatorio de custo.

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "hazards.hpp"
#include "riscv.hpp"

namespace {

namespace fs = std::filesystem;

struct Options {
    std::string inputPath;
    std::string outputDir = ".";
    uint32_t baseAddress = 0x00000000;
    bool ecallRegisters = false;
};

void printUsage(const char* program) {
    std::cout << "Uso: " << program << " [arquivo-rom] [opcoes]\n\n"
              << "Opcoes:\n"
              << "  -b, --base <endereco>   Endereco-base do PC (padrao: 0x00000000)\n"
              << "  -o, --out <pasta>       Pasta dos arquivos de saida (padrao: .)\n"
              << "      --ecall-regs        Trata o ecall como leitor de a7/a0 e escritor de a0\n"
              << "                          (padrao: sem registradores, como no decodificador da M1)\n"
              << "  -h, --help              Mostra esta ajuda\n\n"
              << "Sem arquivo-rom, o programa pergunta o caminho. Saidas, para entrada\n"
              << "'prog.hex': prog_nops.hex/.lst e prog_reordenado.hex/.lst.\n";
}

uint32_t parseAddress(const std::string& text) {
    const int base = (text.rfind("0x", 0) == 0 || text.rfind("0X", 0) == 0) ? 16 : 10;
    return static_cast<uint32_t>(std::stoul(text, nullptr, base));
}

bool parseArguments(int argc, char** argv, Options& options, std::string& error) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        const bool needsValue = (arg == "-b" || arg == "--base" || arg == "-o" || arg == "--out");
        if (needsValue && i + 1 >= argc) {
            error = "faltou o valor de " + arg;
            return false;
        }
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            std::exit(0);
        } else if (arg == "-b" || arg == "--base") {
            try {
                options.baseAddress = parseAddress(argv[++i]);
            } catch (const std::exception&) {
                error = "endereco-base invalido";
                return false;
            }
        } else if (arg == "-o" || arg == "--out") {
            options.outputDir = argv[++i];
        } else if (arg == "--ecall-regs") {
            options.ecallRegisters = true;
        } else if (!arg.empty() && arg[0] == '-') {
            error = "opcao desconhecida: " + arg;
            return false;
        } else if (options.inputPath.empty()) {
            options.inputPath = arg;
        } else {
            error = "apenas um arquivo de entrada e aceito";
            return false;
        }
    }
    return true;
}

std::string hex32(uint32_t value) {
    std::ostringstream os;
    os << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
    return os.str();
}

std::string binary32(uint32_t value) {
    std::string out(32, '0');
    for (int bit = 0; bit < 32; ++bit) {
        if (value >> bit & 1u) out[static_cast<std::size_t>(31 - bit)] = '1';
    }
    return out;
}

// "add   t1, t0, s3": mnemonico alinhado, para as colunas do console e da listagem.
std::string aligned(const riscv::Instruction& inst) {
    const std::string text = riscv::toAssembly(inst);
    const std::size_t space = text.find(' ');
    if (space == std::string::npos) return text;
    std::ostringstream os;
    os << std::left << std::setw(6) << text.substr(0, space) << text.substr(space + 1);
    return os.str();
}

std::string percent(double value) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(1) << value;
    std::string text = os.str();
    for (char& ch : text) {
        if (ch == '.') ch = ',';  // o enunciado usa virgula decimal
    }
    return text + "%";
}

std::string line(const riscv::Instruction& inst, int width = 30) {
    std::ostringstream os;
    os << hex32(inst.pc) << "  " << std::left << std::setw(width) << aligned(inst);
    return os.str();
}

// --- Saida de conflitos (R1, R2) -------------------------------------------

void printConflicts(const riscv::Analysis& a, uint32_t base) {
    auto describe = [&](const riscv::DataHazard& h) {
        const riscv::Instruction& c = a.items[h.consumer].inst;
        const riscv::Instruction& p = a.items[h.producer].inst;
        std::cout << "  " << line(c) << " <- " << line(p) << " [" << riscv::abiName(h.reg)
                  << ", d=" << h.distance << ", " << h.nops() << " nops]\n";
    };

    std::cout << "CONFLITOS DE DADOS\n";
    int plain = 0;
    for (const riscv::DataHazard& h : a.hazards) {
        if (h.crossesBranch) continue;
        describe(h);
        ++plain;
    }
    if (plain == 0) std::cout << "  (nenhum)\n";

    std::cout << "\nCONFLITOS DE CONTROLE\n";
    if (a.controls.empty()) std::cout << "  (nenhum desvio no programa)\n";
    for (const riscv::ControlSlot& c : a.controls) {
        std::cout << "  " << line(a.items[c.branch].inst, 38) << "slot ocupado por: ";
        if (c.slot < 0) std::cout << "(nenhuma: o programa termina no desvio)\n";
        else std::cout << line(a.items[c.slot].inst, 0) << '\n';
    }

    std::cout << "\nCONFLITOS DE DADOS NA FRONTEIRA DOS DESVIOS\n";
    int crossing = 0;
    for (const riscv::DataHazard& h : a.hazards) {
        if (!h.crossesBranch) continue;
        describe(h);
        ++crossing;
    }
    if (crossing == 0) std::cout << "  (nenhum)\n";

    std::cout << "\nBLOCOS BASICOS\n";
    for (std::size_t b = 0; b < a.blocks.size(); ++b) {
        std::cout << "  B" << b << "  " << hex32(base + 4u * static_cast<uint32_t>(a.blocks[b].first))
                  << " .. " << hex32(base + 4u * static_cast<uint32_t>(a.blocks[b].last)) << '\n';
    }
}

// --- Arquivos de saida (R7) --------------------------------------------------

std::string marker(const riscv::Item& item, uint32_t base) {
    auto was = [&]() { return " (era " + hex32(base + 4u * static_cast<uint32_t>(item.orig)) + ")"; };
    switch (item.tag) {
        case riscv::Tag::Original: return "";
        case riscv::Tag::Reordered: return "<- reordenada" + was();
        case riscv::Tag::MovedSlot: return "<- movida para o slot de atraso" + was();
        case riscv::Tag::NopData: return "<- nop inserido (conflito de dados)";
        case riscv::Tag::NopSlot: return "<- nop no slot de atraso";
    }
    return "";
}

bool writeOutputs(const fs::path& stem, const riscv::Corrected& result, riscv::Radix radix,
                  uint32_t base, std::string& error) {
    std::ofstream rom(stem.string() + (radix == riscv::Radix::Binary ? ".bin" : ".hex"));
    std::ofstream listing(stem.string() + ".lst");
    if (!rom || !listing) {
        error = "nao foi possivel criar os arquivos em '" + stem.string() + "*'";
        return false;
    }
    listing << "ENDERECO    CODIGO      INSTRUCAO                     MARCACAO\n";
    for (std::size_t i = 0; i < result.items.size(); ++i) {
        const riscv::Item& item = result.items[i];
        rom << (radix == riscv::Radix::Binary ? binary32(item.inst.raw) : hex32(item.inst.raw)) << '\n';
        listing << hex32(item.inst.pc) << "  " << hex32(item.inst.raw) << "  " << std::left
                << std::setw(30) << aligned(item.inst) << marker(item, base) << '\n';
    }
    return true;
}

// --- Relatorio de custo (R8) -------------------------------------------------

void printCost(std::size_t original, const riscv::Corrected& nops, const riscv::Corrected& reorder) {
    auto row = [&](const char* label, std::size_t total) {
        std::cout << "  " << std::left << std::setw(22) << label << std::right << total << " instrucoes";
        if (total != original) {
            const std::size_t added = total - original;
            std::cout << "  (+" << added << ", +"
                      << percent(100.0 * static_cast<double>(added) / static_cast<double>(original))
                      << ")";
        }
        std::cout << '\n';
    };
    std::cout << "\nCUSTO\n";
    row("original ..........", original);
    row("somente nops ......", nops.items.size());
    row("nops + reordenacao ", reorder.items.size());
    std::cout << "\n  ciclos de uma passada (n + 4, pipeline vazio): " << original + 4 << " original, "
              << nops.items.size() + 4 << " so nops, " << reorder.items.size() + 4 << " reordenado\n";
    if (nops.weightedNops != nops.nopsData + nops.nopsSlot || reorder.weightedNops != reorder.nopsData + reorder.nopsSlot) {
        std::cout << "  nops contados por volta de laco (cada nivel de laco pesa 10): " << nops.weightedNops
                  << " so nops, " << reorder.weightedNops << " reordenado\n";
    }

    const long saved = static_cast<long>(nops.items.size()) - static_cast<long>(reorder.items.size());
    const std::size_t extra = nops.items.size() - original;
    std::cout << "\nComparacao: a reordenacao evitou " << saved << " de " << extra << " nops";
    if (extra > 0) {
        std::cout << " (" << percent(100.0 * static_cast<double>(saved) / static_cast<double>(extra)) << ")";
    }
    std::cout << ".\n  somente nops: " << nops.nopsData << " de dados + " << nops.nopsSlot
              << " em slots\n  reordenacao:  " << reorder.nopsData << " de dados + " << reorder.nopsSlot
              << " em slots, " << reorder.movedToSlot << " instrucao(oes) movida(s) para slots, "
              << reorder.reordered << " reordenada(s) dentro de blocos\n";
}

}  // namespace

int main(int argc, char** argv) {
    Options options;
    std::string error;
    if (!parseArguments(argc, argv, options, error)) {
        std::cerr << "erro: " << error << "\n\n";
        printUsage(argv[0]);
        return 1;
    }
    if (options.inputPath.empty()) {
        std::cout << "Arquivo com o programa em binario ou hexadecimal: ";
        std::getline(std::cin, options.inputPath);
    }

    std::ifstream input(options.inputPath);
    if (!input) {
        std::cerr << "erro: nao foi possivel abrir '" << options.inputPath << "'\n";
        return 1;
    }

    const riscv::ParseResult parsed = riscv::parseStream(input, options.baseAddress);
    for (const riscv::LineError& e : parsed.errors) {
        std::cerr << "aviso: linha " << e.line << " ignorada ('" << e.text << "': " << e.reason << ")\n";
    }
    std::vector<riscv::Instruction> program;
    for (const riscv::RawWord& word : parsed.words) program.push_back(riscv::decode(word.value, word.pc));
    if (program.empty()) {
        std::cerr << "erro: o arquivo nao tem instrucoes\n";
        return 1;
    }

    std::cout << "Arquivo: " << options.inputPath << " (" << riscv::toString(parsed.radix) << ", "
              << program.size() << " instrucoes, base " << hex32(options.baseAddress) << ")\n";
    for (const riscv::Instruction& inst : program) {
        if (!inst.valid()) {
            std::cout << "aviso: " << hex32(inst.pc) << " e invalida (" << inst.error
                      << "); tratada como barreira, nao se move\n";
        }
        if (inst.mnemonic == "auipc") {
            std::cout << "aviso: " << hex32(inst.pc)
                      << " usa auipc; enderecos relativos ao PC mudam quando ha insercao de nops\n";
        }
    }
    std::cout << '\n';

    riscv::Options model;
    model.ecallRegisters = options.ecallRegisters;
    const riscv::Analysis analysis = riscv::analyze(program, options.baseAddress, model);
    printConflicts(analysis, options.baseAddress);

    riscv::Corrected nops;
    riscv::Corrected reorder;
    try {
        nops = riscv::correct(program, options.baseAddress, false, model);
        reorder = riscv::correct(program, options.baseAddress, true, model);
    } catch (const std::exception& e) {
        std::cerr << "\nerro ao recalcular enderecos: " << e.what() << '\n';
        return 1;
    }

    printCost(program.size(), nops, reorder);
    std::cout << "\nVerificacao: conflitos de dados restantes: " << nops.residualHazards
              << " (somente nops), " << reorder.residualHazards << " (reordenacao)\n";

    const fs::path in(options.inputPath);
    const fs::path dir(options.outputDir);
    std::error_code ec;
    fs::create_directories(dir, ec);
    const riscv::Radix radix = parsed.radix == riscv::Radix::Unknown ? riscv::Radix::Hex : parsed.radix;
    if (!writeOutputs(dir / (in.stem().string() + "_nops"), nops, radix, options.baseAddress, error) ||
        !writeOutputs(dir / (in.stem().string() + "_reordenado"), reorder, radix, options.baseAddress, error)) {
        std::cerr << "erro: " << error << '\n';
        return 1;
    }
    std::cout << "Arquivos gerados em '" << dir.string() << "': " << in.stem().string()
              << "_nops.* e " << in.stem().string() << "_reordenado.* (.hex/.bin e .lst)\n";
    return 0;
}
