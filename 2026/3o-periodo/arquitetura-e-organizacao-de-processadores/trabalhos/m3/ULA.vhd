-- =============================================================================
--  ula.vhd  --  Unidade Lógica e Aritmética  --  RISC-V 32-bit Monociclo
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
-- SLTU : res 1 quando a < b (sem sinal)
-- =============================================================================

-- Code your design here
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all; --equivalente a nossa biblioteca math.h
--desenho em vermelho
entity ula is
    port(
        i_a,i_b : in std_logic_vector(31 downto 0);
        i_ula_op: in std_logic_vector(3 downto 0);
        o_ula : out std_logic_vector(31 downto 0);
        o_zero: out std_logic
        );

end ula;


architecture arch of ula is
    signal w_ula : std_logic_vector(31 downto 0);
    -- shamt: os 5 bits menos significativos de i_b definem o deslocamento
    signal shamt : integer range 0 to 31;
begin
    shamt <= to_integer(unsigned(i_b(4 downto 0)));

    process(i_a, i_b, i_ula_op, shamt)
    begin
        if (i_ula_op = "0000") then -- add
            w_ula <= std_logic_vector(signed(i_a) + signed(i_b));
        elsif(i_ula_op = "0001") then -- sub
            w_ula <= std_logic_vector(signed(i_a) - signed(i_b));
        elsif(i_ula_op = "0010") then -- and
            w_ula <= i_a and i_b;
        elsif(i_ula_op = "0011") then -- or
            w_ula <= i_a or i_b;
        elsif(i_ula_op = "0100") then -- xor
            w_ula <= i_a xor i_b;
        elsif(i_ula_op = "0101") then -- sll
            w_ula <= std_logic_vector(shift_left(unsigned(i_a), shamt));
        elsif(i_ula_op = "0110") then -- srl
            w_ula <= std_logic_vector(shift_right(unsigned(i_a), shamt));
        elsif(i_ula_op = "0111") then -- sra
            w_ula <= std_logic_vector(shift_right(signed(i_a), shamt));
        elsif(i_ula_op = "1000") then -- slt
            if signed(i_a) < signed(i_b) then
                w_ula <= "00000000000000000000000000000001";
            else
                w_ula <= (others => '0');
            end if;
        else -- sltu
            if unsigned(i_a) < unsigned(i_b) then
                w_ula <= "00000000000000000000000000000001";
            else
                w_ula <= (others => '0');
            end if;
        end if;

    end process;


    o_zero <= '1' when w_ula = "00000000000000000000000000000000" else '0';
    o_ula <= w_ula;

end arch;
