# Cadeia de dependencias sem nenhuma instrucao independente: nenhuma
# reordenacao ajuda, so nops (2 antes de cada consumidor).
 addi t0, zero, 5
 addi t1, t0, 1
 addi t2, t1, 1
 add  t3, t2, t1
 sw   t3, 0(a0)
