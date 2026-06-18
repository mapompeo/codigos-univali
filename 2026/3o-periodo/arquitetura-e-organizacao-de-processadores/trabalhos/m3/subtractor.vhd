-- Code your design here
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all; --equivalente a nossa biblioteca math.h
--desenho em vermelho
entity subtractor is
    port(a,b : in std_logic_vector(31 downto 0);
         s : out std_logic_vector(31 downto 0));
end subtractor;

--desenho em preto
architecture arch of subtractor is
begin
    s <= std_logic_vector(signed(a) - signed(b));


end arch;
