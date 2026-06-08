.data
    msg_a:   .asciz "Informe o primeiro valor: "
    msg_b:   .asciz "\nInforme o segundo valor: "
    msg_r:   .asciz "\nMDC: "
    newline: .asciz "\n"

.text
_start:
    addi a7, zero, 4
    la   a0, msg_a
    ecall
    addi a7, zero, 5
    ecall
    add  s0, zero, a0       

    addi a7, zero, 4
    la   a0, msg_b
    ecall
    addi a7, zero, 5
    ecall
    add  s1, zero, a0       

# === WHILE: while (b != 0) ===
mdc_loop:
    beq  s1, zero, fim      # se b == 0, termina

    rem  t0, s0, s1         # t0 = a % b
    add  s0, zero, s1       # a = b
    add  s1, zero, t0       # b = resto

    j    mdc_loop

fim:
    addi a7, zero, 4
    la   a0, msg_r
    ecall

    addi a7, zero, 1
    add  a0, zero, s0
    ecall

    addi a7, zero, 4
    la   a0, newline
    ecall

    addi a7, zero, 10
    ecall