-- =============================================================================
--  dmem.vhd  --  Memória de Dados  --  RISC-V 32-bit Monociclo
-- =============================================================================
--
--  RAM de 64 palavras (256 bytes), endereçada por byte (word-aligned).
--  Leitura: combinacional — rdata disponível imediatamente.
--  Escrita: síncrona na borda de subida do clk quando mem_write='1'.
--
--  Índice = addr[7:2]  (ignora os 2 bits menos significativos)
-- =============================================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity DataMem is
    port(
        i_clk      : in  std_logic;
        i_mem_write: in  std_logic;
        i_addr     : in  std_logic_vector(31 downto 0);
        i_wdata    : in  std_logic_vector(31 downto 0);
        o_rdata    : out std_logic_vector(31 downto 0)
    );
end DataMem;

architecture rtl of DataMem is
    type mem_t is array(0 to 63) of std_logic_vector(31 downto 0);
    signal MEM : mem_t := (others => (others => '0'));
begin

    -- Leitura combinacional
    o_rdata <= MEM(to_integer(unsigned(i_addr(7 downto 2))));

    -- Escrita síncrona
    process(i_clk)
    begin
        if rising_edge(i_clk) then
            if i_mem_write = '1' then
                MEM(to_integer(unsigned(i_addr(7 downto 2)))) <= i_wdata;
            end if;
        end if;
    end process;

end rtl;
