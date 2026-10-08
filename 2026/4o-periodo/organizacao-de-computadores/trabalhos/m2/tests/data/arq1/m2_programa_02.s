# Disciplina: Arquitetura e Organização de Processadores
# Atividade: Avaliação 01 – Programação em Linguagem de Montagem
# Programa 02
# Grupo: - José Gabriel
#        - Matheus Pompeo

.data
	msgA: .asciz "Insira um número: "
	msgResposta: .asciz "\nO número é primo: "



.text
	addi a7, zero, 4
	la a0, msgA
	ecall
	
	addi a7, zero, 5
	ecall
	add t0, zero, a0      
	

	addi t1, zero, 2  # I = 2
	
	add a0, zero, t0 # variavel int a
	add a1, zero, t1 # I = 2
	
	jal ra, ehprimo
	
	addi t1, a0, 0      
	
	addi a7, zero, 4
	la a0, msgResposta
	ecall
	
	addi a7, zero, 1
	addi a0, t1, 0
	ecall
	
	addi a7, zero, 10
	ecall

ehprimo:
  	addi sp, sp, -8
  	sw ra, 4(sp)

  	blt a0, a1, return0

  	addi t3, a0, 0

loop:
  	addi a0, t3, 0
 	jal ra, calcularResto 
  
  	addi t2, zero, 0
  	beq a0, t2, return0
  
  	addi a1, a1, 1
  	blt a1, t3, loop

  	jal zero, return1

  	return1:
  	addi a0, zero, 1

  	lw ra, 4(sp)
  	addi sp, sp 8 # pra fecha o SP antes de retornar
  	jalr zero, ra, 0
	    
	return0:
	addi a0, zero, 0
	  
	lw ra, 4(sp)
	addi sp, sp, 8 # pra fecha o SP antes de retornar
	jalr zero, ra, 0
	
	
	
	calcularResto:
		beq a1, zero, fim
	while:
		blt a0, a1, fim
		sub a0, a0, a1
		 jal zero, while
	fim:
		jalr zero, ra, 0
	
	