.data
    texto: .asciz "Digite o primeiro inteiro: "
    textoDois: .asciz "Digite o segundo inteiro: "
.text
    la a0, texto        # carrega endereço da 1ª string
    li a7, 4            # serviço: printar string
    ecall

    li a7, 5            # serviço: ler inteiro
    ecall
    mv t0, a0           # salva 1º número em t0

    la a0, textoDois    # carrega endereço da 2ª string
    li a7, 4            # serviço: printar string
    ecall

    li a7, 5            # serviço: ler inteiro
    ecall
    mv t1, a0           # salva 2º número em t1

    add a0, t0, t1      # soma t0 + t1, resultado em a0
    li a7, 1            # serviço: printar inteiro
    ecall

    li a7, 10           # serviço: encerrar
    ecall