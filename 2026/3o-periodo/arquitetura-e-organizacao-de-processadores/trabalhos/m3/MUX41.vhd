library ieee;
use ieee.std_logic_1164.all; --entrada e saída
use ieee.numeric_std.all; --matemática

--std_logic é uma saída equivalente ao binário
entity MUX41 is
port (i_a, i_b, i_c, i_d : in std_logic_vector(31 downto 0); 
      i_sel: in std_logic_vector(1 downto 0); 
      o_data : out std_logic_vector(31 downto 0));
end MUX41;

architecture qualquernome of MUX41 is
begin
	process (i_a, i_b, i_sel)
    begin
    	if (i_sel = "00") then
        	o_data <= i_a;
        elsif (i_sel = "01") then
        	o_data <= i_b;
        elsif (i_sel = "10") then
        	o_data <= i_c;
        else
        	o_data <= i_d;
        end if;
    end process;
end qualquernome;