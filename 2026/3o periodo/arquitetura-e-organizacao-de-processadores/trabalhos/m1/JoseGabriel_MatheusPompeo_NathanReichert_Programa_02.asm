.data
msg_x: .asciz "Informe o valor de X: "
msg_y: .asciz "Informe o valor de Y: "
msg_res: .asciz "Resultado: "

.text
# Disciplina: Arquitetura e Organizacao de Computadores
# Atividade: Avaliacao 01 - Programacao em Linguagem de Montagem
# Programa 02
# Nome: Jose Gabriel Santos Gomes, Matheus Pompeo Dias e Nathan Gustavo Reichert

_start:
addi a7, zero, 4
la a0, msg_x
ecall
addi a7, zero, 5
ecall
add s0, zero, a0

addi a7, zero, 4
la a0, msg_y
ecall
addi a7, zero, 5
ecall
add s1, zero, a0

add s2, zero, zero

loop_mult:
bge zero, s1, fim_mult

andi t0, s1, 1
beq t0, zero, pula_soma

add s2, s2, s0

pula_soma:
slli s0, s0, 1
srli s1, s1, 1
jal zero, loop_mult

fim_mult:
addi a7, zero, 4
la a0, msg_res
ecall

add a0, zero, s2
addi a7, zero, 1
ecall

addi a7, zero, 10
ecall