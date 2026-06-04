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
-- SLTU : res 1 quando a < b (sem sinal)
-- =============================================================================

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity design is
port (i_a, i_b : in std_logic_vector(31 downto 0);
      i_sel : in std_logic_vector(3 downto 0);
      o_data : out std_logic_vector(31 downto 0);
      o_zero : out std_logic);
end design;

architecture qualquernome of design is
    signal w_res : std_logic_vector(31 downto 0);
    signal w_shamt : integer range 0 to 31;
begin
    w_shamt <= to_integer(unsigned(i_b(4 downto 0)));

    process(i_a, i_b, i_sel, w_shamt)
    begin
        case i_sel is
            when "0000" => w_res <= std_logic_vector(signed(i_a) + signed(i_b));
            when "0001" => w_res <= std_logic_vector(signed(i_a) - signed(i_b));
            when "0010" => w_res <= i_a and i_b;
            when "0011" => w_res <= i_a or i_b;
            when "0100" => w_res <= i_a xor i_b;
            when "0101" => w_res <= std_logic_vector(shift_left(unsigned(i_a), w_shamt));
            when "0110" => w_res <= std_logic_vector(shift_right(unsigned(i_a), w_shamt));
            when "0111" => w_res <= std_logic_vector(shift_right(signed(i_a), w_shamt));
            when "1000" =>
                if signed(i_a) < signed(i_b) then
                    w_res <= (others => '0');
                    w_res(0) <= '1';
                else
                    w_res <= (others => '0');
                end if;
            when "1001" =>
                if unsigned(i_a) < unsigned(i_b) then
                    w_res <= (others => '0');
                    w_res(0) <= '1';
                else
                    w_res <= (others => '0');
                end if;
            when others => w_res <= (others => '0');
        end case;
    end process;

    o_data <= w_res;
    o_zero <= '1' when w_res = (w_res'range => '0') else '0';
end qualquernome;
