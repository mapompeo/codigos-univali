# Disciplina: Arquitetura e Organização de Processadores
# Atividade: Avaliação 01 – Programação em Linguagem de Montagem
# Programa 01
# Grupo: - José Gabriel
#        - Matheus Pompeo



.data
	msgA: .asciz "Insira um número a ser dividido: "
	msgB: .asciz "Insira o número divisor: "
	msgResposta: .asciz "O resto da divisão é: "
	
.text

	addi a7, zero, 4
	la a0, msgA
	ecall
	
	addi a7, zero, 5
	ecall
	add t0, zero, a0      
	
	addi a7, zero, 4
	la a0, msgB
	ecall
	
	addi a7, zero, 5
	ecall
	add t1, zero, a0  
	
	add a0, zero, t0        
	add a1, zero, t1
	
	jal ra, calcularResto
	
	addi t1, a0, 0      
	
	addi a7, zero, 4
	la a0, msgResposta
	ecall
	
	addi a7, zero, 1
	addi a0, t1, 0
	ecall
	
	addi a7, zero, 10
	ecall
	
	
calcularResto:
	beq a1, zero, fim
while:

	blt a0, a1, fim
	sub a0, a0, a1
	jal zero, while

fim:
	jalr zero, ra, 0
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	