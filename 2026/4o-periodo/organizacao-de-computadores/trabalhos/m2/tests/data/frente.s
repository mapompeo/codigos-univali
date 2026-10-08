# Desvio para frente: o consumidor do alvo (fim) depende de t0, lido antes do desvio.
 lw   t0, 0(a0)
 lw   t1, 4(a0)
 blt  t0, t1, fim
 add  t2, t0, t1
 sw   t2, 8(a0)
fim:
 addi t3, t0, 1
 sw   t3, 12(a0)
