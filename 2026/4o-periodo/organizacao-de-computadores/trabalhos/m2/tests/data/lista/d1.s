# Lista de exercicios, D1: soma 10 elementos e guarda o dobro
 addi t0, zero, 0
 addi t1, zero, 10
loop:
 lw   t2, 0(a0)
 add  s0, s0, t2
 addi a0, a0, 4
 addi t0, t0, 1
 bne  t0, t1, loop
 slli s0, s0, 1
 sw   s0, 0(a1)
