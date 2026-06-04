-- =============================================================================
--  ALU.vhd  --  Unidade Lógica e Aritmética  --  RISC-V 32-bit Monociclo
-- =============================================================================
--
--  Operações (op):
--    0000 = ADD    0001 = SUB    0010 = AND    0011 = OR
--    0100 = XOR    0101 = SLL    0110 = SRL    0111 = SRA
--    1000 = SLT    1001 = SLTU
--
--  Saídas:
--    res  : resultado de 32 bits
--    zero : '1' quando res = 0  (usado por BEQ/BNE)
--
-- Funções úteis: 
--   SLL : std_logic_vector(shift_left (unsigned(i_a), shamt))
--   SRL : std_logic_vector(shift_right (unsigned(i_a), shamt))
--   SRA : std_logic_vector(shift_right (signed(i_a), shamt))
--
-- Dicas: 
-- SLT : res 1 quando a < b (com sinal)
-- SLT : res 1 quando a < b (sem sinal)
-- =============================================================================

library ieee;
use ieee.std_logic_1164.all; --entrada e saída
use ieee.numeric_std.all; --matemática

--std_logic é uma saída equivalente ao binário
entity design is
port (i_a, i_b : in std_logic_vector(31 downto 0); 
      i_sel: in std_logic_vector(1 downto 0); 
      o_data : out std_logic_vector(31 downto 0));
end design;

architecture qualquernome of design is
	signal w_SOMA, w_SUB, w_AND, w_OR :  std_logic_vector(31 downto 0);
begin
	--chamar uma função dentro de outra
    u_MUX41 : entity work.MUX41
    port map(w_SOMA, w_SUB, w_AND, w_OR, i_sel, o_data );
    
    --chamar uma função dentro de outra
    u_SOMA : entity work.SOMADOR
    port map(i_a, i_b, w_SOMA );
    
    u_SUB : entity work.SUBTRATOR
    port map(i_a, i_b, w_SUB );
    
    w_AND <= i_a and i_b;
    w_OR <= i_a or i_b;
end qualquernome;
