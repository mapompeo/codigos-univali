# Roteiro de Apresentação
**Trabalho M3 — Processador RISC-V 32-bit Monociclo**  
Arquitetura e Organização de Processadores · UNIVALI 2026  
Tempo estimado: 10–15 minutos

---

## Pessoa 1 — Introdução e visão geral

Fala, pessoal. Neste trabalho vamos apresentar a implementação de um processador RISC-V de 32 bits, desenvolvido do zero em VHDL.

Um processador é o componente que lê uma instrução da memória, entende o que ela pede, executa a operação e salva o resultado. Nós implementamos exatamente isso — cada peça do circuito, componente por componente.

A arquitetura escolhida foi o RISC-V, uma ISA aberta criada pela Universidade de Berkeley, usada em chips reais hoje em dia. O modelo que implementamos é o monociclo: cada instrução é executada em exatamente um ciclo de clock, do início ao fim. É o design mais simples possível — sem pipeline, sem cache, sem hazards.

O processador é dividido em dois grandes blocos. O primeiro é o datapath, que é por onde os dados circulam: registradores, ULA, memórias. O segundo é a unidade de controle, que lê a instrução e decide o que cada componente deve fazer. Juntos, eles formam o circuito completo.

O fluxo de uma instrução sempre começa no PC — o Program Counter — que guarda o endereço da instrução atual. Esse endereço vai para a InstrFile, que é a memória de instruções, e ela devolve a instrução de 32 bits. A partir daí, os campos da instrução são distribuídos para os outros componentes: o opcode vai para a Unidade de Controle, os campos de registrador vão para o RegFile, e o campo imediato vai para o ImmGen. Tudo isso acontece ao mesmo tempo, de forma combinacional — o clock só serve para sincronizar as escritas no final do ciclo.

---

## Pessoa 2 — Os componentes do datapath

Vou detalhar como cada componente funciona.

A **Unidade de Controle** recebe três campos da instrução — opcode, funct3 e funct7b5 — e com isso gera todos os sinais que governam o comportamento do processador naquele ciclo: se vai escrever em registrador, se o segundo operando da ULA é um registrador ou um imediato, se vai ler ou escrever na memória, e se a instrução é um desvio. É ela que entende o que cada instrução quer fazer.

O **RegFile** é o banco com 32 registradores de 32 bits, de x0 a x31. O x0 é especial — ele é hardwired zero: qualquer escrita nele é descartada e qualquer leitura retorna zero. A leitura é combinacional e a escrita acontece na borda de subida do clock.

O **ImmGen** extrai o valor imediato embutido na instrução e faz extensão de sinal para 32 bits. Ele suporta todos os cinco formatos do RISC-V — I-type, S-type, B-type, U-type e J-type — cada um com os bits em posições diferentes na instrução.

O **MUX** escolhe o segundo operando que vai entrar na ULA: pode ser o valor de rs2, vindo do RegFile, ou o imediato gerado pelo ImmGen. Quem decide é o sinal alu_src da Unidade de Controle.

A **ULA** é onde a operação de fato acontece. Ela executa dez operações: ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, SLT e SLTU. Ela também produz um sinal chamado zero, que fica em 1 quando o resultado é zero — esse sinal é usado pela lógica de branch para decidir se o desvio é tomado ou não.

A **DataMem** é a memória de dados, usada pelas instruções LW e SW. O endereço vem do resultado da ULA, calculado como rs1 mais o imediato. A leitura é combinacional e a escrita é síncrona.

Por fim, o **mux de writeback** decide o que vai ser gravado de volta no registrador destino. Dependendo da instrução, pode ser o resultado da ULA, o dado lido da memória, o imediato direto no caso do LUI, o PC mais imediato no caso do AUIPC, ou o PC mais 4 no caso de um salto — que é o endereço de retorno do JAL e JALR.

---

## Pessoa 3 — Programa de teste e resultados

Para validar o processador, implementamos um programa de teste com 11 instruções diretamente na InstrFile.

O programa começa carregando dois valores nos registradores: x5 recebe 6 e x6 recebe 4, usando ADDI. Em seguida, testamos as quatro operações principais: ADD soma os dois e guarda 10 em x7, SUB subtrai e guarda 6 em x8, AND faz a operação bit a bit e guarda 4 em x9, e OR guarda 6 em x10.

Depois testamos a memória de dados. A instrução SW grava o valor de x5, que é 6, na posição 0 da memória. Em seguida, LW lê esse mesmo valor de volta e coloca em x11. Isso valida tanto a escrita quanto a leitura da DataMem.

A instrução mais importante do programa é o BEQ, no PC 32. Ela compara x5 e x11 — os dois valem 6, então a condição é verdadeira. A ULA faz uma subtração, o resultado é zero, o sinal zero fica em 1, e o processador desvia para o PC 40, pulando completamente a instrução no PC 36, que tentaria gravar 99 em x12.

A prova definitiva de que o branch funcionou é que x12 permanece zero ao final da simulação. Se o desvio não tivesse sido tomado, x12 teria recebido 99. Confirmamos isso pela simulação feita no EDA Playground com o GHDL: o PC salta de 32 diretamente para 40, o sinal zero fica em 1 no ciclo do BEQ — confirmando que 6 menos 6 é zero — e x13 recebe 1 no ciclo seguinte. O projeto está disponível no repositório com todos os arquivos VHDL, o testbench e um manual detalhado. Obrigado.

---

> **Observação:** As referências visuais e marcações de ação foram removidas. O vídeo exibe os arquivos VHDL e o waveform no EDA Playground em looping — a narração foi adaptada para se sustentar de forma independente do visual.
