# Laco com desvio para tras: soma de 4 palavras. O bne usa a1 logo apos o addi.
 li   a1, 4
 li   t2, 0
loop:
 lw   t0, 0(a0)
 add  t2, t2, t0
 addi a0, a0, 4
 addi a1, a1, -1
 bne  a1, zero, loop
 sw   t2, 0(s0)
