# Tres pares produtor/consumidor colados: intercalando os pares, a reordenacao
# elimina todos os nops.
 addi t0, zero, 1
 add  t1, t0, t0
 addi t2, zero, 2
 add  t3, t2, t2
 addi t4, zero, 3
 add  t5, t4, t4
 sw   t1, 0(a0)
 sw   t3, 4(a0)
 sw   t5, 8(a0)
