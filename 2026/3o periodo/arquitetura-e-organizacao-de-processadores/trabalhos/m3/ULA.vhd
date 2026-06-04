-- =============================================================================
--  ULA.vhd  --  Unidade Lógica e Aritmética  --  RISC-V 32-bit Monociclo
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
-- =============================================================================

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ULA is
port (i_a, i_b : in std_logic_vector(31 downto 0);
      i_sel : in std_logic_vector(3 downto 0);
      o_res : out std_logic_vector(31 downto 0);
      o_zero : out std_logic);
end ULA;

architecture estrutural of ULA is
    signal w_SOMA   : std_logic_vector(31 downto 0);
    signal w_SUB    : std_logic_vector(31 downto 0);
    signal w_AND    : std_logic_vector(31 downto 0);
    signal w_OR     : std_logic_vector(31 downto 0);
    signal w_XOR    : std_logic_vector(31 downto 0);
    signal w_SLL    : std_logic_vector(31 downto 0);
    signal w_SRL    : std_logic_vector(31 downto 0);
    signal w_SRA    : std_logic_vector(31 downto 0);
    signal w_SLT    : std_logic_vector(31 downto 0);
    signal w_SLTU   : std_logic_vector(31 downto 0);
    signal w_shamt  : integer range 0 to 31;
begin
    w_shamt <= to_integer(unsigned(i_b(4 downto 0)));

    u_SOMADOR : entity work.SOMADOR
    port map(i_a, i_b, w_SOMA);

    u_SUBTRATOR : entity work.SUBTRATOR
    port map(i_a, i_b, w_SUB);

    w_AND <= i_a and i_b;
    w_OR  <= i_a or i_b;
    w_XOR <= i_a xor i_b;

    u_SLL : entity work.DESLOCADOR
    generic map("00")
    port map(i_a, w_shamt, w_SLL);

    u_SRL : entity work.DESLOCADOR
    generic map("01")
    port map(i_a, w_shamt, w_SRL);

    u_SRA : entity work.DESLOCADOR
    generic map("10")
    port map(i_a, w_shamt, w_SRA);

    w_SLT  <= (0 => '1', others => '0') when signed(i_a) < signed(i_b) else (others => '0');
    w_SLTU <= (0 => '1', others => '0') when unsigned(i_a) < unsigned(i_b) else (others => '0');

    process(i_sel, w_SOMA, w_SUB, w_AND, w_OR, w_XOR,
            w_SLL, w_SRL, w_SRA, w_SLT, w_SLTU)
    begin
        case i_sel is
            when "0000" => o_res <= w_SOMA;
            when "0001" => o_res <= w_SUB;
            when "0010" => o_res <= w_AND;
            when "0011" => o_res <= w_OR;
            when "0100" => o_res <= w_XOR;
            when "0101" => o_res <= w_SLL;
            when "0110" => o_res <= w_SRL;
            when "0111" => o_res <= w_SRA;
            when "1000" => o_res <= w_SLT;
            when "1001" => o_res <= w_SLTU;
            when others => o_res <= (others => '0');
        end case;
    end process;

    o_zero <= '1' when o_res = (o_res'range => '0') else '0';
end estrutural;
