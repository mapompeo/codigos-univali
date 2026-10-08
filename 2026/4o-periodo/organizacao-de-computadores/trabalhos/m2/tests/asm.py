#!/usr/bin/env python3
"""Montador minimo de RV32I, so para gerar os casos de teste da M2.

Uso: python tests/asm.py tests/data/laco.s > tests/data/laco.hex

Aceita rotulos ("loop:"), comentarios (#), nomes ABI (t0, a0, s1...) e xN,
as instrucoes usadas nos testes e as pseudo nop, mv, li (12 bits), j, ret.
Saida: uma palavra hexadecimal por linha, como o arquivo de memoria da M1.
(Autores: Jose Gabriel Santos Gomes, Matheus Pompeo Dias e Nathan Gustavo Reichert)
"""
import re
import sys

ABI = ("zero ra sp gp tp t0 t1 t2 s0 s1 a0 a1 a2 a3 a4 a5 a6 a7 "
       "s2 s3 s4 s5 s6 s7 s8 s9 s10 s11 t3 t4 t5 t6").split()
REG = {n: i for i, n in enumerate(ABI)}
REG["fp"] = 8
REG.update({f"x{i}": i for i in range(32)})

R_OPS = {"add": (0, 0), "sub": (0, 0x20), "sll": (1, 0), "slt": (2, 0), "sltu": (3, 0),
         "xor": (4, 0), "srl": (5, 0), "sra": (5, 0x20), "or": (6, 0), "and": (7, 0),
         "mul": (0, 1)}
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


def assemble(text):
    lines, labels = [], {}
    for raw in text.splitlines():
        body = raw.split("#")[0].strip()
        while True:
            m = re.match(r"^(\w+):\s*(.*)$", body)
            if not m:
                break
            labels[m.group(1)] = len(lines)
            body = m.group(2)
        if body:
            lines.append(body)

    words = []
    for index, body in enumerate(lines):
        op, _, rest = body.partition(" ")
        args = [a.strip() for a in rest.split(",")] if rest.strip() else []

        def target(label):
            return (labels[label] - index) * 4

        if op == "nop":
            w = 0x13
        elif op == "mv":
            w = reg(args[1]) << 15 | reg(args[0]) << 7 | 0x13
        elif op == "li":
            w = (num(args[1]) & 0xFFF) << 20 | reg(args[0]) << 7 | 0x13
        elif op == "j":
            w = enc_j(0, target(args[0]))
        elif op == "ret":
            w = 1 << 15 | 0x67
        elif op == "jal":
            w = enc_j(reg(args[0]), target(args[1]))
        elif op == "jalr":
            m = re.match(r"(-?\w+)\((\w+)\)", args[1])
            w = (num(m.group(1)) & 0xFFF) << 20 | reg(m.group(2)) << 15 | reg(args[0]) << 7 | 0x67
        elif op in R_OPS:
            f3, f7 = R_OPS[op]
            w = f7 << 25 | reg(args[2]) << 20 | reg(args[1]) << 15 | f3 << 12 | reg(args[0]) << 7 | 0x33
        elif op in I_OPS:
            w = (num(args[2]) & 0xFFF) << 20 | reg(args[1]) << 15 | I_OPS[op] << 12 | reg(args[0]) << 7 | 0x13
        elif op in SH_OPS:
            f3, f7 = SH_OPS[op]
            w = f7 << 25 | (num(args[2]) & 0x1F) << 20 | reg(args[1]) << 15 | f3 << 12 | reg(args[0]) << 7 | 0x13
        elif op in LOADS:
            m = re.match(r"(-?\w+)\((\w+)\)", args[1])
            w = (num(m.group(1)) & 0xFFF) << 20 | reg(m.group(2)) << 15 | LOADS[op] << 12 | reg(args[0]) << 7 | 0x03
        elif op in STORES:
            m = re.match(r"(-?\w+)\((\w+)\)", args[1])
            imm = num(m.group(1)) & 0xFFF
            w = (imm >> 5) << 25 | reg(args[0]) << 20 | reg(m.group(2)) << 15 | STORES[op] << 12 | (imm & 0x1F) << 7 | 0x23
        elif op in BRANCHES:
            w = enc_b(BRANCHES[op], reg(args[0]), reg(args[1]), target(args[2]))
        elif op in ("lui", "auipc"):
            w = (num(args[1]) & 0xFFFFF) << 12 | reg(args[0]) << 7 | (0x37 if op == "lui" else 0x17)
        else:
            raise SystemExit(f"instrucao nao suportada: {body}")
        words.append(w & 0xFFFFFFFF)
    return words


if __name__ == "__main__":
    for word in assemble(open(sys.argv[1], encoding="utf-8").read()):
        print(f"0x{word:08x}")
