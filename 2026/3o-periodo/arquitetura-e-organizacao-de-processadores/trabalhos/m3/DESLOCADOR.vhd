library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity DESLOCADOR is
generic (g_tipo : std_logic_vector(1 downto 0) := "00");
port (i_a : in std_logic_vector(31 downto 0);
      i_shamt : in integer range 0 to 31;
      o_data : out std_logic_vector(31 downto 0));
end DESLOCADOR;

architecture qualquernome of DESLOCADOR is
begin
    process(i_a, i_shamt)
    begin
        if g_tipo = "00" then
            o_data <= std_logic_vector(shift_left(unsigned(i_a), i_shamt));
        elsif g_tipo = "01" then
            o_data <= std_logic_vector(shift_right(unsigned(i_a), i_shamt));
        else
            o_data <= std_logic_vector(shift_right(signed(i_a), i_shamt));
        end if;
    end process;
end qualquernome;
