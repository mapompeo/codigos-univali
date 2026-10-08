#!/usr/bin/env python3
"""Montador minimo de RV32I, so para gerar os casos de teste da M2.

Uso: python tests/asm.py tests/data/laco.s > tests/data/laco.hex
     python tests/asm.py programa.s -d programa.data > programa.hex

Aceita rotulos, comentarios (#), nomes ABI e xN, secoes .data/.text com
.asciz/.word/.space/.byte, e as pseudo la, li, mv, j, ret, nop, ble, bgt,
beqz, bnez. Segue o mapa de memoria padrao do RARS: texto em 0x00400000 e
dados em 0x10010000 (la = lui + addi, sempre 2 palavras). Saida: uma palavra
hexadecimal por linha, como o arquivo de memoria da M1; com -d, os dados
vao em outro arquivo (uma palavra de 32 bits por linha, little-endian).
(Autores: Jose Gabriel Santos Gomes, Matheus Pompeo Dias e Nathan Gustavo Reichert)
"""
import re
import sys

DATA_BASE = 0x10010000
TEXT_BASE = 0x00400000

ABI = ("zero ra sp gp tp t0 t1 t2 s0 s1 a0 a1 a2 a3 a4 a5 a6 a7 "
       "s2 s3 s4 s5 s6 s7 s8 s9 s10 s11 t3 t4 t5 t6").split()
REG = {n: i for i, n in enumerate(ABI)}
REG["fp"] = 8
REG.update({f"x{i}": i for i in range(32)})

R_OPS = {"add": (0, 0), "sub": (0, 0x20), "sll": (1, 0), "slt": (2, 0), "sltu": (3, 0),
         "xor": (4, 0), "srl": (5, 0), "sra": (5, 0x20), "or": (6, 0), "and": (7, 0),
         "mul": (0, 1), "div": (4, 1), "divu": (5, 1), "rem": (6, 1), "remu": (7, 1)}
I_OPS = {"addi": 0, "slti": 2, "sltiu": 3, "xori": 4, "ori": 6, "andi": 7}
SH_OPS = {"slli": (1, 0), "srli": (5, 0), "srai": (5, 0x20)}
LOADS = {"lb": 0, "lh": 1, "lw": 2, "lbu": 4, "lhu": 5}
STORES = {"sb": 0, "sh": 1, "sw": 2}
BRANCHES = {"beq": 0, "bne": 1, "blt": 4, "bge": 5, "bltu": 6, "bgeu": 7}


def reg(name):
    return REG[name.strip()]


def num(text):
    return int(text.strip(), 0)


def enc_b(f3, rs1, rs2, off):
    u = off & 0x1FFF
    return (((u >> 12) & 1) << 31 | ((u >> 5) & 0x3F) << 25 | rs2 << 20 | rs1 << 15 |
            f3 << 12 | ((u >> 1) & 0xF) << 8 | ((u >> 11) & 1) << 7 | 0x63)


def enc_j(rd, off):
    u = off & 0x1FFFFF
    return (((u >> 20) & 1) << 31 | ((u >> 1) & 0x3FF) << 21 | ((u >> 11) & 1) << 20 |
            ((u >> 12) & 0xFF) << 12 | rd << 7 | 0x6F)


def hi_lo(value):
    """Divide um valor de 32 bits em (lui, addi) com a correcao do sinal do addi."""
    value &= 0xFFFFFFFF
    lo = value & 0xFFF
    if lo >= 0x800:
        lo -= 0x1000
    return ((value - lo) >> 12) & 0xFFFFF, lo


def parse_string(raw):
    body = raw[raw.index('"') + 1: raw.rindex('"')]
    out = bytearray()
    i = 0
    while i < len(body):
        ch = body[i]
        if ch == "\\" and i + 1 < len(body):
            esc = body[i + 1]
            out += {"n": b"\n", "t": b"\t", "0": b"\0", "\\": b"\\", '"': b'"'}.get(esc, esc.encode())
            i += 2
        else:
            out += ch.encode("utf-8")
            i += 1
    return bytes(out)


