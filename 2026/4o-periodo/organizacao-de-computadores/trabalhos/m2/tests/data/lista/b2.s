# Lista de exercicios, B2: incrementa em 10 o elemento a1 do vetor a0
 slli t0, a1, 2
 add  t0, a0, t0
 lw   t1, 0(t0)
 addi t1, t1, 10
 sw   t1, 0(t0)
