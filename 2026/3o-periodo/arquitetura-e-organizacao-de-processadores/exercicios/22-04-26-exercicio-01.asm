fat:
    addi sp, sp, -8     # aloca 8 bytes no stack
    sw a7, 0(sp)        # salva n (a7) no stack
    sw ra, 4(sp)        # salva endereço de retorno
    addi t0, zero, 1    # t0 = 1 (valor de comparação)
    ble a7, t0, fat_r1  # se n <= 1, vai para caso base

    addi a7, a7, -1     # n = n - 1
    jal ra, fat         # fat(n-1) recursivamente

    lw a7, 0(sp)        # restaura n do stack
    lw ra, 4(sp)        # restaura endereço de retorno
    addi sp, sp, 8      # libera stack
    mul a0, a0, a7      # a0 = fat(n-1) * n
    jalr zero, ra, 0    # retorna

fat_r1:
    lw a7, 0(sp)        # restaura n do stack
    lw ra, 4(sp)        # restaura endereço de retorno
    addi sp, sp, 8      # libera stack
    addi a0, zero, 1    # retorna 1 (caso base)
    jalr zero, ra, 0    # retorna