.data
dias: .word 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
msg_aula: .asciz "Entre com o numero da aula (de 0 a 15): "
msg_aluno: .asciz "Entre com o numero do aluno (de 0 a 31): "
msg_tipo: .asciz "Entre com o tipo do registro (presenca=1; ausencia=0): "

.text
# Disciplina: Arquitetura e Organizacao de Computadores
# Atividade: Avaliacao 01 - Programacao em Linguagem de Montagem
# Programa 01
# Nome: Jose Gabriel Santos Gomes, Matheus Pompeo Dias e Nathan Gustavo Reichert

_start:
addi a7, zero, 4
la a0, msg_aula
ecall
addi a7, zero, 5
ecall
add s0, zero, a0

addi a7, zero, 4
la a0, msg_aluno
ecall
addi a7, zero, 5
ecall
add s1, zero, a0

addi a7, zero, 4
la a0, msg_tipo
ecall
addi a7, zero, 5
ecall
add s2, zero, a0

slli t0, s0, 2
la t1, dias
add t1, t1, t0
lw t2, 0(t1)

addi t3, zero, 1
sll t3, t3, s1

addi t6, zero, 1
beq s2, t6, eh_presenca

eh_ausencia:
addi t4, zero, -1
xor t3, t3, t4
and t2, t2, t3
jal zero, fim_registro

eh_presenca:
or t2, t2, t3

fim_registro:
sw t2, 0(t1)
jal zero, _start