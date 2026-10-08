# Programa no estilo do exemplo do enunciado: laco com load-use e store logo depois.
inicio:
 lw   t0, 0(a0)
 add  t1, t0, s3
 sw   t1, 0(a0)
 addi a0, a0, 4
 addi s1, s1, -1
 slli s0, s0, 1
 bne  s1, zero, inicio
 addi t2, zero, 7
 sw   t2, 0(a1)
