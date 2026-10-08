.data
    msg_n:   .asciz "Informe o valor de N: "
    msg_r:   .asciz "Fibonacci de N e: "
    newline: .asciz "\n"

.text 

_start:
    addi a7, zero, 4
    la   a0, msg_n
    ecall
    addi a7, zero, 5
    ecall
    add  s0, zero, a0       # s0 = N

    li   t1, 0
    li   t2, 1
    li   t3, 0              # contador i = 0

loop:
    bge  t3, s0, fim        # se i >= N, termina
    add  t4, t1, t2         # t4 = t1 + t2
    add  t1, zero, t2       # t1 = t2
    add  t2, zero, t4       # t2 = t4
    addi t3, t3, 1          # i++
    j    loop

fim:
    addi a7, zero, 4
    la   a0, msg_r
    ecall
    
    addi a7, zero, 1
    add  a0, zero, t1
    ecall
    
    addi a7, zero, 4
    la   a0, newline
    ecall
    
    addi a7, zero, 10
    ecall