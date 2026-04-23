.data
vetor:
  .word 10, 20, 30, 40, 50
tamanho:
  .word 5

.text
_start:
  la   a0, vetor        # a0 = endereço base do vetor
  lw   a1, tamanho      # a1 = número de elementos
  li   a2, 0            # a2 = acumulador (soma)
  li   t0, 0            # t0 = índice i = 0

loop:
  bge t0, a1, fim         # se i >= tamanho, termina

  slli t1, t0, 2          # t1 = i * 4 (navega no vetor em bytes)
  add t2, a0, t1          # calcula o endereço de i na memória, pegando o endereço base e somando pela posição atual
  lw t3, 0(t2)            # t3 = vetor[i]
  add a2, a2, t3          # soma += vetor[i]
  addi t0, t0, 1          # i++
  j loop

fim: