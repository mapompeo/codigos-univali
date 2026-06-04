library ieee;
use ieee.std_logic_1164.all; --entrada e saída
use ieee.numeric_std.all; --matemática

--std_logic é uma saída equivalente ao binário
entity SUBTRATOR is
port (i_a, i_b : in std_logic_vector(31 downto 0); 
      o_data : out std_logic_vector(31 downto 0));
end SUBTRATOR;

architecture qualquernome of SUBTRATOR is
begin
	o_data <= std_logic_vector(signed(i_a) - signed(i_b));
end qualquernome;