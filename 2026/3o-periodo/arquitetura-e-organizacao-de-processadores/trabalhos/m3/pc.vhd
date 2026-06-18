-- =============================================================================
--  pc.vhd  --  Program Counter  --  RISC-V 32-bit Monociclo
-- =============================================================================
--
--  Registrador de 32 bits que guarda o endereço da instrução atual.
--  Na borda de subida do clock carrega o próximo PC (i_pc_next).
--  Reset síncrono: quando i_rst='1' volta para o endereço 0.
-- =============================================================================

library IEEE;
use IEEE.std_logic_1164.all;

entity PC is
    port(
        i_clk    : in  std_logic;
        i_rst    : in  std_logic;
        i_pc_next: in  std_logic_vector(31 downto 0);
        o_pc     : out std_logic_vector(31 downto 0)
    );
end PC;

architecture rtl of PC is
    signal r_pc : std_logic_vector(31 downto 0) := (others => '0');
begin
    process(i_clk)
    begin
        if rising_edge(i_clk) then
            if i_rst = '1' then
                r_pc <= (others => '0');
            else
                r_pc <= i_pc_next;
            end if;
        end if;
    end process;

    o_pc <= r_pc;
end rtl;
