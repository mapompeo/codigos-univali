# Disciplina: Arquitetura e Organização de Processadores
# Atividade: Avaliação 01 – Programação em Linguagem de Montagem
# Programa 03
# Grupo: - José Gabriel
#        - Matheus Pompeo

.data
	msgA: .asciz "Insira um número: "
	msgResposta: .asciz "\nA soma dos digitos é: "



.text
	addi a7, zero, 4
	la a0, msgA
	ecall
	
	addi a7, zero, 5
	ecall
	
	add t0, zero, a0      
	

	addi t1, zero, 10 
	
	add a0, zero, t0 # variavel int N
	addi a2, zero, 10 # 10

	
	jal ra, somaDigitos
	
	addi t1, a0, 0      
	
	addi a7, zero, 4
	la a0, msgResposta
	ecall
	
	addi a7, zero, 1
	addi a0, t1, 0
	ecall
	
	addi a7, zero, 10
	ecall
	
	
	

	somaDigitos:
		blt a0, a2, returnN
		
		addi sp, sp, -8
	  	sw ra, 4(sp)
		
		add t0, zero, a0 # salva N (a0) antes de bagunçar
		add a1, zero, a2
		jal ra, calcularResto
		
		
		add t1, zero, a0 # salva o retorno de "calcularResto" em t1
		sw t1, 0(sp)
		
		add a0, zero, t0
		div a0, a0, a2
		jal ra, somaDigitos
		
		lw t1, 0(sp)
		add a0, t1, a0
		
		lw ra, 4(sp)
		addi sp, sp, 8
		
		jal zero, returnN
		
		
	returnN:
		jalr, zero, ra, 0


  	
	
	calcularResto:
		beq a1, zero, fim
	while:
		blt a0, a1, fim
		sub a0, a0, a1
		 jal zero, while
	fim:
		jalr zero, ra, 0
	
	