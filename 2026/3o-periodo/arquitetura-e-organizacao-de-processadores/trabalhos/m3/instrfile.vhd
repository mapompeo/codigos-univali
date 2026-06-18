-- =============================================================================
--  instrfile.vhd  --  Memória de Instruções  --  RISC-V 32-bit Monociclo
-- =============================================================================
--
--  ROM combinacional de 32 palavras (128 bytes).
--  Endereçamento por byte (igual ao PC), word-aligned: índice = addr[6:2]
--
--  Programa de teste:
--    PC= 0: ADDI x5,  x0,  6    →  x5  = 6
--    PC= 4: ADDI x6,  x0,  4    →  x6  = 4
--    PC= 8: ADD  x7,  x5,  x6   →  x7  = 6 + 4  = 10
--    PC=12: SUB  x8,  x7,  x6   →  x8  = 10 - 4 = 6
--    PC=16: AND  x9,  x5,  x6   →  x9  = 6 & 4  = 4
--    PC=20: OR   x10, x5,  x6   →  x10 = 6 | 4  = 6
--    PC=24: SW   x5,  0(x0)     →  mem[0] = 6
--    PC=28: LW   x11, 0(x0)     →  x11 = mem[0] = 6
--    PC=32: BEQ  x5,  x11, +8   →  x5==x11 (6==6): desvia para PC=40
--    PC=36: ADDI x12, x0, 99    →  NÃO executa (branch tomado)
--    PC=40: ADDI x13, x0, 1     →  x13 = 1  (confirma que branch funcionou)
--    PC=44+: NOP
-- =============================================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity InstrFile is
    port(
        i_addr : in  std_logic_vector(31 downto 0);
        o_instr: out std_logic_vector(31 downto 0)
    );
end InstrFile;

architecture rtl of InstrFile is
    type mem_t is array(0 to 31) of std_logic_vector(31 downto 0);
    constant MEM : mem_t := (
        0  => "00000000011000000000001010010011", -- ADDI x5,  x0,  6
        1  => "00000000010000000000001100010011", -- ADDI x6,  x0,  4
        2  => "00000000011000101000001110110011", -- ADD  x7,  x5,  x6
        3  => "01000000011000111000010000110011", -- SUB  x8,  x7,  x6
        4  => "00000000011000101111010010110011", -- AND  x9,  x5,  x6
        5  => "00000000011000101110010100110011", -- OR   x10, x5,  x6
        6  => "00000000010100000010000000100011", -- SW   x5,  0(x0)
        7  => "00000000000000000010010110000011", -- LW   x11, 0(x0)
        8  => "00000000101100101000010001100011", -- BEQ  x5,  x11, +8
        9  => "00000110001100000000011000010011", -- ADDI x12, x0,  99  (não executa)
        10 => "00000000000100000000011010010011", -- ADDI x13, x0,  1
        others => "00000000000000000000000000010011"  -- NOP: ADDI x0, x0, 0
    );
begin
    o_instr <= MEM(to_integer(unsigned(i_addr(6 downto 2))));
end rtl;