def split_source(text):
    """Separa em (rotulos, secao, instrucao/diretiva) por linha logica."""
    items = []
    section = "text"
    for raw in text.splitlines():
        body = raw.split("#")[0].strip() if '"' not in raw else raw
        if '"' in raw:  # o # pode estar dentro de uma string: corta so depois da ultima aspa
            last = raw.rindex('"')
            body = raw[: last + 1] + raw[last + 1:].split("#")[0]
        body = body.strip()
        labels = []
        while True:
            m = re.match(r"^([A-Za-z_]\w*):\s*(.*)$", body)
            if not m:
                break
            labels.append(m.group(1))
            body = m.group(2)
        if body.startswith(".data"):
            section, body = "data", body[5:].strip()
        elif body.startswith(".text"):
            section, body = "text", body[5:].strip()
        if re.match(r"^ecall\.data", body):
            body = "ecall"
        items.append((labels, section, body))
    return items


def tokens(body):
    op, _, rest = body.replace(",", " ").partition(" ")
    return op, rest.split()


def assemble(text):
    items = split_source(text)

    # passo 1: tamanho de cada item, enderecos de rotulos
    labels = {}
    text_items, data_bytes = [], bytearray()
    pending = []
    for lab, section, body in items:
        pending += lab
        if not body:
            continue
        if section == "data":
            op, args = tokens(body)
            if op == ".word":
                while len(data_bytes) % 4:
                    data_bytes += b"\0"
            for name in pending:
                labels[name] = DATA_BASE + len(data_bytes)
            pending = []
            if op == ".asciz" or op == ".string":
                data_bytes += parse_string(body) + b"\0"
            elif op == ".ascii":
                data_bytes += parse_string(body)
            elif op == ".word":
                while len(data_bytes) % 4:
                    data_bytes += b"\0"
                for a in args:
                    data_bytes += (num(a) & 0xFFFFFFFF).to_bytes(4, "little")
            elif op == ".byte":
                for a in args:
                    data_bytes += bytes([num(a) & 0xFF])
            elif op == ".space":
                data_bytes += bytes(num(args[0]))
            elif op == ".align":
                pass
            else:
                raise SystemExit(f"diretiva nao suportada: {body}")
        else:
            op, args = tokens(body)
            size = 1
            if op == "la" or (op in LOADS and len(args) == 2 and not re.search(r"\(", args[1])):
                size = 2
            elif op == "li":
                v = num(args[1])
                size = 1 if -2048 <= v <= 2047 else 2
            for name in pending:
                labels[name] = TEXT_BASE + 4 * sum(i[3] for i in text_items)
            pending = []
            text_items.append((op, args, body, size))
    for name in pending:
        labels[name] = TEXT_BASE + 4 * sum(i[3] for i in text_items)

    # passo 2: codificacao
    words = []

    def here():
        return TEXT_BASE + 4 * len(words)

    def off(label):
        return labels[label] - here()

    for op, args, body, _ in text_items:
        if op == "nop":
            words.append(0x13)
        elif op == "mv":
            words.append(reg(args[1]) << 15 | reg(args[0]) << 7 | 0x13)
        elif op == "li":
            v = num(args[1])
            if -2048 <= v <= 2047:
                words.append((v & 0xFFF) << 20 | reg(args[0]) << 7 | 0x13)
            else:
                hi, lo = hi_lo(v)
                words.append(hi << 12 | reg(args[0]) << 7 | 0x37)
                words.append((lo & 0xFFF) << 20 | reg(args[0]) << 15 | reg(args[0]) << 7 | 0x13)
        elif op == "la":
            hi, lo = hi_lo(labels[args[1]])
            rd = reg(args[0])
            words.append(hi << 12 | rd << 7 | 0x37)
            words.append((lo & 0xFFF) << 20 | rd << 15 | rd << 7 | 0x13)
        elif op in LOADS and len(args) == 2 and "(" not in args[1]:
            hi, lo = hi_lo(labels[args[1]])
            rd = reg(args[0])
            words.append(hi << 12 | rd << 7 | 0x37)
            words.append((lo & 0xFFF) << 20 | rd << 15 | LOADS[op] << 12 | rd << 7 | 0x03)
        elif op == "j":
            words.append(enc_j(0, off(args[0])))
        elif op == "ret":
            words.append(1 << 15 | 0x67)
        elif op == "jal":
            if len(args) == 1:
                words.append(enc_j(1, off(args[0])))
            else:
                words.append(enc_j(reg(args[0]), off(args[1])))
        elif op == "jalr":
            if len(args) == 2:
                m = re.match(r"(-?\w+)\((\w+)\)", args[1])
                words.append((num(m.group(1)) & 0xFFF) << 20 | reg(m.group(2)) << 15 | reg(args[0]) << 7 | 0x67)
            else:
                words.append((num(args[2]) & 0xFFF) << 20 | reg(args[1]) << 15 | reg(args[0]) << 7 | 0x67)
        elif op == "ecall":
            words.append(0x73)
        elif op in R_OPS:
            f3, f7 = R_OPS[op]
            words.append(f7 << 25 | reg(args[2]) << 20 | reg(args[1]) << 15 | f3 << 12 | reg(args[0]) << 7 | 0x33)
        elif op in I_OPS:
            words.append((num(args[2]) & 0xFFF) << 20 | reg(args[1]) << 15 | I_OPS[op] << 12 | reg(args[0]) << 7 | 0x13)
        elif op in SH_OPS:
            f3, f7 = SH_OPS[op]
            words.append(f7 << 25 | (num(args[2]) & 0x1F) << 20 | reg(args[1]) << 15 | f3 << 12 | reg(args[0]) << 7 | 0x13)
        elif op in LOADS:
            m = re.match(r"(-?\w+)\((\w+)\)", args[1])
            words.append((num(m.group(1)) & 0xFFF) << 20 | reg(m.group(2)) << 15 | LOADS[op] << 12 | reg(args[0]) << 7 | 0x03)
        elif op in STORES:
            m = re.match(r"(-?\w+)\((\w+)\)", args[1])
            imm = num(m.group(1)) & 0xFFF
            words.append((imm >> 5) << 25 | reg(args[0]) << 20 | reg(m.group(2)) << 15 | STORES[op] << 12 | (imm & 0x1F) << 7 | 0x23)
        elif op in BRANCHES:
            words.append(enc_b(BRANCHES[op], reg(args[0]), reg(args[1]), off(args[2])))
        elif op in ("ble", "bgt", "bleu", "bgtu"):
            base = {"ble": 5, "bgt": 4, "bleu": 7, "bgtu": 6}[op]  # ble a,b = bge b,a ; bgt a,b = blt b,a
            words.append(enc_b(base, reg(args[1]), reg(args[0]), off(args[2])))
        elif op in ("beqz", "bnez"):
            words.append(enc_b(0 if op == "beqz" else 1, reg(args[0]), 0, off(args[1])))
        elif op in ("lui", "auipc"):
            words.append((num(args[1]) & 0xFFFFF) << 12 | reg(args[0]) << 7 | (0x37 if op == "lui" else 0x17))
        else:
            raise SystemExit(f"instrucao nao suportada: {body}")
        words[-1] &= 0xFFFFFFFF
    while len(data_bytes) % 4:
        data_bytes += b"\0"
    data_words = [int.from_bytes(data_bytes[i:i + 4], "little") for i in range(0, len(data_bytes), 4)]
    return words, data_words


if __name__ == "__main__":
    args = sys.argv[1:]
    data_out = None
    if "-d" in args:
        i = args.index("-d")
        data_out = args[i + 1]
        del args[i:i + 2]
    code, data = assemble(open(args[0], encoding="utf-8", errors="replace").read())
    for word in code:
        print(f"0x{word:08x}")
    if data_out:
        with open(data_out, "w", encoding="utf-8", newline="\n") as f:
            for word in data:
                f.write(f"0x{word:08x}\n")
