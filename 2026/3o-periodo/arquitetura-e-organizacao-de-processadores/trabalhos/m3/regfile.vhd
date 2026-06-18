-- =============================================================================
--  regfile.vhd  --  Banco de Registradores  --  RISC-V 32-bit Monociclo
-- =============================================================================
--
--  32 registradores de 32 bits (x0 a x31).
--  x0 é hardwired zero: qualquer escrita em x0 é ignorada.
--
--  Leitura:  combinacional — rdata1/rdata2 disponíveis imediatamente.
--  Escrita:  síncrona na borda de subida do clk quando we='1'.
-- =============================================================================

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RegFile is
    port (
        clk    : in  std_logic;
        we     : in  std_logic;                      -- write enable
        rs1    : in  std_logic_vector(4 downto 0);   -- endereço leitura 1
        rs2    : in  std_logic_vector(4 downto 0);   -- endereço leitura 2
        rd     : in  std_logic_vector(4 downto 0);   -- endereço escrita
        wdata  : in  std_logic_vector(31 downto 0);  -- dado a escrever
        rdata1 : out std_logic_vector(31 downto 0);  -- dado lido de rs1
        rdata2 : out std_logic_vector(31 downto 0)   -- dado lido de rs2
    );
end RegFile;

architecture rtl of RegFile is
    type rf_t is array(0 to 31) of std_logic_vector(31 downto 0);
    signal RF : rf_t := (others => (others => '0'));
begin

    -- Leitura combinacional (x0 sempre retorna zero)
    rdata1 <= (others => '0') when rs1 = "00000"
              else RF(to_integer(unsigned(rs1)));
    rdata2 <= (others => '0') when rs2 = "00000"
              else RF(to_integer(unsigned(rs2)));

    -- Escrita síncrona (ignora x0)
    process(clk)
    begin
        if rising_edge(clk) then
            if we = '1' and rd /= "00000" then
                RF(to_integer(unsigned(rd))) <= wdata;
            end if;
        end if;
    end process;

end rtl;
