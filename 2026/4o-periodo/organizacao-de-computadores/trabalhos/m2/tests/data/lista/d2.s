# Lista de exercicios, D2
 lw   t0, 0(a0)
 beq  t0, zero, fim
 addi t1, t0, -1
 sw   t1, 0(a0)
fim:
 addi a0, a0, 4
